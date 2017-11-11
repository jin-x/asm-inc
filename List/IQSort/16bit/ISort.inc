;##################################################
;##                                              ##
;##         [ Asm7x.List ][ ISort.inc ]          ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       УНИВЕРСАЛЬНЫЙ МОДУЛЬ ДЛЯ 16 БИТ        ##
;##                                              ##
;##             -= Insertion Sort =-             ##
;##             Сортировка вставками             ##
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

ISort_ver	=	100h			; версия данного модуля (word: старший байт - целая часть, младший - дробная)

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
    srt?jle	equ	<jle>
  ELSE
    srt?jle	equ	<jge>
  ENDIF
ELSE
  IF	_SortAscending
    srt?jle	equ	<jbe>
  ELSE
    srt?jle	equ	<jae>
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

;-- InsSort: СОРТИРОВКА массива ВСТАВКАМИ ------------------------------------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. SortElemWords = 2 (размер элемента массива = _SortElemSize = 4 байтам), то:
;   * первое слово содержит опорное значение (по которому происходит сравнение),
;   * второе слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Процедура изменяет регистры AX, (E)BX, SI, DI, сохраняет CX, DX, BP и сегментные регистры
InsSort		PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		shl	cx,_SortElemWords
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура сортировки вставками
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX
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
		srt?jle	@@break			; прыгаем, если [I] <= T

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
InsSort		ENDP
