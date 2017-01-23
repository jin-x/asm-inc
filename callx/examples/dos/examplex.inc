include		callx.inc			; механизм исключения неиспользуемых процедур из кода (в случае отсутствия данного файла просто закомментируйте эту строку)

ifdef		?usecallx			; проверка использования механизма исключения неиспользуемых процедур из кода
  modulex	examplex			; имя модуля
  prelx		examplex, AddAbs, <AbsAX>
  pdefx		examplex, <AddAbs, AbsAX, UIntToScr, Mul10>
else
  pchkx		equ	<?dummy =>
endif

ifdef		??Version			; TASM
  locals
endif

; Получить абсолютное значение регистра AX (не меняет регистров)
ifndef		?exclAbsAX
pchkx		AbsAX
AbsAX		proc
		push	dx
		cwd
		xor	ax,dx
		sub	ax,dx
		pop	dx
		ret
AbsAX		endp
endif ; ?exclAbsAX

; Сложить абсолютные значения CX слов по адресу SI, результат помещается в AX (меняет регистр DX)
ifndef		?exclAddAbs
pchkx		AddAbs
AddAbs		proc
		cld
		xor	dx,dx
	@@Next:
		lodsw
		call	AbsAX
		add	dx,ax
		loop	@@Next
		xchg	ax,dx
		ret
AddAbs		endp
endif ; ?exclAddAbs

; Умножить число AX на 10 (меняет регистр DX)
ifndef		?exclMul10
pchkx		Mul10
Mul10		proc
		mov	dx,ax
		shl	ax,2
		add	ax,dx
		shl	ax,1
		ret
Mul10		endp
endif ; ?exclMul10

; Вывести беззнаковое число AX на экран (меняет регистры BX, CX, DX)
ifndef		?exclUIntToScr
pchkx		UIntToScr
UIntToScr	proc
		xor	cx,cx
		mov	bx,10			; система счисления
	@@NextDigit:
		xor	dx,dx			; dx = 0 (нужно для деления)
		div	bx			; ax = dx:ax/систему_счисления, dx = остаток
		push	dx			; сохраняем цифру в стеке
		inc	cx			; увеличиваем кол-во цифр
		test	ax,ax
		jnz	@@NextDigit		; переход, если частное ещё не равно 0

		mov	ah,2
	@@OutDigit:
		pop	dx			; извлекаем цифру из стека
		add	dl,'0'			; преобразуем её в символ
		int	21h			; вывод цифры
		loop	@@OutDigit		; повторяем цикл вывода символов
		ret
UIntToScr	endp
endif ; ?exclUIntToScr

.data
ifndef		?exclAddAbs
  Nums		dw	5,-70,12,-25,-48,-96,768
  cNums		=	($-Nums)/2
endif

.code
