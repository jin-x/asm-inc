; MASM32: �ਬ�� �ᯮ�짮����� ��堭���� callx (�ਬ�� ॠ�쭮�� ����, ��ਠ�� 2 - � �ᯮ�짮������ ����������筮�� examplez.inc)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ����
usecallx				; �������� ��堭���

; ����� ��ਠ�� �������� �ᯮ�짮���� call ����� callx
inclx_All	?
exclx		Mul10

include		windows.inc
include		kernel32.inc
includelib	kernel32.lib

.data

mSum		db	'Sum = ',0
NumStr	db	11 dup (?)
Result	dd	?

.code

Start:
		jmp	Begin

include		examplez_32.inc

	Begin:
		mov	esi,offset Nums
		mov	ecx,cNums
		call	AddAbs
;		call	Mul10
		mov	edi,offset NumStr
		push	edi
		call	UIntToStr
		mov	edi,offset mSum
		call	WriteString
		pop	edi
		call	WriteString
		invoke  ExitProcess, NULL

;-----------------------------------------------------------------------------------------------------------------------

; �뢮� ASCIIZ-��ப� �� ����� EDI (����� ॣ����� EAX, ECX, EDX, EDI)
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

end		Start
