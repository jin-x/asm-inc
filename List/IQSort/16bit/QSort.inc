;##################################################
;##                                              ##
;##         [ Asm7x.List ][ QSort.inc ]          ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       ������������� ������ ��� 16 ���        ##
;##                                              ##
;##               -= Quick Sort =-               ##
;##              ������ ���஢��              ##
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

QSort_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

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
    srt?jl	EQU	<jl>
    srt?jg	EQU	<jg>
  ELSE
    srt?jl	EQU	<jg>
    srt?jg	EQU	<jl>
  ENDIF
ELSE
  IF	_SortAscending
    srt?jl	EQU	<jb>
    srt?jg	EQU	<ja>
  ELSE
    srt?jl	EQU	<ja>
    srt?jg	EQU	<jb>
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

;-- QSort: ������� ���������� ���ᨢ� ----------------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. SortElemWords = 2 (ࠧ��� ������� ���ᨢ� = _SortElemSize = 4 ���⠬), �:
;   * ��ࢮ� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; ��楤�� ������� ॣ����� AX, (E)BX, CX, DX, SI, DI, ��࠭�� BP � ᥣ����� ॣ�����
QSort		PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		push	bp
		xor	bp,bp			; BP = ���-�� ४��ᨩ
		shl	cx,_SortElemWords
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ����ன ���஢��
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX, BP = �஢��� ४��ᨨ
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
		push	si			; ��࠭塞 R � I
		mov	cx,di
		; DX = L, CX = J
		inc	bp			; 㢥��稢��� ��㡨�� ४��ᨨ � ��� �� ४����
		jmp	@@QSortMain		;;   QSort(L, J); �맮� ������ �१ jmp ��� �������� �⥪� :)
	@@recursret:
		pop	si
		pop	cx			; ����⠭�������� I � R
	@@norecurs:
		mov	dx,si			;; L := I
		cmp	si,cx
		jnae	@@repeat1		;; until I >= R
	@@finish:
		dec	bp			; 㬥��蠥� ��㡨�� ४��ᨨ
		jns	@@recursret		; ��룠��, �᫨ �� �� ���� (��୥���) �஢��� ४��ᨨ
		pop	bp
	@@exit:	ret
QSort		ENDP
