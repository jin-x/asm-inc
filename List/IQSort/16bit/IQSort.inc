;##################################################
;##                                              ##
;##         [ Asm7x.List ][ IQSort.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       ������������� ������ ��� 16 ���        ##
;##                                              ##
;##            -= Smart Quick Sort =-            ##
;##           ����� ������ ���஢��           ##
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

IQSort_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

; ����� ���������� ������� ����� � ������ *�����* ���������� ��������� �������:

; inclIQSort = 1 - ������� ��楤��� IQSort (㬭�� ������ ���஢��) � ���, 0 - �� ������� [�� 㬮�砭�� 1]

; inclInsSort = 1 - ������� ��楤��� InsSort (���஢�� ��⠢����) � ���, 0 - �� ������� [�� 㬮�砭�� 0]
;   �᫨ ��楤�� InsSort �ᯮ������ ��楤�ன IQSort, � ��� ����砥��� ���� ���� �⮩ ��楤���, ����室���� ��� ࠡ��� IQSort

; SortSignCmp = 1 - �������� �ࠢ�����, 0 - ����������� [�� 㬮�砭�� 1]

; SortAscending = 1 - ���஢�� �� �����⠭�� (�� ����襣� � ����襬�), 0 - �� �뢠��� (�� ���쥣� � ����襬�)

; SortElemWords = ���-�� ᫮� � ����묨 (����᪠���� ���祭��: 1 - ⮫쪮 ���୮� ᫮��, 2 - ���୮� ᫮�� � �易��� �����; ��㣨� ���祭�� ��த�� �訡��)
;   [*���� ������ ���������� ���������� �����������*, ���祭�� �� 㬮�砭�� ��� ���� ���!!!]

; IQSortInsThrs = ��ண �ᯮ�짮����� ���஢�� ��⠢���� (����� ���-�� ������⮢ ����� 㪠������� ����� ���祭��) [0 = �⪫�祭�; ������ 2; �� 㬮�砭�� 16]
;   �᫨ ���-�� ������⮢ (�� ��ࢮ� ��� ��᫥���饩 ���樨) ����� ��� ࠢ�� ���祭�� IQSortInsThrs, �ᯮ������ ������ ���஢��, ����
;   �ᯮ������ ���஢�� ��⠢���� - �� ������� �᪮��� ����� (�� �ࠢ��쭮� �롮� ���祭�� IQSortInsThrs, ���ਬ��, �� ���祭�� �� 㬮�砭��)
;   � 㬥��蠥� ��㡨�� ४��ᨨ, ����� �⥪ �� ��९�������

; IQSortMaxStk = ���ᨬ���� ࠧ��� �⥪� � �����, ����� �����⨬� �ᯮ�짮���� ��楤�� IQSort (������ call �� �᭮����� ����) [0 = �⪫�祭�; ������ 12; �� 㬮�砭�� 128]
;   ����� �஢��� ���������� �ᯮ���� 4 ���� (2 ᫮��) �⥪� (���� �஢��� - �� 4-� ᫮�, ��᫥���� ����� �ᯮ�짮���� �� 1-2 ᫮�� �����, �⮣� ������ 10-12 ����),
;   �.�. 128 ���� ��������� �࣠�������� �� 30 �஢��� ४��ᨨ, �� �����筮 ���� ��� �祭� ������ ���ᨢ��, �ᮡ���� ����� IQSortInsThrs > 0

; �� ��⠭���� ���祭�� IQSortInsThrs = IQSortMaxStk = 0 ��楤�� IQSort �ॢ�頥��� � ��楤��� �⠭���⭮� ����ன ���஢�� (�.�. ���������஢�����)

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

_defdef	IQSortInsThrs, 16
_defdef	IQSortMaxStk, 128

IF	_IQSortInsThrs and _IQSortInsThrs lt 2
  _IQSortInsThrs =	0			; �᫨ IQSortInsThrs < 2, �ਭ����� ���祭�� 0
ENDIF

IF	_IQSortMaxStk and _IQSortMaxStk lt 10-(_SortElemWords gt 1 and not @386)*2
  _IQSortMaxStk	=	0			; �᫨ IQSortMaxStk < 10 + 2 (�᫨ SortElemWords = 2 ��� ������樨 386 �⪫�祭�), �ਭ����� ���祭�� 0
ENDIF

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

;-- IQSort: ����� ������� ���������� ���ᨢ� (�������஢���� ��⮤��) -------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. SortElemWords = 2 (ࠧ��� ������� ���ᨢ� = _SortElemSize = 4 ���⠬), �:
;   * ��ࢮ� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; �᫨ ���-�� ������⮢ (�� ��ࢮ� ��� ��᫥���饩 ���樨) ����� ��� ࠢ�� ���祭�� IQSortInsThrs, �ᯮ������ ������ ���஢��,
;   ���� �ᯮ������ ���஢�� ��⠢���� [⮫쪮 �᫨ IQSortInsThrs <> 0]
; ����஢�� ��⠢���� ⠪�� �ᯮ������, �᫨ ��� ᫥���饣� �஢�� ४��ᨨ ���ॡ���� ����� IQSortMaxStk ���� �⥪� � ��饬 ᫮�����
;   [⮫쪮 �᫨ IQSortMaxStk <> 0]
; ��楤�� ������� ॣ����� AX, (E)BX, CX, DX, SI, DI, ��࠭�� BP � ᥣ����� ॣ�����
IF	_inclIQSort
IQSort		PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		push	bp
		xor	bp,bp			; BP = ���-�� ४��ᨩ
		shl	cx,_SortElemWords
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ����ன ���஢��
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX, BP = �஢��� ४��ᨨ
	@@IQSortMain:
IF	_IQSortInsThrs
		mov	ax,cx
		sub	ax,dx
		shr	ax,_SortElemWords	; AX = ���-�� ������⮢ ����� 1
		cmp	ax,_IQSortInsThrs-1
		jb	@@callins		; �᫨ ���-�� ������⮢ ����� ��ண����� ���祭��, �ᯮ��㥬 ���஢�� ��⠢����
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
		push	si			; ��࠭塞 R � I
		mov	cx,di
		; DX = L, CX = J
IF	_IQSortMaxStk
		cmp	bp,(_IQSortMaxStk-(10-(_SortElemWords gt 1 and not @386)*2))/4	; 6 ᫮� - �� ���� ������ � ��뢠���� �ணࠬ�� +
						; bp + cx + si + ���� ������ �� InsSort + bp (�᫨ SortElemWords = 2 ��� ������樨 386 �⪫�祭�)
		jae	@@callins2		; �᫨ �᫮ ४��ᨩ ���⨣�� ���ᨬ㬠, ��� �� �맮� ���஢�� ��⠢����: InsSort(L, J)
ENDIF
		inc	bp			; ���� 㢥��稢��� ��㡨�� ४��ᨨ � ��� �� ४����
		jmp	@@IQSortMain		;;   IQSort(L, J); �맮� ������ �१ jmp ��� �������� �⥪� :)
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
IF	_IQSortInsThrs
	@@callins:
		push	offset @@finish		; ���� ������
		jmp	@IQInsSort		; ����� call + jmp @@finish ������ push + jmp
ENDIF
IF	_IQSortMaxStk
	@@callins2:
		push	offset @@recursret	; ���� ������
		jmp	@IQInsSort		; ����� call + jmp @@recursret ������ push + jmp
ENDIF
IQSort		ENDP
ENDIF ; _inclIQSort

;-- InsSort: ���������� ���ᨢ� ��������� ------------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. SortElemWords = 2 (ࠧ��� ������� ���ᨢ� = _SortElemSize = 4 ���⠬), �:
;   * ��ࢮ� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; ��楤�� ������� ॣ����� AX, (E)BX, SI, DI, ��࠭�� CX, DX, BP � ᥣ����� ॣ�����
IF	_inclInsSort
InsSort		PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		shl	cx,_SortElemWords
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ���஢�� ��⠢����
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX
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
%		srt?jl&e @@break		; ��룠��, �᫨ [I] <= T

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
ENDIF ; _inclInsSort or _IQSortInsThrs or _IQSortMaxStk
IF	_inclInsSort
InsSort		ENDP
ELSEIF	_IQSortInsThrs or _IQSortMaxStk
@IQInsSort	ENDP
ENDIF
