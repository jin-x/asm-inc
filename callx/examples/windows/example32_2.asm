; MASM32: �ਬ�� �ᯮ�짮����� ��堭���� callx (��ਠ�� 2 - � ����饭��� include-䠩�� � ��砫� ����)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ����
usecallx				; �������� ��堭���

modulex		example			; ��� �������� �����

; ��ਠ�� �ᯮ�짮����� ��⮤��� �᪫�祭�� ��楤��� (�������� ���祭��: 1 ��� 2)
Variant		=	2

if		Variant eq 1
  inclx		AddAbs, UIntToScr	; ᯨ᮪ ����砥��� � ��� ��楤�� (�ᯮ����⥫�� ��楤��� [AbsAX] ����� �� 㪠�뢠��, �.�. � include-䠩�� �ய�ᠭ� ����ᨬ���� AddAbs �� AbsAX)
else
  inclx_All	examplex
  exclx		Mul10
endif

include		windows.inc
include		kernel32.inc
includelib	kernel32.lib

.code

Start:
		jmp	Begin

include		examplex_32.inc

	Begin:
	ifdef	AddAbs
		mov	esi,offset Nums
		mov	ecx,cNums
		callx	AddAbs		; � ������ ��砥 (����� � ����) ����� �ᯮ�짮���� call (� �� callx)
	else
		mov	eax,10
	endif
	ifdef	Mul10
		call	Mul10
	endif
	ifdef	UIntToStr
		mov	edi,offset NumStr
		push	edi
		call	UIntToStr
		mov	edi,offset mSum
		callx	WriteString
		pop	edi
		callx	WriteString
	endif
		invoke  ExitProcess, NULL

;-----------------------------------------------------------------------------------------------------------------------

pdefx		example, <WriteString>	; �� ��ப� ������ ��室����� ����� ��� �맮��� callx/invokex, �� �� ��楤��

; �뢮� ASCIIZ-��ப� �� ����� EDI (����� ॣ����� EAX, ECX, EDX, EDI)
ifndef		?exclWriteString
pchkx		WriteString
WriteString	proc
		push	edi
		mov	ecx,65536
		xor	al,al
		repne scasb
		neg	ecx
		add	ecx,65535
		pop	edi
		invoke	GetStdHandle, STD_OUTPUT_HANDLE
		mov	edx,offset Result
		invoke	WriteFile, eax, edi, ecx, edx, 0
		ret
WriteString	endp
endif ; ?exclWriteString

.data					; �� ��६�頩� ��� ᥪ�� ��� ᥪ樨 .code, ���� ������ ᮮ�饭�� 'Module is pass dependent' �� �������樨 TASM'�� (��-�� ifdef UIntToScr)

ifdef		UIntToStr
  mSum		db	'Sum = ',0
  NumStr	db	11 dup (?)
  Result	dd	?
endif

end		Start
