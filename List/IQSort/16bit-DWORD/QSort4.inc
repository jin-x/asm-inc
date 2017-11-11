;##################################################
;##                                              ##
;##         [ Asm7x.List ][ QSort4.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       УНИВЕРСАЛЬНЫЙ МОДУЛЬ ДЛЯ 16 БИТ        ##
;##                                              ##
;##               -= Quick Sort =-               ##
;##        (for extended elenemt arrays)         ##
;##                                              ##
;##              Быстрая сортировка              ##
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

QSort4_ver	=	100h			; версия данного модуля (word: старший байт - целая часть, младший - дробная)

; ПЕРЕД ВКЛЮЧЕНИЕМ ДАННОГО ФАЙЛА В ПРОЕКТ *МОЖНО* ОПРЕДЕЛИТЬ СЛЕДУЮЩИЕ СИМВОЛЫ:

; Sort4SignCmp = 1 - знаковое сравнение, 0 - беззнаковое [по умолчанию 1]

; Sort4Ascending = 1 - сортировка по возрастанию (от меньшего к большему), 0 - по убыванию (от больего к меньшему)

; Sort4ElemDWords = кол-во двойных слов с данными (допускаются значения: 1 - только опорное слово, 2 - опорное слово и связанные данные; другие значения породят ошибки)
;   [*ЭТОТ СИМВОЛ НЕОБХОДИМО ОПРЕДЕЛИТЬ ОБЯЗАТЕЛЬНО*, значения по умолчанию для него нет!!!]

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

;-- QSort4: БЫСТРАЯ СОРТИРОВКА массива ----------------------------------------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. Sort4ElemDWords = 2 (размер элемента массива = _Sort4ElemSize = 8 байтам), то:
;   * первое двойное слово содержит опорное значение (по которому происходит сравнение),
;   * второе двойное слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Процедура изменяет регистры EAX, EBX, CX, DX, SI, DI, старшее слово EBP; сохраняет BP и сегментные регистры
QSort4		PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		push	bp
		xor	bp,bp			; BP = кол-во рекурсий
		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура быстрой сортировки
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX, BP = уровень рекурсии
	@@QSort4Main:
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

		mov	ebx,[si]
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
		inc	bp			; увеличиваем глубину рекурсии и идём на рекурсию
		jmp	@@QSort4Main		;;   QSort4(L, J); вызов делаем через jmp для экономии стека :)
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
QSort4		ENDP
