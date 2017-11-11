;##################################################
;##                                              ##
;##        [ Asm7x.List ][ ISortS2.inc ]         ##
;##                                              ##
;##             -= Insertion Sort =-             ##
;##         (for double element arrays)          ##
;##                                              ##
;##             ����஢�� ��⠢����             ##
;##     (��� ���ᨢ�� � �����묨 ������⠬�)     ##
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

ISortS2_ver	=	100h			; ����� ������� ����� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

; !!! � ������ ���㫥 �ᯮ������ �������� �ࠢ����� ���祭�� � ���஢�� �� ����������� (�� ����襣� � ����襬�)
; !!! ��� ������ �� ����������� �ࠢ����� ��� ���஢�� �� �������� ����室��� �������� ᮮ⢥�����騥 ������樨, ����祭�� � ���������� ᨬ������ ***

;-----------------------------------------------------------------------------------------------------------------------

;-- InsSortDE: ���������� ���ᨢ� ��������� ----------------------------------------------------------------------------
; > �室�� �����: DS:DX = ���� ���ᨢ�, CX = ���-�� ������⮢ ���ᨢ� (�������� ���祭��)
; > �������: �����஢���� ���ᨢ (�� ⮬� �� �����)
; �������� ���ᨢ� ᮤ�ঠ� �� 2 ���祭�� ⨯� WORD:
;   * ��ࢮ� ᫮�� ᮤ�ন� ���୮� ���祭�� (�� ���஬� �ந�室�� �ࠢ�����),
;   * ��஥ ᫮�� - �易��� � ������⮬ ����� (���筮 �� 㪠��⥫� �� �����);
;     �� ���஢�� �易��� ����� ��७������ ����� � ����묨 ���祭�ﬨ
; ��楤�� ������� ॣ����� AX, BX, SI, DI, ��࠭�� CX, DX, BP � ᥣ����� ॣ�����
InsSortDE	PROC
		dec	cx
		jle	@@exit			; ��室��, �᫨ ���-�� ������⮢ <= 1

		shl	cx,2
		add	cx,dx			; CX = ���� ��᫥����� �������

		; ������� ��楤�� ���஢�� ��⠢����
		; DX = ���� ��ࢮ�� �������, CX = ���� ��᫥����� �������, CX > DX
		push	bp
		mov	di,dx			; J (DI) := L - ���� ��ࢮ�� �������
	@@next:					;; for J (DI) := L+1 (CX) to R (DX) do
		add	di,4			; J++ (DI) - ���� ᫥���饣� �஢��塞��� ������� (� �᭮���� 横��)
		mov	bx,[di]			;; T (BP:BX) := [J]
		mov	bp,[di+2]
		mov	si,di			; I+1 (SI) = DI - ���� �������, ᫥���饣� �� �ࠢ������� (�� ����७��� 横��)
	@@loop:					;; repeat
		mov	ax,[si-4]
		cmp	ax,bx			;; if [I] > T then
		jle	@@break			; ��룠��, �᫨ [I] <= T {*** ���஢�� �� �����⠭��: jle - �������� �ࠢ�����, jbe - �����������; �� �뢠���: jge - ��������, jae - �����������}

		mov	[si],ax			;;  [I+1] := [I] else Break
		mov	ax,[si-4+2]
		mov	[si+2],ax

		sub	si,4			;; Dec(I)
		cmp	si,dx
		jnbe	@@loop			;; until I < L (I+1 <= L)
	@@break:
		mov	[si],bx			;; [I+1] := T
		mov	[si+2],bp

		cmp	di,cx
		jnae	@@next			; ᫥���騩 ������� ���ᨢ� ;; end for

		pop	bp
	@@exit:	ret
InsSortDE	ENDP