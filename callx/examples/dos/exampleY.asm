; ������ ������������� ��������� callx (������ ��������� ����, ������� 1 - � �������������� ���������������� examplez.inc)

.model tiny
.286

include		callx.inc		; �������� ���������� �������������� �������� �� ����
usecallx				; ��������� ��������

.data

mPressKey	db	13,10,'Press a key...',13,10,'$'

.code
.startup

		mov	si,offset Nums
		mov	cx,cNums
		callx	AddAbs
;		callx	Mul10
		callx	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
		int	20h

include		examplez.inc

end
