; MASM32: �ਬ�� �ᯮ�짮����� ��堭���� callx (�ਬ�� ॠ�쭮�� ����, ��ਠ�� 1 - ४�����㥬� + � �ᯮ�짮������ 㭨���ᠫ쭮�� ����, ࠡ���饣� ���� ��� callx.inc)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ���� (� ��砥 ������⢨� ������� 䠩�� ���� ������������ ��� ��ப�)

; ���� 㭨���ᠫ���樨 ���� (�᫨ ��� 㤠����, �ணࠬ�� �� �㤥� �������஢����� ��� 䠩�� callx.inc, ���� �᫨ ���������஢��� �।����� ��ப�)
ifdef		callx_ver		; �஢�ઠ ������ callx.inc
  usecallx				; �������� ��堭���
  modulex	example			; ��� �������� �����
else
  callx		equ	call
  invokex	equ	invoke
  pchkx		equ	<?dummy =>
endif

include		windows.inc
include		kernel32.inc
includelib	kernel32.lib

.data

mSum		db	'Sum = ',0
NumStr		db	11 dup (?)
Result		dd	?

.code

Start:

		mov	esi,offset Nums
		mov	ecx,cNums
		callx	AddAbs
;		callx	Mul10
		mov	edi,offset NumStr
		push	edi
		callx	UIntToStr
		mov	edi,offset mSum
		callx	WriteString
		pop	edi
		callx	WriteString
		invoke  ExitProcess, NULL

;-----------------------------------------------------------------------------------------------------------------------

ifdef		callx_ver		; �஢�ઠ ������ callx.inc
  pdefx		example, <WriteString>	; �� ��ப� ������ ��室����� ����� ��� �맮��� callx/invokex, �� �� ��楤��
endif

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
		invoke	WriteFile, eax, edi, ecx, addr Result, 0
		ret
WriteString	endp
endif ; ?exclWriteString

include		examplex_32.inc

end		Start
