; ������ ������������� ��������� callx (������ ��������� ����, ������� 2 - � ���������� include-����� � ������ ����)

.model tiny
.286

include		callx.inc		; �������� ���������� �������������� �������� �� ����
usecallx				; ��������� ��������

; ����� ������� ��������� ������������ call ������ callx
inclx		AddAbs, UIntToScr	; ������ ���������� � ��� �������� (��������������� ��������� [AbsAX] ����� �� ���������, �.�. � include-����� ��������� ����������� AddAbs �� AbsAX)

.data

mPressKey	db	13,10,'Press a key...',13,10,'$'

.code
.startup

		jmp	Start

include		examplex.inc

	Start:
		mov	si,offset Nums
		mov	cx,cNums
		call	AddAbs
;		call	Mul10
		call	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
		int	20h

end
