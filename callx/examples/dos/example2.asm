; ������ ������������� ��������� callx (������� 2 - � ���������� include-����� � ������ ����)

.model tiny
.286

include		callx.inc		; �������� ���������� �������������� �������� �� ����
usecallx				; ��������� ��������

; ������� ������������� �������� ���������� ��������� (��������� ��������: 1 ��� 2)
Variant		=	2

if		Variant eq 1
  inclx		AddAbs, UIntToScr	; ������ ���������� � ��� �������� (��������������� ��������� [AbsAX] ����� �� ���������, �.�. � include-����� ��������� ����������� AddAbs �� AbsAX)
else
  inclx_All	examplex
  exclx		Mul10
endif

.code
.startup

		jmp	Start

include		examplex.inc

	Start:
	ifdef	AddAbs
		mov	si,offset Nums
		mov	cx,cNums
		callx	AddAbs		; � ������ ������ (����� � ����) ����� ������������ call (� �� callx)
	else
		mov	ax,10
	endif
	ifdef	Mul10
		call	Mul10
	endif
	ifdef	UIntToScr
		call	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
	endif
		int	20h

.data					; �� ����������� ��� ������ ���� ������ .code, ����� �������� ��������� 'Module is pass dependent' ��� ���������� TASM'�� (��-�� ifdef UIntToScr)

ifdef	UIntToScr
  mPressKey	db	13,10,'Press a key...',13,10,'$'
endif

end
