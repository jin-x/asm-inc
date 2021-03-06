;##################################################
;##                                              ##
;##         [ Asm7x.List ][ ISort4.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       ������������� ������ ��� 16 ���        ##
;##                                              ##
;##             -= Insertion Sort =-             ##
;##        (for extended elenemt arrays)         ##
;##                                              ##
;##             ����஢�� ��⠢����             ##
;##   (��� ���ᨢ�� � ���७�묨 ������⠬�)   ##
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

ISort4_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

; ����� ���������� ������� ����� � ������ *�����* ���������� ��������� �������:

; Sort4SignCmp = 1 - �������� �ࠢ�����, 0 - ����������� [�� 㬮�砭�� 1]

; Sort4Ascending = 1 - ���஢�� �� �����⠭�� (�� ����襣� � ����襬�), 0 - �� �뢠��� (�� ���쥣� � ����襬�)

; Sort4ElemDWords = ���-�� ������� ᫮� � ����묨 (����᪠���� ���祭��: 1 - ⮫쪮 ���୮� ᫮��, 2 - ���୮� ᫮�� � �易��� �����; ��㣨� ���祭�� ��த�� �訡��)
;   [*���� ������ ���������� ���������� �����������*, ���祭�� �� 㬮�砭�� ��� ���� ���!!!]

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
_Sort4ElemSize	=	_Sort4ElemDWords*4	; ࠧ��� ������ ������� ���ᨢ� (4 ��� 8 ����)

;-- InsSort4: ���������� ���ᨢ� ��������� -----------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. Sort4ElemDWords = 2 (ࠧ��� ������� ���ᨢ� = _Sort4ElemSize = 8 ���⠬), �:
;   * ��ࢮ� ������� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ������� ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; ��楤�� ������� ॣ����� EAX, EBX, SI, DI, ���襥 ᫮�� EBP; ��࠭�� CX, DX, BP � ᥣ����� ॣ�����
InsSort4	PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ���஢�� ��⠢����
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX
IF	_Sort4ElemDWords gt 1
		push	bp
ENDIF
		mov	di,dx			; J (DI) := L - ���� ��ࢮ�� �������
	@@next:					;; for J (DI) := L+1 (CX) to R (DX) do
		add	di,_Sort4ElemSize	; J++ (DI) - ���� ᫥���饣� �஢��塞��� ������� (� �᭮���� 横��)
		mov	ebx,[di]		;; T (EBP:EBX|EBX) := [J]
IF	_Sort4ElemDWords gt 1
		mov	ebp,[di+4]
ENDIF
		mov	si,di			; I+1 (SI) := DI - ���� �������, ᫥���饣� �� �ࠢ������� (�� ����७��� 横��)
	@@loop:					;; repeat
		mov	eax,[si-_Sort4ElemSize]
		cmp	eax,ebx			;; if [I] > T then
		srt?jle	@@break			; ��룠��, �᫨ [I] <= T

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
		jnae	@@next			; ᫥���騩 ������� ���ᨢ� ;; end for
IF	_Sort4ElemDWords gt 1
		pop	bp
ENDIF
	@@exit:	ret
InsSort4	ENDP
