; MASM32: Пример использования механизма callx (вариант 1 - рекомендуемый)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; механизм исключения неиспользуемых процедур из кода
usecallx				; запустить механизм

modulex		example			; имя главного модуля

; Данные константы нужны лишь для демонстрации (используйте значения 0 и 1 на своё усмотрение); в реальном коде необходимости в них нет (см. example32_B.asm)
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
		callx	AddAbs
	else
		mov	eax,10
	endif
	if	useMul10
		callx	Mul10
	endif
	if	useUIntToStr
		mov	edi,offset NumStr
		push	edi
		callx	UIntToStr
		mov	edi,offset mSum
		callx	WriteString
		pop	edi
		callx	WriteString
	endif
		invoke  ExitProcess, NULL

;-----------------------------------------------------------------------------------------------------------------------

pdefx		example, <WriteString>		; эта строка должна находиться ПОСЛЕ всех вызовов callx/invokex, но ДО процедур

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
