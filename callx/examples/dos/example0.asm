; ������ ������������� ��������� callx (��� ���������� ��������� ���������� ����)

.model tiny
.286

; ������ ��������� ����� ���� ��� ������������ (����������� �������� 0 � 1 �� ��� ����������); � �������� ���� ������������� � ��� ��� (��. exampleA.asm)
useAddAbs	=	1
useMul10	=	0
useUIntToScr	=	1

.data

if	useUIntToScr
  mPressKey	db	13,10,'Press a key...',13,10,'$'
endif

.code
.startup

	if	useAddAbs
		mov	si,offset Nums
		mov	cx,cNums
		call	AddAbs
	else
		mov	ax,10
	endif
	if	useMul10
		call	Mul10
	endif
	if	useUIntToScr
		call	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
	endif
		int	20h

include		examplex.inc

end
