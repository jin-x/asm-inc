; �ਬ�� �ᯮ�짮����� ��堭���� callx (�ਬ�� ॠ�쭮�� ����, ��ਠ�� 2 - � �ᯮ�짮������ ����������筮�� examplez.inc)

.model tiny
.286

include		callx.inc		; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ����
usecallx				; �������� ��堭���

; ����� ��ਠ�� �������� �ᯮ�짮���� call ����� callx
inclx_All	?
exclx		Mul10

.data

mPressKey	db	13,10,'Press a key...',13,10,'$'

.code
.startup

		jmp	Start

include		examplez.inc

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
