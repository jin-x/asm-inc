; MASM32: Пример использования механизма callx (без применения механизма исключения кода)

.586p
.model flat,stdcall
option casemap:none

; Данные константы нужны лишь для демонстрации (используйте значения 0 и 1 на своё усмотрение); в реальном коде необходимости в них нет (см. example32_A.asm)
useAddAbs	=	1
useMul10	=	0
useUIntToStr	=	1

include		windows.inc
include		kernel32.inc
includelib	kernel32.lib

.data

if		useUIntToStr
  mSum		db	'Sum = ',0
  NumStr	db	11 dup (?)
  Result	dd	?
endif

.code

Start:

	if	useAddAbs
		mov	esi,offset Nums
		mov	ecx,cNums
		call	AddAbs
	else
		mov	eax,10
	endif
	if	useMul10
		call	Mul10
	endif
	if	useUIntToStr
		mov	edi,offset NumStr
		push	edi
		call	UIntToStr
		mov	edi,offset mSum
		call	WriteString
		pop	edi
		call	WriteString
	endif
		invoke  ExitProcess, NULL

; Вывод ASCIIZ-строки по адресу EDI (меняет регистры EAX, ECX, EDX, EDI)
if		useUIntToStr
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
endif ; useUIntToStr

include		examplex_32.inc

end		Start
