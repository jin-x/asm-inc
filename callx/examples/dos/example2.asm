; Пример использования механизма callx (вариант 2 - с помещением include-файла в начало кода)

.model tiny
.286

include		callx.inc		; механизм исключения неиспользуемых процедур из кода
usecallx				; запустить механизм

; Вариант использования методики исключения процедуры (возможные значения: 1 или 2)
Variant		=	2

if		Variant eq 1
  inclx		AddAbs, UIntToScr	; список включаемых в код процедур (вспомогательные процедуры [AbsAX] можно не указывать, т.к. в include-файле прописана зависимость AddAbs от AbsAX)
else
  inclx_All	examplex
  exclx		Mul10
endif

.code
.startup

		jmp	Start

include		examplex.inc

	Start:
	ifdef	AddAbs
		mov	si,offset Nums
		mov	cx,cNums
		callx	AddAbs		; в данном случае (здесь и ниже) можно использовать call (а не callx)
	else
		mov	ax,10
	endif
	ifdef	Mul10
		call	Mul10
	endif
	ifdef	UIntToScr
		call	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
	endif
		int	20h

.data					; не перемещайте эту секцию выше секции .code, иначе получите сообщение 'Module is pass dependent' при компиляции TASM'ом (из-за ifdef UIntToScr)

ifdef	UIntToScr
  mPressKey	db	13,10,'Press a key...',13,10,'$'
endif

end
