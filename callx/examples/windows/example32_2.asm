; MASM32: ������ ������������� ��������� callx (������� 2 - � ���������� include-����� � ������ ����)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; �������� ���������� �������������� �������� �� ����
usecallx				; ��������� ��������

modulex		example			; ��� �������� ������

; ������� ������������� �������� ���������� ��������� (��������� ��������: 1 ��� 2)
Variant		=	2

if		Variant eq 1
  inclx		AddAbs, UIntToScr	; ������ ���������� � ��� �������� (��������������� ��������� [AbsAX] ����� �� ���������, �.�. � include-����� ��������� ����������� AddAbs �� AbsAX)
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
		callx	AddAbs		; � ������ ������ (����� � ����) ����� ������������ call (� �� callx)
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

pdefx		example, <WriteString>	; ��� ������ ������ ���������� ����� ���� ������� callx/invokex, �� �� ��������

; ����� ASCIIZ-������ �� ������ EDI (������ �������� EAX, ECX, EDX, EDI)
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

.data					; �� ����������� ��� ������ ���� ������ .code, ����� �������� ��������� 'Module is pass dependent' ��� ���������� TASM'�� (��-�� ifdef UIntToScr)

ifdef		UIntToStr
  mSum		db	'Sum = ',0
  NumStr	db	11 dup (?)
  Result	dd	?
endif

end		Start
