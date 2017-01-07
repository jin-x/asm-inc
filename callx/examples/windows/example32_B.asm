; MASM32: Пример использования механизма callx (пример реального кода, вариант 1 - рекомендуемый + с использованием универсального кода, работающего даже без callx.inc)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; механизм исключения неиспользуемых процедур из кода (в случае отсутствия данного файла просто закомментируйте эту строку)
ifdef		callx_ver		; проверка наличия callx.inc
  usecallx				; запустить механизм
  modulex	example			; имя главного модуля
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

ifdef		callx_ver		; проверка наличия callx.inc
  pdefx		example, <WriteString>	; эта строка должна находиться ПОСЛЕ всех вызовов callx/invokex, но ДО процедур
endif

; Вывод ASCIIZ-строки по адресу EDI (меняет регистры EAX, ECX, EDX, EDI)
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
