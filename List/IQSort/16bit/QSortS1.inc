;##################################################
;##                                              ##
;##        [ Asm7x.List ][ QSortS1.inc ]         ##
;##                                              ##
;##               -= Quick Sort =-               ##
;##         (for single element arrays)          ##
;##                                              ##
;##              ������ ���஢��              ##
;##    (��� ���ᨢ�� � ������묨 ������⠬�)    ##
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

QSortS1_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

; !!! � ������ ���㫥 �ᯮ������ �������� �ࠢ����� ���祭�� � ���஢�� �� ����������� (�� ����襣� � ����襬�)
; !!! ��� ������ �� ����������� �ࠢ����� ��� ���஢�� �� �������� ����室��� �������� ᮮ⢥�����騥 ������樨, ����祭�� � ���������� ᨬ������ ***

;-----------------------------------------------------------------------------------------------------------------------

;-- QSort: ������� ���������� ���ᨢ� ----------------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �������� ���ᨢ� ᮤ�ঠ� �� 1 ���祭�� ⨯� WORD, �� ���஬� �ந�室�� �ࠢ����� � ���஢��
; ��楤�� ������� ॣ����� AX, BX, CX, DX, SI, DI, ��࠭�� BP � ᥣ����� ॣ�����
QSort		PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		push	bp
		xor	bp,bp			; BP = ���-�� ४��ᨩ
		shl	cx,1
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
		and	bx,-2
		add	bx,dx			;; P (BX) := (L + R) / 2
		mov	ax,[bx]			;; T (AX) := [P]
	@@repeat2 = @@cmpI			;; repeat
		; SI = I, AX = T, DI = J, DX = L, CX = R
		jmp	@@cmpI
	@@addI:	add	si,2			;; Inc(I)
	@@cmpI:	cmp	[si],ax			;; while [I] < T
		jl	@@addI			; {*** ���஢�� �� �����⠭��: jl - �������� �ࠢ�����, jb - �����������; �� �뢠���: jg - ��������, ja - �����������}

		jmp	@@cmpJ
	@@subJ:	sub	di,2			;; Dec(J)
	@@cmpJ:	cmp	[di],ax			;; while [J] > T
		jg	@@subJ			; {*** ���஢�� �� �����⠭��: jg - �������� �ࠢ�����, ja - �����������; �� �뢠���: jl - ��������, jb - �����������}

		cmp	si,di
		jnbe	@@noswap		;; if I <= J then

		mov	bx,[si]			;;   Swap [I],[J]
		xchg	[di],bx
		mov	[si],bx

		add	si,2			;; Inc(I)
		sub	di,2			;; Dec(J)
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
