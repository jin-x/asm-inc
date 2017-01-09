;############################################
;##                                        ##
;##   -= EXTRA MACROSES (MINI EDITION) =-  ##
;##      МАКРОСЫ ДЛЯ MASM/TASM (16/32)     ##
;##           Сокращённый вариант          ##
;##         [ v1.00 :: 09.01.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmac_ver	=	100h			; версия данного файла (word: старший байт - целая часть, младший - дробная)

;-----------------------------------------------------------------------------------------------------------------------

; Выполнение инструкций/директив/макросов, заданных одной строкой
_		macro	p1, p2, p3, p4, p5, p6, p7, p8, p9, p10
	irp	x, <<p1>, <p2>, <p3>, <p4>, <p5>, <p6>, <p7>, <p8>, <p9>, <p10>>
		x
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------
;-- УСЛОВНЫЕ МАКРОСЫ ---------------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; Если cond, исполнить do1; иначе do0
ifel		macro	cond:req, do1, do0
	ifelx	< cond>, <do1>, <do0>
endm
ifdo		equ	<ifel>

ifn		equ	<ife>

; Если НЕ cond, исполнить do1; иначе do0
ifnel		macro	cond:req, do1, do0
	ifelx	<e cond>, <do1>, <do0>
endm
ifeel		equ	<ifnel>

; Если cond1, исполнить do1; иначе если cond2, исполнить do2; иначе do0
ifelif		macro	cond1:req, do1, cond2:req, do2, do0
	ifelifx	< cond1>, <do1>, < cond2>, <do2>, <do0>
endm

; Аналогично ifel, но условие подставляется слитно с if, например: ifelx <def debug>, <int 3>, nop
ifelx		macro	cond:req, do1, do0
	if&cond
		do1
	else
		do0
	endif
endm
ifx		equ	<ifelx>

; Аналогично ifelif, но условия подставляются слитно с if, например: ifelifx <def debug>, <int 3>, <idn <spec>, <stosb>>, stosb, nop
ifelifx		macro	cond1:req, do1, cond2:req, do2, do0
	if&cond1
		do1
	elseif&cond2
		do2
	else
		do0
	endif
endm

;-----------------------------------------------------------------------------------------------------------------------

; Если идентификатор value определён, установить значение const = value, иначе установить значение _const = defval
defdef		macro	const:req, value:req, defval:req
  ifdef		value
	const	=	value
  else
	const	=	defval
  endif
endm

; Если константа const определена, установить значение _const = const, иначе установить значение _const = defval
_defdef		macro	const:req, defval:req
	defdef	<_&const>, <const>, <defval>
endm

;-----------------------------------------------------------------------------------------------------------------------

; Вывод сообщения txt об ошибке (одинаковый синтаксис для MASM и TASM)
?err		macro	txt
  ifel	isTASM, <.err "&txt">, <.err <&txt>>
endm

; Вывод сообщения txt об ошибке при выполнении условия cond
?errif		macro	cond:req, txt
  ifx	< cond>, <?err <txt>>
endm

; Аналогично ?errif, но условие подставляется слитно с if, например: ?errifx <ndef ok>, <Error message>
?errifx		macro	cond:req, txt
  ifx	<cond>, <?err <txt>>
endm

;-----------------------------------------------------------------------------------------------------------------------

; Следующие идентификаторы определены для более удобного использования вложенных условных директив компиляции, например:
; if abc
;  ?if def
;   ...
;  ?endif
; endif
?if		equ	<if>
?ifdef		equ	<ifdef>
?ifndef		equ	<ifndef>
?ifb		equ	<ifb>
?ifnb		equ	<ifnb>
?ifidn		equ	<ifidn>
?ifidni		equ	<ifidni>
?ifdif		equ	<ifdif>
?ifdifi		equ	<ifdifi>
?if1		equ	<if1>
?if2		equ	<if2>
?else		equ	<else>
?elseif		equ	<elseif>
?elseifdef	equ	<elseifdef>
?elseifndef	equ	<elseifndef>
?elseifb	equ	<elseifb>
?elseifnb	equ	<elseifnb>
?elseifidn	equ	<elseifidn>
?elseifidni	equ	<elseifidni>
?elseifdif	equ	<elseifdif>
?elseifdifi	equ	<elseifdifi>
?elseif1	equ	<elseif1>
?elseif2	equ	<elseif2>
?endif		equ	<endif>

;=======================================================================================================================

;-----------------------------------------------------------------------------------------------------------------------
;-- ОПРЕДЕЛЕНИЕ ЗНАЧЕНИЙ КОНСТАНТ --------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

false		=	0
true		=	not false

; Установка isTASM = true и isMASM = false, если текущий компилятор - TASM, иначе наоборот
ifelx		<def ??Version>, <_ <isTASM = true>, <isMASM = false>>, <_ <isMASM = true>, <isTASM = false>>

; Установка идентификатора @32Bit (32-битный режим) в MASM
ifdo		isMASM, <@32Bit = (@WordSize eq 4)>

; Разрешены ли инструкции/регистры процессора 80386
@386		=	(@Cpu and 8) or (@Cpu eq 0)	; в TASM @Cpu = 0 в режиме .586

; Включить режим оптимизации и использования локальных меток, начинающихся на @@
ifdo		isTASM,	<_ smart, locals>

; Проверка требуемой версии данного include-файла, заданной через константу xmacro_ver_req или xmac_ver_req
ifx		<def xmac_ver_req>, <?errif <xmac_ver_req gt xmac_ver>, <Version of 'xmac.inc' is less than required :(>>
?errifx		<def xmacro_ver_req>, <Full edition of 'xmacro.inc' is required :(>

;=======================================================================================================================

;-----------------------------------------------------------------------------------------------------------------------
;-- МАКРОСЫ, ОПИСЫВАЮЩИЕ ИЛИ УПРОЩАЮЩИЕ ИСПОЛЬЗОВАНИЕ ИНСТРУКЦИЙ ПРОЦЕССОРА --------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; Недокументированная инструкция salc (setalc)
salc		macro
		db	0D6h
endm
setalc		equ	<salc>

; Инструкция rdtsc
if		isTASM
  rdtsc		macro
		db	0Fh,31h
  endm
endif

;-----------------------------------------------------------------------------------------------------------------------

; Прыжок через 1 байт; если указан 1-байтовый регистр reg, его значение изменяется, но сохраняются флаги, иначе изменяются флаги, но сохраняются регистры
jmp_1		macro	reg
	local	x
	x	=	$
	ifnbel	reg, <mov reg,0>, <test al,0>
		org	$-1
	?errif <$-x ne 1>, <Wrong register '&reg' for this macro>
endm

; Прыжок через 2 байта; если указан 2-байтовый регистр reg, его значение изменяется, но сохраняются флаги, иначе изменяются флаги, но сохраняются регистры
jmp_2		macro	reg
	local	x
	x	=	$
	ifnbel	reg, <mov reg,0>, <test ax,0>
		org	$-2
	ife	@32Bit
	  ?errif <$-x ne 1>, <Wrong register '&reg' for this macro>
	else
	  ?errif <$-x ne 2>, <Wrong register '&reg' for this macro>
	endif
endm

; Прыжок через 4 байта; если указан 4-байтовый регистр reg, его значение изменяется, но сохраняются флаги, иначе изменяются флаги, но сохраняются регистры
jmp_4		macro	reg
	local	x
	x	=	$
	ifnbel	<reg>, <mov reg,0>, <test eax,0>
		org	$-4
	if	@32Bit
	  ?errif <$-x ne 1>, <Wrong register '&reg' for this macro>
	else
	  ?errif <$-x ne 2>, <Wrong register '&reg' for this macro>
	endif
endm

jmps		equ	<jmp short>
jmpn		equ	<jmp near>

; jmp far seg:ofs (если seg:ofs не указан, генерируется только 1 байт опкода)
jmpf		macro	seg, ofs
		db	0EAh
	ifelx	<nb <seg>>, <dw ofs, seg>
endm

; call far seg:ofs (если seg:ofs не указан, генерируется только 1 байт опкода)
callf		macro	seg, ofs
		db	09Ah
	ifelx	<nb <seg>>, <dw ofs, seg>
endm

; call int (pushf + callf)
calli		macro	seg, ofs
		pushf
		callf	seg, ofs
endm

;-----------------------------------------------------------------------------------------------------------------------

; Выполнение присвоения значения регистру с помощью mov или xor (если src = 0)
; Чтобы передать в параметре src константу, необходимо поставить перед ней % (movx ax,%val), иначе xor не выполнится
movx		macro	dst:req, src:req
	ifelx	<idn <src>, <0>>, <xor dst,dst>, <mov dst,src>
endm

; Выполнение сравнение регистра с помощью cmp или test (если src = 0)
; Чтобы передать в параметре src константу, необходимо поставить перед ней % (cmpx ax,%val), иначе test не выполнится
cmpx		macro	dst:req, src:req
	ifelx	<idn <src>, <0>>, <test dst,dst>, <cmp dst,src>
endm

; Выполнение присвоения op = val через стек
movp		macro	op:req, val:req
		push	val
		pop	op
endm

; Выполнение присвоения op = val через временный регистр temp (по умолчанию ax; например, movt ds,@data или movt es,0A000h,dx)
movt		macro	op:req, val:req, temp:=<ax>
		mov	temp,val
		pop	op2
endm

; Обмен значениями op1 и op2 через стек
; Макрос не проверяет соответствие разрядности операндов !!!
xchgp		macro	op1:req, op2:req
		push	op1
		push	op2
		pop	op1
		pop	op2
endm

; Сохранение регистров в одну строку: push ax, bx, cx, dx
pushx		macro	regs:vararg
	irp	r, <regs>
		push	r
	endm
endm

; Восстановление регистров в одну строку: pop dx, cx, bx, ax
popx		macro	regs:vararg
	irp	r, <regs>
		pop	r
	endm
endm

; Восстановление регистров в обратном порядке: popb ax, bx, cx, dx
popb		macro	regs:vararg
	local	n, i
	n = 0
	irp	r, <regs>
	  n = n + 1
	endm
	while	n gt 0
	  i = 0
	  irp	r, <regs>
	    i = i + 1
	    if	i eq n
		pop	r
	    endif
	  endm
	  n = n - 1
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------

; Данный макрос определяет множество других макросов, которые выполняют lodsb/lodsw/lodsd с использованием сегментов, отличных от ds
; lodsb_seg, lodsw_seg, lodsd_seg с названием сегментного регистра в качестве аргумента (например, lodsb_seg es)
; lodsb_es, lodsb_cs, lodsb_ss
; lodsw_es, lodsw_cs, lodsw_ss
; lodsd_es, lodsd_cs, lodsd_ss без аргументов
irp		x, <byte, word, dword>
  local	t, m1, m2
  t	substr	<x>,1,1
  m1	catstr	<lods>,t,<_seg>
  m1	macro	seg:req
	lods x ptr seg:[si]
  endm
  irp		s, <es, cs, ss>
    m2	catstr	<lods>,t,<_>,<s>
    m2	macro
	lods x ptr s:[si]
    endm
  endm
endm

; Данный макрос определяет множество других макросов, которые выполняют outsb/outsw/outsd с использованием сегментов, отличных от ds
; outsb_seg, outsw_seg, outsd_seg с названием сегментного регистра в качестве аргумента (например, outsb_seg es)
; outsb_es, outsb_cs, outsb_ss,
; outsw_es, outsw_cs, outsw_ss,
; outsd_es, outsd_cs, outsd_ss без аргументов
irp		x, <byte, word, dword>
  local	t, m1, m2
  t	substr	<x>,1,1
  m1	catstr	<outs>,t,<_seg>
  m1	macro	seg:req
	outs x ptr seg:[si]
  endm
  irp		s, <es, cs, ss>
    m2	catstr	<outs>,t,<_>,<s>
    m2	macro
	outs x ptr s:[si]
    endm
  endm
endm

;-----------------------------------------------------------------------------------------------------------------------

; Вызов прерывания no с установкой регистров reg1 = val1, reg2 = val2...
; Если указан только reg1 без val1, то значение reg1 заносится в ah (или ax, если число > 0FFh)
; Например: intfn 16h,0 или intfn 15h,ah,86h,cx,0,dx,10000
intfn		macro	no:req, reg1:req, val1, reg2, val2, reg3, val3, reg4, val4, reg5, val5
	ifnb	<val1>
		movx	reg1,val1
	else
	  ifel	<reg1 lt 100h>, <movx ah,reg1>, <movx ax,reg1>
	endif
	ifelx	<nb <reg2>>, <movx reg2,val2>
	ifelx	<nb <reg3>>, <movx reg3,val3>
	ifelx	<nb <reg4>>, <movx reg4,val4>
	ifelx	<nb <reg5>>, <movx reg5,val5>
		int	no
endm
