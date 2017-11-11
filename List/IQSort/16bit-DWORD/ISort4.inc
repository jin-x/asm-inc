;##################################################
;##                                              ##
;##         [ Asm7x.List ][ ISort4.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       УНИВЕРСАЛЬНЫЙ МОДУЛЬ ДЛЯ 16 БИТ        ##
;##                                              ##
;##             -= Insertion Sort =-             ##
;##        (for extended elenemt arrays)         ##
;##                                              ##
;##             Сортировка вставками             ##
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

ISort4_ver	=	100h			; версия данного модуля (word: старший байт - целая часть, младший - дробная)

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
    srt?jle	EQU	<jle>
  ELSE
    srt?jle	EQU	<jge>
  ENDIF
ELSE
  IF	_Sort4Ascending
    srt?jle	EQU	<jbe>
  ELSE
    srt?jle	EQU	<jae>
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

;-- InsSort4: СОРТИРОВКА массива ВСТАВКАМИ -----------------------------------------------------------------------------
; > Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива (знаковое значение)
; > Результат: отсортированный массив (по тому же адресу)
; Если элементы массива содержат по 2 значения, т.е. Sort4ElemDWords = 2 (размер элемента массива = _Sort4ElemSize = 8 байтам), то:
;   * первое двойное слово содержит опорное значение (по которому происходит сравнение),
;   * второе двойное слово - связанные с элементом данные (обычно это указатель на данные);
;     при сортировке связанные данные переносятся вместе с опорными значениями
; Процедура изменяет регистры EAX, EBX, SI, DI, старшее слово EBP; сохраняет CX, DX, BP и сегментные регистры
InsSort4	PROC
		dec	cx
		jle	@@exit			; выходим, если кол-во элементов <= 1

		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = адрес последнего элемента

		; Главная процедура сортировки вставками
		; DX = адрес первого элемента, CX = адрес последнего элемента, CX > DX
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
		srt?jle	@@break			; прыгаем, если [I] <= T

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
InsSort4	ENDP
