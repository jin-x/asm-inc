.MODEL Small

IFDEF		??Version			; TASM
  LOCALS
ENDIF

SHOWARRAYS	=	1			; ���������� �� ����� �������� (1- ��, 0 - ���)?
SPEEDTEST	=	1			; �������� �� �������� (1 - ��, 0 - ���)?
RANDOMIZE	=	1			; ���������������� ��������� ��������������� ����� (1 - ��������� �� ��������� �������� �������, 0 - ���������� ���������)?

SortElemWords	=	2			; ���-�� ���� (WORD) � ������ �������� �������

IF	SPEEDTEST
  .386
ELSE
  .286
ENDIF

.STACK 100h

.DATA

IF	SHOWARRAYS
  msgSrc	DB	'Source array:',13,10,'$'
ENDIF
msgSorting	DB	13,10,'Sorting...$'
msgDone		DB	' done!',13,10,'$'
IF	SHOWARRAYS
  msgRes	DB	13,10,'Sorted array:',13,10,'$'
ENDIF
msgChk		DB	13,10,'Checking...$'
msgChkRes	DB	' done!',13,10,'Fails count = '
msgCnt		DB	'     ',13,10,'$'
IF	SPEEDTEST
  msgSpeed	DB	13,10,'Sort procedure took $'
  msgTicks	DB	' ticks',13,10,'$'
ENDIF

.DATA?

NArray		=	2500			; ���-�� ��������� �������
Array		DW	(NArray*2) dup (?)	; ����������� ������ (������ ����� - ��������, �� ������� ���������� ���������; ������ ����� - ��������� �� ��������������� ������)
Strings		DB	(NArray*7) dup (?)	; ������ ����� (�� 7 �������� ������)

.CODE

Start:

                cld
		mov	ax,@data
		mov	ds,ax			; DS = ������� ������
		mov	es,ax			; ES = ������� ������

		; ������������� ��������
		lea	si,Array
		lea	di,Strings
		mov	cx,NArray
		call	InitArray

IF	SHOWARRAYS
		; ����� ��������� ������� �� �����
		lea	si,Array
		mov	cx,NArray
		lea	dx,msgSrc
		call	ShowArray
ENDIF
		; ���������� �������
		lea	dx,msgSorting
		call	ShowMsg

IF	SPEEDTEST
		cli
		db	0Fh,31h			; rdtsc
		push	eax
ENDIF
		; ������� ������: DS:DX = ����� �������, CX = ���-�� ��������� �������
		lea	dx,Array
		mov	cx,NArray
		call	InsSort
IF	SPEEDTEST
		db	0Fh,31h			; rdtsc
		sti
		push	eax
ENDIF
		lea	dx,msgDone
		call	ShowMsg

IF	SHOWARRAYS
		; ����� ���������������� ������� �� �����
		lea	si,Array
		mov	cx,NArray
		lea	dx,msgRes
		call	ShowArray
ENDIF
		; �������� �������
		lea	dx,msgChk
		call	ShowMsg

		lea	si,Array
		mov	cx,NArray-1
		xor	bx,bx			; BX = 0 - ���-�� ������
		lodsw				; ��������
	ChkNxt:	xchg	dx,ax
		lodsw				; ��������� ������
		lodsw				; ��������
		cmp	ax,dx
		jge	ChkOk
		inc	bx			; ����������� ������� ������
	ChkOk:	loop	ChkNxt

		mov	ax,bx
		lea	di,msgCnt
		call	SIntToStr
		lea	dx,msgChkRes
		call	ShowMsg

IF	SPEEDTEST
		lea	dx,msgSpeed
		call	ShowMsg
		pop	eax
		pop	edx
		sub	eax,edx
		call	ShowEUInt3
		lea	dx,msgTicks
		call	ShowMsg
ENDIF
		mov	ax,4C00h
		int	21h			; ����� �� ���������

; ����� ������ �� ������ DS:DX �� �����
ShowMsg		PROC
		mov	ah,9
		int	21h
		ret
ShowMsg		ENDP

; ������������� �������� (DS:SI - ������ �������, ES:DI - ����� ��� �����, CX - ���-�� ���������)
; ������ �������� AX, BX, CX, DX, SI, DI
InitArray	PROC
IF	RANDOMIZE
		push	cx
		mov	ah,2Ch
		int	21h			; �������� ������� �����
		xchg	ax,dx
		xchg	cl,ch
		add	ax,cx
		inc	ax			; AX = �������� ��������
		pop	cx
ELSE
		mov	ax,-1
ENDIF
	@@next:	call	Random			; ��������� ���������� �����
		mov	[si],ax			; ������ � ������ ���������� �����
		mov	[si+2],di		; ������ � ������ ������ ������
		add	si,4
		push	ax
		push	cx
		call	SIntToStr		; ����� ������ � ������ Strings
		mov	al,'$'
		stosb				; ���������� ������� '$' � ����� ������
		pop	cx
		pop	ax
		loop	@@next			; ��������� ���� ��������� �������
		ret
InitArray	ENDP

; ��������� ���������������� ����� ������� XorShift �� ������ �������� � �������� AX (�� ������ ���� = 0)
; ��������� � AX (������� �� ���������� 0)
; ������ ������� DX
Random		PROC
		mov	dx,ax
		shl	ax,13
		xor	ax,dx

		mov	dx,ax
		shr	ax,9
		xor	ax,dx

		mov	dx,ax
		shl	ax,7
		xor	ax,dx
		ret
Random		ENDP

IF	SHOWARRAYS
; ����� ������ ����������� (DS:DX) � ������ �����, �� ������� ��������� ������ ����� ������� (DS:SI - ������ �������, CX - ���-�� ���������)
; ������ �������� AX, DX, SI
ShowArray	PROC
		mov	ah,9
		int	21h			; ������� ������ �����������
	@@next:	lodsw
		lodsw
		mov	dx,ax
		mov	ah,9
		int	21h			; ������� ������
		mov	ah,2
		mov	dl,13
		int	21h			; ������� CR
		mov	dl,10
		int	21h			; ������� LF
		loop	@@next
		ret
ShowArray	ENDP
ENDIF

; ����� ������ ��������� ����������� ����� AX � ����� �� ������ ES:DI (DS=ES)
; ������ �������� AX, BX, CX, DX, DI (DI ��������� �� ������)
SIntToStr	PROC
		test	ax,ax			; ��������� �����
		jns	@@nosign		; ���������, ���� ����� �������������
		mov	byte ptr [di],'-'	; ������� ������ '-'
		inc	di
		neg	ax			; � ������ ����
	@@nosign:
		xor	cx,cx			; ���-�� ���� (���� 0)
		mov	bx,10			; ������� ��������� (����������)
	@@next:
		xor	dx,dx			; DX = 0 (����� ��� �������)
		div	bx			; AX = DX:AX/�������_��������� (10), DX = �������
		push	dx			; ��������� ����� � �����
		inc	cx			; ����������� ���-�� ����
		test	ax,ax			; ��������� AX (�����)
		jnz	@@next			; ���������, ���� ������� ��� �� ����� 0
	@@outdigit:
		pop	ax			; ��������� ����� �� �����
		add	al,'0'			; ����������� � � ������
		stosb				; ������� �����
		loop	@@outdigit		; ��������� ���� ������ ��������
		ret
SIntToStr	ENDP

; ����� ������ ������������ ����������� ����� EAX �� ����� � ������������ �� 3 �����
; ������ �������� EAX, EBX, CX, EDX
IF	SPEEDTEST
ShowEUInt3	PROC
		xor	cx,cx			; CL = ���-�� �������� (���� 0), CH = ���-�� ���� � ������� ������
		mov	ebx,10			; ������� ���������
	@@next:
		cmp	ch,3
		jb	@@not3
		push	"'"
		inc	cx
		xor	ch,ch
	@@not3:
		xor	edx,edx			; EDX = 0 (����� ��� �������)
		div	ebx			; EAX = EDX:EAX/�������_��������� (10), (E)DX = �������
		add	dl,'0'			; ����������� ��� � ������
		push	dx			; ��������� ����� � �����
		add	cx,101h			; ����������� ���-�� �������� � ���-�� ���� � ������� ������
		test	eax,eax
		jnz	@@next			; �������, ���� ������� ��� �� ����� 0

		mov	ah,2
	@@outdigit:
		pop	dx			; ��������� ������ �� �����
		int	21h			; ������� ��� �� �����
	@@nospace:
		dec	cl
		jnz	@@outdigit		; ��������� ���� ������ ��������
		ret
ShowEUInt3	ENDP
ENDIF

INCLUDE		ISort.inc

END		Start
