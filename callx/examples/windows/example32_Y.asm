; MASM32: Пример использования механизма callx (пример реального кода, вариант 1 - с использованием минималистичного examplez.inc)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; механизм исключения неиспользуемых процедур из кода
usecallx				; запустить механизм
modulex	example				; имя главного модуля

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

pdefx		example, <WriteString>	; эта строка должна находиться ПОСЛЕ всех вызовов callx/invokex, но ДО процедур

; Вывод ASCIIZ-строки по адресу EDI (меняет регистры EAX, ECX, EDX, EDI)
ifndef		?exclWriteString
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

include		examplez_32.inc

end		Start
