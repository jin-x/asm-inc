; Пример использования механизма callx (пример реального кода, без применения механизма исключения кода)

.model tiny
.286

.data

mPressKey	db	13,10,'Press a key...',13,10,'$'

.code
.startup

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

include		examplex.inc

end
