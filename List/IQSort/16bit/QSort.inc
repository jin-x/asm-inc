;##################################################
;##                                              ##
;##        [ Asm7x.List ][ QSort.inc ]           ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       УНИВЕРСАЛЬНЫЙ МОДУЛЬ ДЛЯ 16 БИТ        ##
;##                                              ##
;##               -= Quick Sort =-               ##
;##              Быстрая сортировка              ##
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

QSort_ver	=	100h			; версия данного модуля (word: старший байт - целая часть, младший - дробная)

; ПЕРЕД ВКЛЮЧЕНИЕМ ДАННОГО ФАЙЛА В ПРОЕКТ *МОЖНО* ОПРЕДЕЛИТЬ СЛЕДУЮЩИЕ СИМВОЛЫ:

; SortSignCmp = 1 - знаковое сравнение, 0 - беззнаковое [по умолчанию 1]

; SortAscending = 1 - сортировка по возрастанию (от меньшего к большему), 0 - по убыванию (от больего к меньшему)

; SortElemWords = кол-во слов с данными (допускаются значения: 1 - только опорное слово, 2 - опорное слово и связанные данные; другие значения породят ошибки)
;   [*ЭТОТ СИМВОЛ НЕОБХОДИМО ОПРЕДЕЛИТЬ ОБЯЗАТЕЛЬНО*, значения по умолчанию для него нет!!!]

;-----------------------------------------------------------------------------------------------------------------------

_defdef		MACRO	Const:REQ, DefVal:REQ
  IFDEF		const
	_&Const	=	Const
  ELSE
	_&Const	=	DefVal
  ENDIF
ENDM

@386		=	((@Cpu and 8) ne 0) or ((@Cpu and 2Fh) eq 0)

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

;-- QSort: БЫСТРАЯ СОРТИРОВКА массива ----------------------------------------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. SortElemWords = 2 (размер элемента массива = _SortElemSize = 4 байтам), то:
;   * первое слово содержит опорное значение (по которому происходит сравнение),
;   * второе слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Процедура изменяет регистры AX, (E)BX, CX, DX, SI, DI, сохраняет BP и сегментные регистры
QSort		PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		push	bp
		xor	bp,bp			; BP = кол-во рекурсий
		shl	cx,_SortElemWords
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура быстрой сортировки
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX, BP = уровень рекурсии
	@@QSortMain:
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

IF	_SortElemWords gt 1 and @386
		mov	ebx,[si]		;;   Swap [I],[J]
		xchg	[di],ebx
		mov	[si],ebx
ELSE
		mov	bx,[si]
		xchg	[di],bx
		mov	[si],bx
  IF	_SortElemWords gt 1
		mov	bx,[si+2]
		xchg	[di+2],bx
		mov	[si+2],bx
  ENDIF
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
		inc	bp			; увеличиваем глубину рекурсии и идём на рекурсию
		jmp	@@QSortMain		;;   QSort(L, J); вызов делаем через jmp для экономии стека :)
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
QSort		ENDP
