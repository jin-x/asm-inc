; �ਬ�� �ᯮ�짮����� ��堭���� callx (��ਠ�� 2 - � ����饭��� include-䠩�� � ��砫� ����)

.model tiny
.286

include		callx.inc		; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ����
usecallx				; �������� ��堭���

; ��ਠ�� �ᯮ�짮����� ��⮤��� �᪫�祭�� ��楤��� (�������� ���祭��: 1 ��� 2)
Variant		=	2

if		Variant eq 1
  inclx		AddAbs, UIntToScr	; ᯨ᮪ ����砥��� � ��� ��楤�� (�ᯮ����⥫�� ��楤��� [AbsAX] ����� �� 㪠�뢠��, �.�. � include-䠩�� �ய�ᠭ� ����ᨬ���� AddAbs �� AbsAX)
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
		callx	AddAbs		; � ������ ��砥 (����� � ����) ����� �ᯮ�짮���� call (� �� callx)
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

.data					; �� ��६�頩� ��� ᥪ�� ��� ᥪ樨 .code, ���� ������ ᮮ�饭�� 'Module is pass dependent' �� �������樨 TASM'�� (��-�� ifdef UIntToScr)

ifdef	UIntToScr
  mPressKey	db	13,10,'Press a key...',13,10,'$'
endif

end
