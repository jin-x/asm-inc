.MODEL Small

IFDEF		??Version			; TASM
  LOCALS
ENDIF

SHOWARRAYS	=	1			; показывать ли числа массивов (1- да, 0 - нет)?
SPEEDTEST	=	1			; измерять ли скорость (1 - да, 0 - нет)?
RANDOMIZE	=	1			; инициализировать генератор псевдослучайных чисел (1 - значением на основании текущего времени, 0 - постоянным значением)?

SortElemWords	=	2			; кол-во слов (WORD) в каждом элементе массива

IF	SPEEDTEST
  .386
ELSE
  .286
ENDIF

.STACK 100h

.DATA

IF	SHOWARRAYS
  msgSrc	DB	'Source array:',13,10,'$'
ENDIF
msgSorting	DB	13,10,'Sorting...$'
msgDone		DB	' done!',13,10,'$'
IF	SHOWARRAYS
  msgRes	DB	13,10,'Sorted array:',13,10,'$'
ENDIF
msgChk		DB	13,10,'Checking...$'
msgChkRes	DB	' done!',13,10,'Fails count = '
msgCnt		DB	'     ',13,10,'$'
IF	SPEEDTEST
  msgSpeed	DB	13,10,'Sort procedure took $'
  msgTicks	DB	' ticks',13,10,'$'
ENDIF

.DATA?

NArray		=	2500			; кол-во элементов массива
Array		DW	(NArray*2) dup (?)	; сортируемый массив (первые слова - элементы, по которым происходит сравнение; вторые слова - указатели на соответствующие строки)
Strings		DB	(NArray*7) dup (?)	; массив строк (до 7 символов каждая)

.CODE

Start:

                cld
		mov	ax,@data
		mov	ds,ax			; DS = сегмент данных
		mov	es,ax			; ES = сегмент данных

		; Инициализация массивов
		lea	si,Array
		lea	di,Strings
		mov	cx,NArray
		call	InitArray

IF	SHOWARRAYS
		; Вывод исходного массива на экран
		lea	si,Array
		mov	cx,NArray
		lea	dx,msgSrc
		call	ShowArray
ENDIF
		; Сортировка массива
		lea	dx,msgSorting
		call	ShowMsg

IF	SPEEDTEST
		cli
		db	0Fh,31h			; rdtsc
		push	eax
ENDIF
		; Входные данные: DS:DX = адрес массива, CX = кол-во элементов массива
		lea	dx,Array
		mov	cx,NArray
		call	InsSort
IF	SPEEDTEST
		db	0Fh,31h			; rdtsc
		sti
		push	eax
ENDIF
		lea	dx,msgDone
		call	ShowMsg

IF	SHOWARRAYS
		; Вывод отсортированного массива на экран
		lea	si,Array
		mov	cx,NArray
		lea	dx,msgRes
		call	ShowArray
ENDIF
		; Проверка массива
		lea	dx,msgChk
		call	ShowMsg

		lea	si,Array
		mov	cx,NArray-1
		xor	bx,bx			; BX = 0 - кол-во ошибок
		lodsw				; значение
	ChkNxt:	xchg	dx,ax
		lodsw				; связанные данные
		lodsw				; значение
		cmp	ax,dx
		jge	ChkOk
		inc	bx			; увеличиваем счётчик ошибок
	ChkOk:	loop	ChkNxt

		mov	ax,bx
		lea	di,msgCnt
		call	SIntToStr
		lea	dx,msgChkRes
		call	ShowMsg

IF	SPEEDTEST
		lea	dx,msgSpeed
		call	ShowMsg
		pop	eax
		pop	edx
		sub	eax,edx
		call	ShowEUInt3
		lea	dx,msgTicks
		call	ShowMsg
ENDIF
		mov	ax,4C00h
		int	21h			; выход из программы

; Вывод строки по адресу DS:DX на экран
ShowMsg		PROC
		mov	ah,9
		int	21h
		ret
ShowMsg		ENDP

; Инициализация массивов (DS:SI - начало массива, ES:DI - буфер для строк, CX - кол-во элементов)
; Меняет регистры AX, BX, CX, DX, SI, DI
InitArray	PROC
IF	RANDOMIZE
		push	cx
		mov	ah,2Ch
		int	21h			; получаем текущее время
		xchg	ax,dx
		xchg	cl,ch
		add	ax,cx
		inc	ax			; AX = исходное значение
		pop	cx
ELSE
		mov	ax,-1
ENDIF
	@@next:	call	Random			; генерация случайного числа
		mov	[si],ax			; запись в массив случайного числа
		mov	[si+2],di		; запись в массив адреса строки
		add	si,4
		push	ax
		push	cx
		call	SIntToStr		; вывод строки в массив Strings
		mov	al,'$'
		stosb				; добавление символа '$' в конец строки
		pop	cx
		pop	ax
		loop	@@next			; следующая пара элементов массива
		ret
InitArray	ENDP

; Генерация псевдослучайного числа методом XorShift на основе значения в регистре AX (не должен быть = 0)
; Результат в AX (никогда не возвращает 0)
; Меняем регистр DX
Random		PROC
		mov	dx,ax
		shl	ax,13
		xor	ax,dx

		mov	dx,ax
		shr	ax,9
		xor	ax,dx

		mov	dx,ax
		shl	ax,7
		xor	ax,dx
		ret
Random		ENDP

IF	SHOWARRAYS
; Вывод строки приветствия (DS:DX) и списка строк, на которые указывают вторые слова массива (DS:SI - начало массива, CX - кол-во элементов)
; Меняет регистры AX, DX, SI
ShowArray	PROC
		mov	ah,9
		int	21h			; выводим строку приветствия
	@@next:	lodsw
		lodsw
		mov	dx,ax
		mov	ah,9
		int	21h			; выводим строку
		mov	ah,2
		mov	dl,13
		int	21h			; выводим CR
		mov	dl,10
		int	21h			; выводим LF
		loop	@@next
		ret
ShowArray	ENDP
ENDIF

; Вывод целого знакового десятичного числа AX в буфер по адресу ES:DI (DS=ES)
; Меняет регистры AX, BX, CX, DX, DI (DI указывает за строку)
SIntToStr	PROC
		test	ax,ax			; проверяем число
		jns	@@nosign		; переходим, если число положительное
		mov	byte ptr [di],'-'	; выводим символ '-'
		inc	di
		neg	ax			; и меняем знак
	@@nosign:
		xor	cx,cx			; кол-во цифр (пока 0)
		mov	bx,10			; система счисления (десятичная)
	@@next:
		xor	dx,dx			; DX = 0 (нужно для деления)
		div	bx			; AX = DX:AX/систему_счисления (10), DX = остаток
		push	dx			; сохраняем цифру в стеке
		inc	cx			; увеличиваем кол-во цифр
		test	ax,ax			; проверяем AX (число)
		jnz	@@next			; переходим, если частное ещё не равно 0
	@@outdigit:
		pop	ax			; извлекаем цифру из стека
		add	al,'0'			; преобразуем её в символ
		stosb				; выводим цифру
		loop	@@outdigit		; повторяем цикл вывода символов
		ret
SIntToStr	ENDP

; Вывод целого беззнакового десятичного числа EAX на экран с группировкой по 3 цифры
; Меняет регистры EAX, EBX, CX, EDX
IF	SPEEDTEST
ShowEUInt3	PROC
		xor	cx,cx			; CL = кол-во символов (пока 0), CH = кол-во цифр в текущей тройке
		mov	ebx,10			; система счисления
	@@next:
		cmp	ch,3
		jb	@@not3
		push	"'"
		inc	cx
		xor	ch,ch
	@@not3:
		xor	edx,edx			; EDX = 0 (нужно для деления)
		div	ebx			; EAX = EDX:EAX/систему_счисления (10), (E)DX = остаток
		add	dl,'0'			; преобразуем его в символ
		push	dx			; сохраняем цифру в стеке
		add	cx,101h			; увеличиваем кол-во символов и кол-во цифр в текущей тройке
		test	eax,eax
		jnz	@@next			; переход, если частное ещё не равно 0

		mov	ah,2
	@@outdigit:
		pop	dx			; извлекаем символ из стека
		int	21h			; выводим его на экран
	@@nospace:
		dec	cl
		jnz	@@outdigit		; повторяем цикл вывода символов
		ret
ShowEUInt3	ENDP
ENDIF

INCLUDE		ISort.inc

END		Start
