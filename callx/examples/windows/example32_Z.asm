; MASM32: Пример использования механизма callx (пример реального кода, вариант 2 - с использованием минималистичного examplez.inc)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; механизм исключения неиспользуемых процедур из кода
usecallx				; запустить механизм

; Такой вариант позволяет использовать call вместо callx
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

; Вывод ASCIIZ-строки по адресу EDI (меняет регистры EAX, ECX, EDX, EDI)
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
