.MODEL Small
.386

IFDEF		??Version			; TASM
  LOCALS
ENDIF

SHOWARRAYS	=	1			; ���������� �� ����� �������� (1- ��, 0 - ���)?
SPEEDTEST	=	1			; �������� �� �������� (1 - ��, 0 - ���)?
RANDOMIZE	=	1			; ���������������� ��������� ��������������� ����� (1 - ��������� �� ��������� �������� �������, 0 - ���������� ���������)?

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
msgChkRes	DB	' done!',13,10,'Fails count = $'
IF	SPEEDTEST
  msgSpeed	DB	13,10,'Sort procedure took $'
  msgTicks	DB	' ticks',13,10,'$'
ENDIF

.DATA?

NArray		=	2000			; ���-�� ��������� �������
Array		DW	NArray dup (?)		; ����������� ������

.CODE

Start:

                cld
		mov	ax,@data
		mov	ds,ax			; DS = ������� ������
		mov	es,ax			; ES = ������� ������

		; ������������� ��������
		lea	di,Array
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
		call	InsSort4
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
		lodsd
	ChkNxt:	xchg	edx,eax
		lodsd
		cmp	eax,edx
		jge	ChkOk
		inc	bx			; ����������� ������� ������
	ChkOk:	loop	ChkNxt

		lea	dx,msgChkRes
		call	ShowMsg
		movzx	eax,bx
		call	ShowSEInt
		call	NewLine

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

; ������� ������
NewLine		PROC
		mov	ah,2
		mov	dl,13
		int	21h			; ������� CR
		mov	dl,10
		int	21h			; ������� LF
		ret
NewLine		ENDP

; ������������� �������� (ES:DI - ����� ��� �������, CX - ���-�� ���������)
; ������ �������� EAX, CX, EDX, DI
InitArray	PROC
IF	RANDOMIZE
		push	cx
		mov	ah,2Ch
		int	21h			; �������� ������� �����
		xchg	ax,cx
		shl	eax,16
		xchg	ax,dx
		inc	eax			; EAX = �������� ��������
		pop	cx
ELSE
		mov	eax,-1
ENDIF
	@@next:	call	Random			; ��������� ���������� �����
		stosd
		loop	@@next			; ��������� ���� ��������� �������
		ret
InitArray	ENDP

; ��������� ���������������� ����� ������� XorShift �� ������ �������� � �������� EAX (�� ������ ���� = 0)
; ��������� � EAX (������� �� ���������� 0)
; ������ ������� EDX
Random		PROC
		mov	edx,eax
		shl	eax,13
		xor	eax,edx

		mov	edx,eax
		shr	eax,17
		xor	eax,edx

		mov	edx,eax
		shl	eax,5
		xor	eax,edx
		ret
Random		ENDP

IF	SHOWARRAYS
; ����� ������ ����������� (DS:DX) � ������ �����, �� ������� ��������� ������ ����� ������� (DS:SI - ������ �������, CX - ���-�� ���������)
; ������ �������� AX, DX, SI
ShowArray	PROC
		mov	ah,9
		int	21h			; ������� ������ �����������
	@@next:	lodsd
		push	cx
		call	ShowSEInt		; ������� �����
		pop	cx
		call	NewLine			; ������� ������
		loop	@@next
		ret
ShowArray	ENDP
ENDIF

; ����� ������ ��������� ����������� ����� EAX �� �����
; ������ �������� EAX, EBX, CX, EDX
ShowSEInt	PROC
		test	eax,eax			; ��������� �����
		jns	@@nosign		; ���������, ���� ����� �������������
		push	ax
		mov	ah,2
		mov	dl,'-'
		int	21h			; ������� ������ '-'
		pop	ax
		neg	eax			; � ������ ����
	@@nosign:
		xor	cx,cx			; ���-�� ���� (���� 0)
		mov	ebx,10			; ������� ��������� (����������)
	@@next:
		xor	edx,edx			; EDX = 0 (����� ��� �������)
		div	ebx			; EAX = EDX:EAX/�������_��������� (10), EDX = �������
		push	dx			; ��������� ����� � �����
		inc	cx			; ����������� ���-�� ����
		test	eax,eax			; ��������� EAX (�����)
		jnz	@@next			; ���������, ���� ������� ��� �� ����� 0

		mov	ah,2
	@@outdigit:
		pop	dx			; ��������� ����� �� �����
		add	dl,'0'			; ����������� � � ������
		int	21h			; ������� �����
		loop	@@outdigit		; ��������� ���� ������ ��������
		ret
ShowSEInt	ENDP

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

INCLUDE		ISort4S1.inc

END		Start
