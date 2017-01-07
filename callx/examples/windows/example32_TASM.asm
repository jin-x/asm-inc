; TASM: ������ ������������� ��������� callx (������ ��������� ����, ������� 1 - ������������� + � �������������� �������������� ����, ����������� ���� ��� callx.inc)

.586p
.model flat,stdcall

smart
locals

include		callx.inc		; �������� ���������� �������������� �������� �� ���� (� ������ ���������� ������� ����� ������ ��������������� ��� ������)
ifdef		callx_ver		; �������� ������� callx.inc
  usecallx				; ��������� ��������
  modulex	example			; ��� �������� ������
else
  callx		equ	call
  invokex	equ	invoke
  pchkx		equ	<?dummy =>
endif

includelib	import32.lib

extrn		ExitProcess	: proc
extrn		GetStdHandle	: proc
extrn		WriteFile	: proc

STD_OUTPUT_HANDLE =	-11

.data

mSum		db	'Sum = ',0
NumStr		db	11 dup (?)
Result		dd	?

.code

Start:

		lea	esi,Nums
		mov	ecx,cNums
		callx	AddAbs
;		callx	Mul10
		lea	edi,NumStr
		push	edi
		callx	UIntToStr
		lea	edi,mSum
		callx	WriteString
		pop	edi
		callx	WriteString
		call	ExitProcess, 0

;-----------------------------------------------------------------------------------------------------------------------

ifdef		callx_ver		; �������� ������� callx.inc
  pdefx		example, <WriteString>	; ��� ������ ������ ���������� ����� ���� ������� callx/invokex, �� �� ��������
endif

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
		call	GetStdHandle, STD_OUTPUT_HANDLE
		call	WriteFile, eax, edi, ecx, offset Result, 0
		ret
WriteString	endp
endif ; ?exclWriteString

include		examplex_32.inc

end		Start
