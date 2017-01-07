; MASM32: Пример использования механизма callx (вариант 2 - с помещением include-файла в начало кода)

.586p
.model flat,stdcall
option casemap:none

include		callx.inc		; механизм исключения неиспользуемых процедур из кода
usecallx				; запустить механизм

modulex		example			; имя главного модуля

; Вариант использования методики исключения процедуры (возможные значения: 1 или 2)
Variant		=	2

if		Variant eq 1
  inclx		AddAbs, UIntToScr	; список включаемых в код процедур (вспомогательные процедуры [AbsAX] можно не указывать, т.к. в include-файле прописана зависимость AddAbs от AbsAX)
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
		callx	AddAbs		; в данном случае (здесь и ниже) можно использовать call (а не callx)
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

pdefx		example, <WriteString>	; эта строка должна находиться ПОСЛЕ всех вызовов callx/invokex, но ДО процедур

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
		mov	edx,offset Result
		invoke	WriteFile, eax, edi, ecx, edx, 0
		ret
WriteString	endp
endif ; ?exclWriteString

.data					; не перемещайте эту секцию выше секции .code, иначе получите сообщение 'Module is pass dependent' при компиляции TASM'ом (из-за ifdef UIntToScr)

ifdef		UIntToStr
  mSum		db	'Sum = ',0
  NumStr	db	11 dup (?)
  Result	dd	?
endif

end		Start
