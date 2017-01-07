include		callx.inc			; в случае отсутствия данного файла просто закомментируйте эту строку

ifdef		?usecallx			; проверка использования механизма исключения неиспользуемых процедур из кода
  modulex	examplex			; имя модуля
  prelx		examplex, AddAbs, <AbsAX>
  pdefx		examplex, <AddAbs, AbsAX, UIntToStr, Mul10>
else
  pchkx		equ	<?dummy =>
endif

ifdef		??Version			; TASM
  locals
endif

; Получить абсолютное значение регистра EAX (не меняет регистров)
ifndef		?exclAbsAX
pchkx		AbsAX
AbsAX		proc
		push	edx
		cdq
		xor	eax,edx
		sub	eax,edx
		pop	edx
		ret
AbsAX		endp
endif ; ?exclAbsAX

; Сложить абсолютные значения ECX двойных слов по адресу ESI, результат помещается в EAX (меняет регистр EDX)
ifndef		?exclAddAbs
pchkx		AddAbs
AddAbs		proc
		cld
		xor	edx,edx
	@@Next:
		lodsd
		call	AbsAX
		add	edx,eax
		loop	@@Next
		xchg	eax,edx
		ret
AddAbs		endp
endif ; ?exclAddAbs

; Умножить число EAX на 10 (меняет регистр EDX)
ifndef		?exclMul10
pchkx		Mul10
Mul10		proc
		mov	edx,eax
		shl	eax,2
		add	eax,edx
		shl	eax,1
		ret
Mul10		endp
endif ; ?exclMul10

; Преобразовать число EAX в ASCIIZ-строку по адресу EDI (меняет регистры EBX, ECX, EDX)
ifndef		?exclUIntToStr
pchkx		UIntToStr
UIntToStr	proc
		xor	ecx,ecx
		mov	ebx,10			; система счисления
	@@NextDigit:
		xor	edx,edx			; dx = 0 (нужно для деления)
		div	ebx			; ax = dx:ax/систему_счисления, dx = остаток
		push	edx			; сохраняем цифру в стеке
		inc	ecx			; увеличиваем кол-во цифр
		test	eax,eax
		jnz	@@NextDigit		; переход, если частное ещё не равно 0

	@@OutDigit:
		pop	eax			; извлекаем цифру из стека
		add	al,'0'			; преобразуем её в символ
		stosb				; записываем символ (цифру)
		loop	@@OutDigit		; повторяем цикл записи символов
		xor	al,al
		stosb				; записываем символ конца строки
		ret
UIntToStr	endp
endif ; ?exclUIntToStr

.data
ifndef		?exclAddAbs
  Nums		dd	4096,-7075,-333,20500,-1,98765,-2533
  cNums		=	($-Nums)/4
endif

.code
