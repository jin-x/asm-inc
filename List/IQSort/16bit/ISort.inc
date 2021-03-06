;##################################################
;##                                              ##
;##         [ Asm7x.List ][ ISort.inc ]          ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       ������������� ������ ��� 16 ���        ##
;##                                              ##
;##             -= Insertion Sort =-             ##
;##             ����஢�� ��⠢����             ##
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

ISort_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

; ����� ���������� ������� ����� � ������ *�����* ���������� ��������� �������:

; SortSignCmp = 1 - �������� �ࠢ�����, 0 - ����������� [�� 㬮�砭�� 1]

; SortAscending = 1 - ���஢�� �� �����⠭�� (�� ����襣� � ����襬�), 0 - �� �뢠��� (�� ���쥣� � ����襬�)

; SortElemWords = ���-�� ᫮� � ����묨 (����᪠���� ���祭��: 1 - ⮫쪮 ���୮� ᫮��, 2 - ���୮� ᫮�� � �易��� �����; ��㣨� ���祭�� ��த�� �訡��)
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

_defdef	SortSignCmp, 1
_defdef	SortAscending, 1

IF	_SortSignCmp
  IF	_SortAscending
    srt?jle	EQU	<jle>
  ELSE
    srt?jle	EQU	<jge>
  ENDIF
ELSE
  IF	_SortAscending
    srt?jle	EQU	<jbe>
  ELSE
    srt?jle	EQU	<jae>
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
_SortElemSize	=	_SortElemWords*2	; ࠧ��� ������ ������� ���ᨢ� (2 ��� 4 ����)

IF	_SortElemWords gt 1 and @386
  srt?ax	EQU	<eax>
  srt?bx	EQU	<ebx>
ELSE
  srt?ax	EQU	<ax>
  srt?bx	EQU	<bx>
ENDIF

IF	_SortElemWords gt 1 and @386
  srt?xchgreg	EQU	<ebx>
ELSE
  srt?xchgreg	EQU	<bx>
ENDIF

;-- InsSort: ���������� ���ᨢ� ��������� ------------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. SortElemWords = 2 (ࠧ��� ������� ���ᨢ� = _SortElemSize = 4 ���⠬), �:
;   * ��ࢮ� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; ��楤�� ������� ॣ����� AX, (E)BX, SI, DI, ��࠭�� CX, DX, BP � ᥣ����� ॣ�����
InsSort		PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		shl	cx,_SortElemWords
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ���஢�� ��⠢����
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX
IF	_SortElemWords gt 1 and not @386
		push	bp
ENDIF
		mov	di,dx			; J (DI) := L - ���� ��ࢮ�� �������
	@@next:					;; for J (DI) := L+1 (CX) to R (DX) do
		add	di,_SortElemSize	; J++ (DI) - ���� ᫥���饣� �஢��塞��� ������� (� �᭮���� 横��)
		mov	srt?bx,[di]		;; T (BP:BX|EBX) := [J]
IF	_SortElemWords gt 1 and not @386
		mov	bp,[di+2]
ENDIF
		mov	si,di			; I+1 (SI) := DI - ���� �������, ᫥���饣� �� �ࠢ������� (�� ����७��� 横��)
	@@loop:					;; repeat
		mov	srt?ax,[si-_SortElemSize]
		cmp	ax,bx			;; if [I] > T then
		srt?jle	@@break			; ��룠��, �᫨ [I] <= T

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
		jnae	@@next			; ᫥���騩 ������� ���ᨢ� ;; end for
IF	_SortElemWords gt 1 and not @386
		pop	bp
ENDIF
	@@exit:	ret
InsSort		ENDP
