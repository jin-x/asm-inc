; �ਬ�� �ᯮ�짮����� ��堭���� callx (�ਬ�� ॠ�쭮�� ����, ��ਠ�� 1 - ४�����㥬� + � �ᯮ�짮������ 㭨���ᠫ쭮�� ����, ࠡ���饣� ���� ��� callx.inc)

.model tiny
.286

include		callx.inc		; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ���� (� ��砥 ������⢨� ������� 䠩�� ���� ������������ ��� ��ப�)
ifdef		callx_ver		; �஢�ઠ ������ callx.inc
  usecallx				; �������� ��堭���
else
  callx		equ	call
endif

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

include		examplex.inc

end
