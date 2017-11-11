;##################################################
;##                                              ##
;##        [ Asm7x.List ][ IQSort4.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       УНИВЕРСАЛЬНЫЙ МОДУЛЬ ДЛЯ 16 БИТ        ##
;##                                              ##
;##            -= Smart Quick Sort =-            ##
;##        (for extended elenemt arrays)         ##
;##                                              ##
;##           Умная быстрая сортировка           ##
;##   (для массивов с расширенными элементами)   ##
;##                                              ##
;##           [ v1.00 :: 11.11.2017 ]            ##
;##           MASM/TASM (16 bit 386+)            ##
;##                                              ##
;##     (c) 2017 by Jin X (jin.x@sources.ru)     ##
;##             http://xk7.ru/p/a/i              ##
;##                                              ##
;##################################################

IFDEF		??Version	; TASM
  LOCALS
ENDIF

IQSort4_ver	=	100h			; версия данного модуля (word: старший байт - целая часть, младший - дробная)

; ПЕРЕД ВКЛЮЧЕНИЕМ ДАННОГО ФАЙЛА В ПРОЕКТ *МОЖНО* ОПРЕДЕЛИТЬ СЛЕДУЮЩИЕ СИМВОЛЫ:

; inclIQSort4 = 1 - включить процедуру IQSort4 (умная быстрая сортировка) в код, 0 - не включать [по умолчанию 1]

; inclInsSort4 = 1 - включить процедуру InsSort4 (сортировка вставками) в код, 0 - не включать [по умолчанию 0]
;   Если процедура InsSort4 используется процедурой IQSort4, в код включается лишь часть этой процедуры, необходимая для работы IQSort4

; Sort4SignCmp = 1 - знаковое сравнение, 0 - беззнаковое [по умолчанию 1]

; Sort4Ascending = 1 - сортировка по возрастанию (от меньшего к большему), 0 - по убыванию (от больего к меньшему)

; Sort4ElemDWords = кол-во двойных слов с данными (допускаются значения: 1 - только опорное слово, 2 - опорное слово и связанные данные; другие значения породят ошибки)
;   [*ЭТОТ СИМВОЛ НЕОБХОДИМО ОПРЕДЕЛИТЬ ОБЯЗАТЕЛЬНО*, значения по умолчанию для него нет!!!]

; IQSort4InsThrs = порог использования сортировки вставками (когда кол-во элементов меньше указанного здесь значения) [0 = отключено; минимум 2; по умолчанию 16]
;   Если кол-во элементов (на первой или последюущей итерации) больше или равно значению IQSort4InsThrs, используется быстрая сортировка, иначе
;   используется сортировка вставками - это немного ускоряет процесс (при правильном выборе значения IQSort4InsThrs, например, при значении по умолчанию)
;   и уменьшает глубину рекурсии, защищая стек от переполнения

; IQSort4MaxStk = максимальный размер стека в байтах, который допустимо использовать процедуре IQSort4 (включая call из основного кода) [0 = отключено; минимум 12; по умолчанию 128]
;   Каждый уровень вложенности использует 4 байта (2 слова) стека (первый уровень - до 4-х слов, последний может использовать на 1-2 слова больше, итого минимум 10-12 байт),
;   т.о. 128 байт позволяют организовать до 30 уровней рекурсии, что достаточно даже для очень больших массивов, особенно когда IQSort4InsThrs > 0

; При установке значений IQSort4InsThrs = IQSort4MaxStk = 0 процедура IQSort4 превращается в процедуру стандартной быстрой сортировки (т.е. некомбинированную)

;-----------------------------------------------------------------------------------------------------------------------

_defdef		MACRO	Const:REQ, DefVal:REQ
  IFDEF		Const
	_&Const	=	Const
  ELSE
	_&Const	=	DefVal
  ENDIF
ENDM

@386		=	((@Cpu and 8) ne 0) or ((@Cpu and 2Fh) eq 0)

IF	not @386
  IFDEF		??Version	; TASM
    .ERR	"This module requires 386+ instructions enabled !!!"
  ELSE				; MASM
    .ERR	<This module requires 386+ instructions enabled !!!>
  ENDIF
ENDIF

_defdef	inclIQSort4, 1
_defdef	inclInsSort4, 0

_defdef	Sort4SignCmp, 1
_defdef	Sort4Ascending, 1

IF	_Sort4SignCmp
  IF	_Sort4Ascending
    srt?jl	EQU	<jl>
    srt?jg	EQU	<jg>
  ELSE
    srt?jl	EQU	<jg>
    srt?jg	EQU	<jl>
  ENDIF
ELSE
  IF	_Sort4Ascending
    srt?jl	EQU	<jb>
    srt?jg	EQU	<ja>
  ELSE
    srt?jl	EQU	<ja>
    srt?jg	EQU	<jb>
  ENDIF
ENDIF

_Sort4ElemDWords =	Sort4ElemDWords
IF	(_Sort4ElemDWords+1)/2 ne 1
  IFDEF		??Version	; TASM
    .ERR	"Wrong value of Sort4ElemDWords, it must be = 1 or 2 !!!"
  ELSE				; MASM
    .ERR	<Wrong value of Sort4ElemDWords, it must be = 1 or 2 !!!>
  ENDIF
ENDIF
_Sort4ElemSize	=	_Sort4ElemDWords*4	; размер одного элемента массива (4 или 8 байт)

_defdef	IQSort4InsThrs, 16
_defdef	IQSort4MaxStk, 128

IF	_IQSort4InsThrs and _IQSort4InsThrs lt 2
  _IQSort4InsThrs =	0			; если IQSort4InsThrs < 2, принимаем значение 0
ENDIF

IF	_IQSort4MaxStk and _IQSort4MaxStk lt 10+(_Sort4ElemDWords-1)*2
  _IQSort4MaxStk =	0			; если IQSort4MaxStk < 10 + 2 (если Sort4ElemDWords = 2), принимаем значение 0
ENDIF

;-- IQSort4: Умная БЫСТРАЯ СОРТИРОВКА массива (комбинированным методом) ------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. Sort4ElemDWords = 2 (размер элемента массива = _Sort4ElemSize = 8 байтам), то:
;   * первое двойное слово содержит опорное значение (по которому происходит сравнение),
;   * второе двойное слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Если кол-во элементов (на первой или последующей итерации) больше или равно значению IQSort4InsThrs, используется быстрая сортировка,
;   иначе используется сортировка вставками [только если IQSort4InsThrs <> 0]
; Сортировка вставками также используется, если для следующего уровня рекурсии потребуется более IQSort4MaxStk байт стека в общем сложности
;   [только если IQSort4MaxStk <> 0]
; Процедура изменяет регистры EAX, EBX, CX, DX, SI, DI, старшее слово EBP; сохраняет BP и сегментные регистры
IF	_inclIQSort4
IQSort4		PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		push	bp
		xor	bp,bp			; BP = кол-во рекурсий
		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура быстрой сортировки
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX, BP = уровень рекурсии
	@@IQSort4Main:
IF	_IQSort4InsThrs
		mov	ax,cx
		sub	ax,dx
		shr	ax,_Sort4ElemDWords+1	; AX = кол-во элементов минус 1
		cmp	ax,_IQSort4InsThrs-1
		jb	@@callins		; если кол-во элементов меньше порогового значения, используем сортировку вставками
ENDIF
		mov	si,dx			;; I (SI) := L (DX)
	@@repeat1:				;; repeat
		mov	di,cx			;; J (DI) := R (CX)
		mov	bx,cx
		sub	bx,dx
		shr	bx,1
		and	bx,-_Sort4ElemSize
		add	bx,dx			;; P (BX) := (L + R) / 2
		mov	eax,[bx]		;; T (EAX) := [P]
	@@repeat2 = @@cmpI			;; repeat
		; SI = I, EAX = T, DI = J, DX = L, CX = R
		jmp	@@cmpI
	@@addI:	add	si,_Sort4ElemSize	;; Inc(I)
	@@cmpI:	cmp	[si],eax		;; while [I] < T
		srt?jl	@@addI

		jmp	@@cmpJ
	@@subJ:	sub	di,_Sort4ElemSize	;; Dec(J)
	@@cmpJ:	cmp	[di],eax		;; while [J] > T
		srt?jg	@@subJ

		cmp	si,di
		jnbe	@@noswap		;; if I <= J then

		mov	ebx,[si]		;;   Swap [I],[J]
		xchg	[di],ebx
		mov	[si],ebx
IF	_Sort4ElemDWords gt 1
		mov	ebx,[si+4]
		xchg	[di+4],ebx
		mov	[si+4],ebx
ENDIF
		add	si,_Sort4ElemSize	;; Inc(I)
		sub	di,_Sort4ElemSize	;; Dec(J)
	@@noswap:
		cmp	si,di
		jna	@@repeat2		;; until I > J

		cmp	dx,di
		jnb	@@norecurs		;; if L < J then

		push	cx
		push	si			; сохраняем R и I
		mov	cx,di
		; DX = L, CX = J
IF	_IQSort4MaxStk
		cmp	bp,(_IQSort4MaxStk-(10+(_Sort4ElemDWords-1)*2))/4	; 6 слов - это адрес возврата в вызываемую программу +
						; bp + cx + si + адрес возврата из InsSort4 + bp (если Sort4ElemDWords = 2)
		jae	@@callins2		; если число рекурсий достигло максимума, идём на вызов сортировки вставками: InsSort4(L, J)
ENDIF
		inc	bp			; иначе увеличиваем глубину рекурсии и идём на рекурсию
		jmp	@@IQSort4Main		;;   IQSort4(L, J); вызов делаем через jmp для экономии стека :)
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
IF	_IQSort4InsThrs
	@@callins:
		push	offset @@finish		; адрес возврата
		jmp	@IQInsSort4		; вместо call + jmp @@finish делаем push + jmp
ENDIF
IF	_IQSort4MaxStk
	@@callins2:
		push	offset @@recursret	; адрес возврата
		jmp	@IQInsSort4		; вместо call + jmp @@recursret делаем push + jmp
ENDIF
IQSort4		ENDP
ENDIF ; _inclIQSort4

;-- InsSort4: СОРТИРОВКА массива ВСТАВКАМИ -----------------------------------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. Sort4ElemDWords = 2 (размер элемента массива = _Sort4ElemSize = 8 байтам), то:
;   * первое двойное слово содержит опорное значение (по которому происходит сравнение),
;   * второе двойное слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Процедура изменяет регистры EAX, EBX, SI, DI, старшее слово EBP; сохраняет CX, DX, BP и сегментные регистры
IF	_inclInsSort4
InsSort4	PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура сортировки вставками
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX
IFDEF		??Version	; TASM
  @IQInsSort4:
ELSE				; MASM
  @IQInsSort4::
ENDIF
ELSEIF	_IQSort4InsThrs or _IQSort4MaxStk
@IQInsSort4	PROC
ENDIF
IF	_inclInsSort4 or _IQSort4InsThrs or _IQSort4MaxStk
IF	_Sort4ElemDWords gt 1
		push	bp
ENDIF
		mov	di,dx			; J (DI) := L - адрес первого элемента
	@@next:					;; for J (DI) := L+1 (CX) to R (DX) do
		add	di,_Sort4ElemSize	; J++ (DI) - адрес следующего проверяемого элемента (в основном цикле)
		mov	ebx,[di]		;; T (EBP:EBX|EBX) := [J]
IF	_Sort4ElemDWords gt 1
		mov	ebp,[di+4]
ENDIF
		mov	si,di			; I+1 (SI) := DI - адрес элемента, следующего за сравниваемым (во внутреннем цикле)
	@@loop:					;; repeat
		mov	eax,[si-_Sort4ElemSize]
		cmp	eax,ebx			;; if [I] > T then
%		srt?jl&e @@break		; прыгаем, если [I] <= T

		mov	[si],eax		;;  [I+1] := [I] else Break
IF	_Sort4ElemDWords gt 1
		mov	eax,[si-_Sort4ElemSize+4]
		mov	[si+4],eax
ENDIF
		sub	si,_Sort4ElemSize	;; Dec(I)
		cmp	si,dx
		jnbe	@@loop			;; until I < L (I+1 <= L)
	@@break:
		mov	[si],ebx		;; [I+1] := T
IF	_Sort4ElemDWords gt 1
		mov	[si+4],ebp
ENDIF
		cmp	di,cx
		jnae	@@next			; следующий элемент массива ;; end for
IF	_Sort4ElemDWords gt 1
		pop	bp
ENDIF
	@@exit:	ret
ENDIF ; _inclInsSort4 or _IQSort4InsThrs or _IQSort4MaxStk
IF	_inclInsSort4
InsSort4	ENDP
ELSEIF	_IQSort4InsThrs or _IQSort4MaxStk
@IQInsSort4	ENDP
ENDIF
