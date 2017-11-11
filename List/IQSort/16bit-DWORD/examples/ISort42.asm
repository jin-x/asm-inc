.MODEL Small
.386

IFDEF		??Version			; TASM
  LOCALS
ENDIF

SHOWARRAYS	=	1			; показывать ли числа массивов (1- да, 0 - нет)?
SPEEDTEST	=	1			; измерять ли скорость (1 - да, 0 - нет)?
RANDOMIZE	=	1			; инициализировать генератор псевдослучайных чисел (1 - значением на основании текущего времени, 0 - постоянным значением)?

Sort4ElemDWords	=	2			; кол-во двойных слов (WORD) в каждом элементе массива

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

NArray		=	2000			; кол-во элементов массива
Array		DD	(NArray*2) dup (?)	; сортируемый массив (первые слова - элементы, по которым происходит сравнение; вторые слова - указатели на соответствующие строки)
Strings		DB	(NArray*12) dup (?)	; массив строк (до 7 символов каждая)

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
		call	InsSort4
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
		lodsd				; значение
	ChkNxt:	xchg	edx,eax
		lodsd				; связанные данные
		lodsd				; значение
		cmp	eax,edx
		jge	ChkOk
		inc	bx			; увеличиваем счётчик ошибок
	ChkOk:	loop	ChkNxt

		movzx	eax,bx
		lea	di,msgCnt
		call	SEIntToStr
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
; Меняет регистры EAX, EBX, CX, EDX, SI, DI
InitArray	PROC
IF	RANDOMIZE
		push	cx
		mov	ah,2Ch
		int	21h			; получаем текущее время
		xchg	ax,cx
		shl	eax,16
		xchg	ax,dx
		inc	eax			; EAX = исходное значение
		pop	cx
ELSE
		mov	eax,-1
ENDIF
	@@next:	call	Random			; генерация случайного числа
		mov	[si],eax		; запись в массив случайного числа
		mov	[si+4],di		; запись в массив адреса строки
		mov	[si+6],di
		add	si,8
		push	eax
		push	cx
		call	SEIntToStr		; вывод строки в массив Strings
		mov	al,'$'
		stosb				; добавление символа '$' в конец строки
		pop	cx
		pop	eax
		loop	@@next			; следующая пара элементов массива
		ret
InitArray	ENDP

; Генерация псевдослучайного числа методом XorShift на основе значения в регистре EAX (не должен быть = 0)
; Результат в EAX (никогда не возвращает 0)
; Меняем регистр EDX
Random		PROC
		mov	edx,eax
		shl	eax,13
		xor	eax,edx

		mov	edx,eax
		shr	eax,17
		xor	eax,edx

		mov	edx,eax
		shl	eax,5
		xor	eax,edx
		ret
Random		ENDP

IF	SHOWARRAYS
; Вывод строки приветствия (DS:DX) и списка строк, на которые указывают вторые слова массива (DS:SI - начало массива, CX - кол-во элементов)
; Меняет регистры EAX, DX, SI
ShowArray	PROC
		mov	ah,9
		int	21h			; выводим строку приветствия
	@@next:	lodsd
		lodsw
		mov	dx,ax
		lodsw
		add	dx,ax
		rcr	dx,1
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

; Вывод целого знакового десятичного числа EAX в буфер по адресу ES:DI (DS=ES)
; Меняет регистры EAX, EBX, CX, EDX, DI (DI указывает за строку)
SEIntToStr	PROC
		test	eax,eax			; проверяем число
		jns	@@nosign		; переходим, если число положительное
		mov	byte ptr [di],'-'	; выводим символ '-'
		inc	di
		neg	eax			; и меняем знак
	@@nosign:
		xor	cx,cx			; кол-во цифр (пока 0)
		mov	ebx,10			; система счисления (десятичная)
	@@next:
		xor	edx,edx			; DS = 0 (нужно для деления)
		div	ebx			; EAX = EDX:EAX/систему_счисления (10), EDX = остаток
		push	dx			; сохраняем цифру в стеке
		inc	cx			; увеличиваем кол-во цифр
		test	eax,eax			; проверяем EAX (число)
		jnz	@@next			; переходим, если частное ещё не равно 0
	@@outdigit:
		pop	ax			; извлекаем цифру из стека
		add	al,'0'			; преобразуем её в символ
		stosb				; выводим цифру
		loop	@@outdigit		; повторяем цикл вывода символов
		ret
SEIntToStr	ENDP

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

INCLUDE		ISort4.inc

END		Start
