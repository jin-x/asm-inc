;##################################################
;##                                              ##
;##        [ Asm7x.List ][ IQSort4.inc ]         ##
;##                                              ##
;##          UNIVERSAL UNIT FOR 16 BIT           ##
;##       ������������� ������ ��� 16 ���        ##
;##                                              ##
;##            -= Smart Quick Sort =-            ##
;##        (for extended elenemt arrays)         ##
;##                                              ##
;##           ����� ������ ���஢��           ##
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

IQSort4_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

; ����� ���������� ������� ����� � ������ *�����* ���������� ��������� �������:

; inclIQSort4 = 1 - ������� ��楤��� IQSort4 (㬭�� ������ ���஢��) � ���, 0 - �� ������� [�� 㬮�砭�� 1]

; inclInsSort4 = 1 - ������� ��楤��� InsSort4 (���஢�� ��⠢����) � ���, 0 - �� ������� [�� 㬮�砭�� 0]
;   �᫨ ��楤�� InsSort4 �ᯮ������ ��楤�ன IQSort4, � ��� ����砥��� ���� ���� �⮩ ��楤���, ����室���� ��� ࠡ��� IQSort4

; Sort4SignCmp = 1 - �������� �ࠢ�����, 0 - ����������� [�� 㬮�砭�� 1]

; Sort4Ascending = 1 - ���஢�� �� �����⠭�� (�� ����襣� � ����襬�), 0 - �� �뢠��� (�� ���쥣� � ����襬�)

; Sort4ElemDWords = ���-�� ������� ᫮� � ����묨 (����᪠���� ���祭��: 1 - ⮫쪮 ���୮� ᫮��, 2 - ���୮� ᫮�� � �易��� �����; ��㣨� ���祭�� ��த�� �訡��)
;   [*���� ������ ���������� ���������� �����������*, ���祭�� �� 㬮�砭�� ��� ���� ���!!!]

; IQSort4InsThrs = ��ண �ᯮ�짮����� ���஢�� ��⠢���� (����� ���-�� ������⮢ ����� 㪠������� ����� ���祭��) [0 = �⪫�祭�; ������ 2; �� 㬮�砭�� 16]
;   �᫨ ���-�� ������⮢ (�� ��ࢮ� ��� ��᫥���饩 ���樨) ����� ��� ࠢ�� ���祭�� IQSort4InsThrs, �ᯮ������ ������ ���஢��, ����
;   �ᯮ������ ���஢�� ��⠢���� - �� ������� �᪮��� ����� (�� �ࠢ��쭮� �롮� ���祭�� IQSort4InsThrs, ���ਬ��, �� ���祭�� �� 㬮�砭��)
;   � 㬥��蠥� ��㡨�� ४��ᨨ, ����� �⥪ �� ��९�������

; IQSort4MaxStk = ���ᨬ���� ࠧ��� �⥪� � �����, ����� �����⨬� �ᯮ�짮���� ��楤�� IQSort4 (������ call �� �᭮����� ����) [0 = �⪫�祭�; ������ 12; �� 㬮�砭�� 128]
;   ����� �஢��� ���������� �ᯮ���� 4 ���� (2 ᫮��) �⥪� (���� �஢��� - �� 4-� ᫮�, ��᫥���� ����� �ᯮ�짮���� �� 1-2 ᫮�� �����, �⮣� ������ 10-12 ����),
;   �.�. 128 ���� ��������� �࣠�������� �� 30 �஢��� ४��ᨨ, �� �����筮 ���� ��� �祭� ������ ���ᨢ��, �ᮡ���� ����� IQSort4InsThrs > 0

; �� ��⠭���� ���祭�� IQSort4InsThrs = IQSort4MaxStk = 0 ��楤�� IQSort4 �ॢ�頥��� � ��楤��� �⠭���⭮� ����ன ���஢�� (�.�. ���������஢�����)

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
_Sort4ElemSize	=	_Sort4ElemDWords*4	; ࠧ��� ������ ������� ���ᨢ� (4 ��� 8 ����)

_defdef	IQSort4InsThrs, 16
_defdef	IQSort4MaxStk, 128

IF	_IQSort4InsThrs and _IQSort4InsThrs lt 2
  _IQSort4InsThrs =	0			; �᫨ IQSort4InsThrs < 2, �ਭ����� ���祭�� 0
ENDIF

IF	_IQSort4MaxStk and _IQSort4MaxStk lt 10+(_Sort4ElemDWords-1)*2
  _IQSort4MaxStk =	0			; �᫨ IQSort4MaxStk < 10 + 2 (�᫨ Sort4ElemDWords = 2), �ਭ����� ���祭�� 0
ENDIF

;-- IQSort4: ����� ������� ���������� ���ᨢ� (�������஢���� ��⮤��) ------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. Sort4ElemDWords = 2 (ࠧ��� ������� ���ᨢ� = _Sort4ElemSize = 8 ���⠬), �:
;   * ��ࢮ� ������� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ������� ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; �᫨ ���-�� ������⮢ (�� ��ࢮ� ��� ��᫥���饩 ���樨) ����� ��� ࠢ�� ���祭�� IQSort4InsThrs, �ᯮ������ ������ ���஢��,
;   ���� �ᯮ������ ���஢�� ��⠢���� [⮫쪮 �᫨ IQSort4InsThrs <> 0]
; ����஢�� ��⠢���� ⠪�� �ᯮ������, �᫨ ��� ᫥���饣� �஢�� ४��ᨨ ���ॡ���� ����� IQSort4MaxStk ���� �⥪� � ��饬 ᫮�����
;   [⮫쪮 �᫨ IQSort4MaxStk <> 0]
; ��楤�� ������� ॣ����� EAX, EBX, CX, DX, SI, DI, ���襥 ᫮�� EBP; ��࠭�� BP � ᥣ����� ॣ�����
IF	_inclIQSort4
IQSort4		PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		push	bp
		xor	bp,bp			; BP = ���-�� ४��ᨩ
		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ����ன ���஢��
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX, BP = �஢��� ४��ᨨ
	@@IQSort4Main:
IF	_IQSort4InsThrs
		mov	ax,cx
		sub	ax,dx
		shr	ax,_Sort4ElemDWords+1	; AX = ���-�� ������⮢ ����� 1
		cmp	ax,_IQSort4InsThrs-1
		jb	@@callins		; �᫨ ���-�� ������⮢ ����� ��ண����� ���祭��, �ᯮ��㥬 ���஢�� ��⠢����
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
		push	si			; ��࠭塞 R � I
		mov	cx,di
		; DX = L, CX = J
IF	_IQSort4MaxStk
		cmp	bp,(_IQSort4MaxStk-(10+(_Sort4ElemDWords-1)*2))/4	; 6 ᫮� - �� ���� ������ � ��뢠���� �ணࠬ�� +
						; bp + cx + si + ���� ������ �� InsSort4 + bp (�᫨ Sort4ElemDWords = 2)
		jae	@@callins2		; �᫨ �᫮ ४��ᨩ ���⨣�� ���ᨬ㬠, ��� �� �맮� ���஢�� ��⠢����: InsSort4(L, J)
ENDIF
		inc	bp			; ���� 㢥��稢��� ��㡨�� ४��ᨨ � ��� �� ४����
		jmp	@@IQSort4Main		;;   IQSort4(L, J); �맮� ������ �१ jmp ��� �������� �⥪� :)
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
IF	_IQSort4InsThrs
	@@callins:
		push	offset @@finish		; ���� ������
		jmp	@IQInsSort4		; ����� call + jmp @@finish ������ push + jmp
ENDIF
IF	_IQSort4MaxStk
	@@callins2:
		push	offset @@recursret	; ���� ������
		jmp	@IQInsSort4		; ����� call + jmp @@recursret ������ push + jmp
ENDIF
IQSort4		ENDP
ENDIF ; _inclIQSort4

;-- InsSort4: ���������� ���ᨢ� ��������� -----------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �᫨ �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭��, �.�. Sort4ElemDWords = 2 (ࠧ��� ������� ���ᨢ� = _Sort4ElemSize = 8 ���⠬), �:
;   * ��ࢮ� ������� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ������� ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; ��楤�� ������� ॣ����� EAX, EBX, SI, DI, ���襥 ᫮�� EBP; ��࠭�� CX, DX, BP � ᥣ����� ॣ�����
IF	_inclInsSort4
InsSort4	PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		shl	cx,_Sort4ElemDWords+1
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ���஢�� ��⠢����
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX
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
%		srt?jl&e @@break		; ��룠��, �᫨ [I] <= T

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
ENDIF ; _inclInsSort4 or _IQSort4InsThrs or _IQSort4MaxStk
IF	_inclInsSort4
InsSort4	ENDP
ELSEIF	_IQSort4InsThrs or _IQSort4MaxStk
@IQInsSort4	ENDP
ENDIF
