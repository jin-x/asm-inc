; Пример использования механизма callx (пример реального кода, вариант 2 - с помещением include-файла в начало кода)

.model tiny
.286

include		callx.inc		; механизм исключения неиспользуемых процедур из кода
usecallx				; запустить механизм

; Такой вариант позволяет использовать call вместо callx
inclx		AddAbs, UIntToScr	; список включаемых в код процедур (вспомогательные процедуры [AbsAX] можно не указывать, т.к. в include-файле прописана зависимость AddAbs от AbsAX)

.data

mPressKey	db	13,10,'Press a key...',13,10,'$'

.code
.startup

		jmp	Start

include		examplex.inc

	Start:
		mov	si,offset Nums
		mov	cx,cNums
		call	AddAbs
;		call	Mul10
		call	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
		int	20h

end
