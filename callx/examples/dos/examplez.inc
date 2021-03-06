include		callx.inc			; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ���� (� ��砥 ������⢨� ������� 䠩�� ���� ������������ ��� ��ப�)

ifdef		?usecallx			; �஢�ઠ �ᯮ�짮����� ��堭���� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ����
  prelx,	AddAbs, <AbsAX>
  pdefx,	<AddAbs, AbsAX, UIntToScr, Mul10>
endif

ifdef		??Version			; TASM
  locals
endif

; ������� ��᮫�⭮� ���祭�� ॣ���� AX (�� ����� ॣ���஢)
ifndef		?exclAbsAX
AbsAX		proc
		push	dx
		cwd
		xor	ax,dx
		sub	ax,dx
		pop	dx
		ret
AbsAX		endp
endif ; ?exclAbsAX

; ������� ��᮫��� ���祭�� CX ᫮� �� ����� SI, १���� ����頥��� � AX (����� ॣ���� DX)
ifndef		?exclAddAbs
AddAbs		proc
		cld
		xor	dx,dx
	@@Next:
		lodsw
		call	AbsAX
		add	dx,ax
		loop	@@Next
		xchg	ax,dx
		ret
AddAbs		endp
endif ; ?exclAddAbs

; �������� �᫮ AX �� 10 (����� ॣ���� DX)
ifndef		?exclMul10
Mul10		proc
		mov	dx,ax
		shl	ax,2
		add	ax,dx
		shl	ax,1
		ret
Mul10		endp
endif ; ?exclMul10

; �뢥�� ����������� �᫮ AX �� ��࠭ (����� ॣ����� BX, CX, DX)
ifndef		?exclUIntToScr
UIntToScr	proc
		xor	cx,cx
		mov	bx,10			; ��⥬� ��᫥���
	@@NextDigit:
		xor	dx,dx			; dx = 0 (�㦭� ��� �������)
		div	bx			; ax = dx:ax/��⥬�_��᫥���, dx = ���⮪
		push	dx			; ��࠭塞 ���� � �⥪�
		inc	cx			; 㢥��稢��� ���-�� ���
		test	ax,ax
		jnz	@@NextDigit		; ���室, �᫨ ��⭮� ��� �� ࠢ�� 0

		mov	ah,2
	@@OutDigit:
		pop	dx			; ��������� ���� �� �⥪�
		add	dl,'0'			; �८�ࠧ㥬 �� � ᨬ���
		int	21h			; �뢮� ����
		loop	@@OutDigit		; �����塞 横� �뢮�� ᨬ�����
		ret
UIntToScr	endp
endif ; ?exclUIntToScr

.data
ifndef		?exclAddAbs
  Nums		dw	5,-70,12,-25,-48,-96,768
  cNums		=	($-Nums)/2
endif

.code
