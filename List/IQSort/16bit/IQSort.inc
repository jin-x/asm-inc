;##################################################
;##                                              ##
;##         [ Asm7x.List ][ IQSort.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       УНИВЕРСАЛЬНЫЙ МОДУЛЬ ДЛЯ 16 БИТ        ##
;##                                              ##
;##            -= Smart Quick Sort =-            ##
;##           Умная быстрая сортировка           ##
;##                                              ##
;##           [ v1.00 :: 11.11.2017 ]            ##
;##              MASM/TASM (16 bit)              ##
;##                                              ##
;##     (c) 2017 by Jin X (jin.x@sources.ru)     ##
;##             http://xk7.ru/p/a/i              ##
;##                                              ##
;##################################################

IFDEF		??Version	; TASM
  LOCALS
ENDIF

IQSort_ver	=	100h			; версия данного модуля (word: старший байт - целая часть, младший - дробная)

; ПЕРЕД ВКЛЮЧЕНИЕМ ДАННОГО ФАЙЛА В ПРОЕКТ *МОЖНО* ОПРЕДЕЛИТЬ СЛЕДУЮЩИЕ СИМВОЛЫ:

; inclIQSort = 1 - включить процедуру IQSort (умная быстрая сортировка) в код, 0 - не включать [по умолчанию 1]

; inclInsSort = 1 - включить процедуру InsSort (сортировка вставками) в код, 0 - не включать [по умолчанию 0]
;   Если процедура InsSort используется процедурой IQSort, в код включается лишь часть этой процедуры, необходимая для работы IQSort

; SortSignCmp = 1 - знаковое сравнение, 0 - беззнаковое [по умолчанию 1]

; SortAscending = 1 - сортировка по возрастанию (от меньшего к большему), 0 - по убыванию (от больего к меньшему)

; SortElemWords = кол-во слов с данными (допускаются значения: 1 - только опорное слово, 2 - опорное слово и связанные данные; другие значения породят ошибки)
;   [*ЭТОТ СИМВОЛ НЕОБХОДИМО ОПРЕДЕЛИТЬ ОБЯЗАТЕЛЬНО*, значения по умолчанию для него нет!!!]

; IQSortInsThrs = порог использования сортировки вставками (когда кол-во элементов меньше указанного здесь значения) [0 = отключено; минимум 2; по умолчанию 16]
;   Если кол-во элементов (на первой или последюущей итерации) больше или равно значению IQSortInsThrs, используется быстрая сортировка, иначе
;   используется сортировка вставками - это немного ускоряет процесс (при правильном выборе значения IQSortInsThrs, например, при значении по умолчанию)
;   и уменьшает глубину рекурсии, защищая стек от переполнения

; IQSortMaxStk = максимальный размер стека в байтах, который допустимо использовать процедуре IQSort (включая call из основного кода) [0 = отключено; минимум 12; по умолчанию 128]
;   Каждый уровень вложенности использует 4 байта (2 слова) стека (первый уровень - до 4-х слов, последний может использовать на 1-2 слова больше, итого минимум 10-12 байт),
;   т.о. 128 байт позволяют организовать до 30 уровней рекурсии, что достаточно даже для очень больших массивов, особенно когда IQSortInsThrs > 0

; При установке значений IQSortInsThrs = IQSortMaxStk = 0 процедура IQSort превращается в процедуру стандартной быстрой сортировки (т.е. некомбинированную)

;-----------------------------------------------------------------------------------------------------------------------

_defdef		MACRO	Const:REQ, DefVal:REQ
  IFDEF		Const
	_&Const	=	Const
  ELSE
	_&Const	=	DefVal
  ENDIF
ENDM

@386		=	((@Cpu and 8) ne 0) or ((@Cpu and 2Fh) eq 0)

_defdef	inclIQSort, 1
_defdef	inclInsSort, 0

_defdef	SortSignCmp, 1
_defdef	SortAscending, 1

IF	_SortSignCmp
  IF	_SortAscending
    srt?jl	equ	<jl>
    srt?jg	equ	<jg>
  ELSE
    srt?jl	equ	<jg>
    srt?jg	equ	<jl>
  ENDIF
ELSE
  IF	_SortAscending
    srt?jl	equ	<jb>
    srt?jg	equ	<ja>
  ELSE
    srt?jl	equ	<ja>
    srt?jg	equ	<jb>
  ENDIF
ENDIF

_SortElemWords	=	SortElemWords
IF	(_SortElemWords+1)/2 ne 1
  IFDEF		??Version	; TASM
    .ERR	"Wrong value of SortElemWords, it must be = 1 or 2 !!!"
  ELSE				; MASM
    .ERR	<Wrong value of SortElemWords, it must be = 1 or 2 !!!>
  ENDIF
ENDIF
_SortElemSize	=	_SortElemWords*2	; размер одного элемента массива (2 или 4 байта)

_defdef	IQSortInsThrs, 16
_defdef	IQSortMaxStk, 128

IF	_IQSortInsThrs and _IQSortInsThrs lt 2
  _IQSortInsThrs = 0				; если IQSortInsThrs < 2, принимаем значение 0
ENDIF

IF	_IQSortMaxStk and _IQSortMaxStk lt 10-(_SortElemWords gt 1 and not @386)*2
  _IQSortMaxStk = 0				; если IQSortMaxStk < 10 + 2 (если SortElemWords = 2 или инструкции 386 отключены), принимаем значение 0
ENDIF

IF	_SortElemWords gt 1 and @386
  srt?ax	equ	<eax>
  srt?bx	equ	<ebx>
ELSE
  srt?ax	equ	<ax>
  srt?bx	equ	<bx>
ENDIF

IF	_SortElemWords gt 1 and @386
  srt?xchgreg	EQU	<ebx>
ELSE
  srt?xchgreg	EQU	<bx>
ENDIF

;-- IQSort: Умная БЫСТРАЯ СОРТИРОВКА массива (комбинированным методом) -------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. SortElemWords = 2 (размер элемента массива = _SortElemSize = 4 байтам), то:
;   * первое слово содержит опорное значение (по которому происходит сравнение),
;   * второе слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Если кол-во элементов (на первой или последующей итерации) больше или равно значению IQSortInsThrs, используется быстрая сортировка,
;   иначе используется сортировка вставками [только если IQSortInsThrs <> 0]
; Сортировка вставками также используется, если для следующего уровня рекурсии потребуется более IQSortMaxStk байт стека в общем сложности
;   [только если IQSortMaxStk <> 0]
; Процедура изменяет регистры AX, (E)BX, CX, DX, SI, DI, сохраняет BP и сегментные регистры
IF	_inclIQSort
IQSort		PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		push	bp
		xor	bp,bp			; BP = кол-во рекурсий
		shl	cx,_SortElemWords
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура быстрой сортировки
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX, BP = уровень рекурсии
	@@IQSortMain:
IF	_IQSortInsThrs
		mov	ax,cx
		sub	ax,dx
		shr	ax,_SortElemWords	; AX = кол-во элементов минус 1
		cmp	ax,_IQSortInsThrs-1
		jb	@@callins		; если кол-во элементов меньше порогового значения, используем сортировку вставками
ENDIF
		mov	si,dx			;; I (SI) := L (DX)
	@@repeat1:				;; repeat
		mov	di,cx			;; J (DI) := R (CX)
		mov	bx,cx
		sub	bx,dx
		shr	bx,1
		and	bx,-_SortElemSize
		add	bx,dx			;; P (BX) := (L + R) / 2
		mov	ax,[bx]			;; T (AX) := [P]
	@@repeat2 = @@cmpI			;; repeat
		; SI = I, AX = T, DI = J, DX = L, CX = R
		jmp	@@cmpI
	@@addI:	add	si,_SortElemSize	;; Inc(I)
	@@cmpI:	cmp	[si],ax			;; while [I] < T
		srt?jl	@@addI

		jmp	@@cmpJ
	@@subJ:	sub	di,_SortElemSize	;; Dec(J)
	@@cmpJ:	cmp	[di],ax			;; while [J] > T
		srt?jg	@@subJ

		cmp	si,di
		jnbe	@@noswap		;; if I <= J then

		mov	srt?xchgreg,[si]	;;   Swap [I],[J]
		xchg	[di],srt?xchgreg
		mov	[si],srt?xchgreg
IF	_SortElemWords gt 1 and not @386
		mov	srt?xchgreg,[si+2]
		xchg	[di+2],srt?xchgreg
		mov	[si+2],srt?xchgreg
ENDIF
		add	si,_SortElemSize	;; Inc(I)
		sub	di,_SortElemSize	;; Dec(J)
	@@noswap:
		cmp	si,di
		jna	@@repeat2		;; until I > J

		cmp	dx,di
		jnb	@@norecurs		;; if L < J then

		push	cx
		push	si			; сохраняем R и I
		mov	cx,di
		; DX = L, CX = J
IF	_IQSortMaxStk
		cmp	bp,(_IQSortMaxStk-(10-(_SortElemWords gt 1 and not @386)*2))/4	; 6 слов - это адрес возврата в вызываемую программу +
						; bp + cx + si + адрес возврата из InsSort + bp (если SortElemWords = 2 или инструкции 386 отключены)
		jae	@@callins2		; если число рекурсий достигло максимума, идём на вызов сортировки вставками: InsSort(L, J)
ENDIF
		inc	bp			; иначе увеличиваем глубину рекурсии и идём на рекурсию
		jmp	@@IQSortMain		;;   IQSort(L, J); вызов делаем через jmp для экономии стека :)
	@@recursret:
		pop	si
		pop	cx			; восстанавливаем I и R
	@@norecurs:
		mov	dx,si			;; L := I
		cmp	si,cx
		jnae	@@repeat1		;; until I >= R
	@@finish:
		dec	bp			; уменьшаем глубину рекурсии
		jns	@@recursret		; прыгаем, если это не первый (корневой) уровень рекурсии
		pop	bp
	@@exit:	ret
IF	_IQSortInsThrs
	@@callins:
		push	offset @@finish		; адрес возврата
		jmp	@IQInsSort		; вместо call + jmp @@finish делаем push + jmp
ENDIF
IF	_IQSortMaxStk
	@@callins2:
		push	offset @@recursret	; адрес возврата
		jmp	@IQInsSort		; вместо call + jmp @@recursret делаем push + jmp
ENDIF
IQSort		ENDP
ENDIF ; _inclIQSort

;-- InsSort: СОРТИРОВКА массива ВСТАВКАМИ ------------------------------------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. SortElemWords = 2 (размер элемента массива = _SortElemSize = 4 байтам), то:
;   * первое слово содержит опорное значение (по которому происходит сравнение),
;   * второе слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Процедура изменяет регистры AX, (E)BX, SI, DI, сохраняет CX, DX, BP и сегментные регистры
IF	_inclInsSort
InsSort		PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		shl	cx,_SortElemWords
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура сортировки вставками
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX
IFDEF		??Version	; TASM
  @IQInsSort:
ELSE				; MASM
  @IQInsSort::
ENDIF
ELSEIF	_IQSortInsThrs or _IQSortMaxStk
@IQInsSort	PROC
ENDIF
IF	_inclInsSort or _IQSortInsThrs or _IQSortMaxStk
IF	_SortElemWords gt 1 and not @386
		push	bp
ENDIF
		mov	di,dx			; J (DI) := L - адрес первого элемента
	@@next:					;; for J (DI) := L+1 (CX) to R (DX) do
		add	di,_SortElemSize	; J++ (DI) - адрес следующего проверяемого элемента (в основном цикле)
		mov	srt?bx,[di]		;; T (BP:BX|EBX) := [J]
IF	_SortElemWords gt 1 and not @386
		mov	bp,[di+2]
ENDIF
		mov	si,di			; I+1 (SI) := DI - адрес элемента, следующего за сравниваемым (во внутреннем цикле)
	@@loop:					;; repeat
		mov	srt?ax,[si-_SortElemSize]
		cmp	ax,bx			;; if [I] > T then
%		srt?jl&e @@break		; прыгаем, если [I] <= T

		mov	[si],srt?ax		;;  [I+1] := [I] else Break
IF	_SortElemWords gt 1 and not @386
		mov	ax,[si-_SortElemSize+2]
		mov	[si+2],ax
ENDIF
		sub	si,_SortElemSize	;; Dec(I)
		cmp	si,dx
		jnbe	@@loop			;; until I < L (I+1 <= L)
	@@break:
		mov	[si],srt?bx		;; [I+1] := T
IF	_SortElemWords gt 1 and not @386
		mov	[si+2],bp
ENDIF
		cmp	di,cx
		jnae	@@next			; следующий элемент массива ;; end for
IF	_SortElemWords gt 1 and not @386
		pop	bp
ENDIF
	@@exit:	ret
ENDIF ; _inclInsSort or _IQSortInsThrs or _IQSortMaxStk
IF	_inclInsSort
InsSort		ENDP
ELSEIF	_IQSortInsThrs or _IQSortMaxStk
@IQInsSort	ENDP
ENDIF
