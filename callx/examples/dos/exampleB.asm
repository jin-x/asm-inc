; Пример использования механизма callx (пример реального кода, вариант 1 - рекомендуемый + с использованием универсального кода, работающего даже без callx.inc)

.model tiny
.286

include		callx.inc		; механизм исключения неиспользуемых процедур из кода (в случае отсутствия данного файла просто закомментируйте эту строку)
ifdef		callx_ver		; проверка наличия callx.inc
  usecallx				; запустить механизм
else
  callx		equ	call
endif

.data

mPressKey	db	13,10,'Press a key...',13,10,'$'

.code
.startup

		mov	si,offset Nums
		mov	cx,cNums
		callx	AddAbs
;		callx	Mul10
		callx	UIntToScr
		mov	ah,9
		mov	dx,offset mPressKey
		int	21h
		xor	ah,ah
		int	16h
		int	20h

include		examplex.inc

end
