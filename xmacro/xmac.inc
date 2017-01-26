;############################################
;##                                        ##
;##   -= EXTRA MACROSES (MINI EDITION) =-  ##
;##      МАКРОСЫ ДЛЯ MASM/TASM (16/32)     ##
;##          Сокращённая редакция          ##
;##         [ v1.01 :: 26.01.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmac_ver	=	101h			; версия данного файла (word: старший байт - целая часть, младший - дробная)

;------------------------------------------------------------------------------------------------------------------------
;
; -= ИСТОРИЯ ВЕРСИЙ =-
;
; v1.01 (26.01.2017)
; [+] Добавлены идентификаторы $ax, $bx, $cx, $dx, $si, $di, $bp, $sp, $word, j$cxz, cwdq$, pushf$, popf$.
; [+] Добавлены макросы iforel, ifandel, iforelx, ifandelx (из полной редакции).
; [+] Добавлен псевдоним ifndo.
; [*] В макросы defdef и _defdef добавлен дополнительный параметр, позволяющий делать проверку ещё одной константы.
; [*] В макросы ?err, ?errif, ?errifx добавлен дополнительный параметр, позволяющий добавлять к сообщению 3 восклицательных знака.
; [-] Из данной редакции удалены макросы movt, movp, xchgp, calli, outsXXX, intfn.
; [-] Из данной редакции удалена проверка xmacro_ver_req (поскольку это могло явиться причиной сообщения об ошибке при включении обеих редакций).
;
; v1.00 (09.01.2017)
; [!] Самая первая версия.
;
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
ifndo		equ	<ifnel>

ifeel		equ	<ifnel>

; Если cond1, исполнить do1; иначе если cond2, исполнить do2; иначе do0
ifelif		macro	cond1:req, do1, cond2:req, do2, do0
	ifelifx	< cond1>, <do1>, < cond2>, <do2>, <do0>
endm

; Если cond1 или cond2 (условие cond2 проверяется только в том случае, если cond1 не выполнилось), исполнить do1; иначе do0
iforel		macro	cond1:req, cond2:req, do1, do0
	ifelifx	< cond1>, <do1>, < cond2>, <do1>, <do0>
endm

; Если cond1 и cond2 (условие cond2 проверяется только в том случае, если cond1 выполнилось), исполнить do1; иначе do0
ifandel		macro	cond1:req, cond2:req, do1, do0
	ifandelx < cond1>, < cond2>, <do1>, <do0>
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

; Аналогично iforel, но условия подставляются слитно с if
iforelx		macro	cond1:req, cond2:req, do1, do0
	ifelifx	<cond1>, <do1>, <cond2>, <do1>, <do0>
endm

; Аналогично ifandel, но условия подставляются слитно с if
ifandelx	macro	cond1:req, cond2:req, do1, do0
	if&cond1
	  if&cond2
		do1
	  else
		do0
	  endif
	else
		do0
	endif
endm

;-----------------------------------------------------------------------------------------------------------------------

; Если константа value1 определена, установить значение const = value1, иначе:
; а) если задано 2 параметра, то установить значение const = value2
; б) если задано 3 параметра, то если константа value2 определена, установить значение const = value2, иначе установить значение const = value3
defdef		macro	const:req, value1:req, value2:req, value3
  ifelifx	<def value1>, <const = value1>, <b <value3>>, <const = value2>, <ifx <def value2>, <const = value2>, <const = value3>>
endm

; Если константа const определена, установить значение _const = const, иначе:
; а) если задано 2 параметра, то установить значение _const = defval1
; б) если задано 3 параметра, то если константа defval1 определена, установить значение _const = defval1, иначе установить значение _const = defval2
_defdef		macro	const:req, defval1:req, defval2
	defdef	<_&const>, <const>, <defval1>, <defval2>
endm

;-----------------------------------------------------------------------------------------------------------------------

; Вывод сообщения txt об ошибке (одинаковый синтаксис для MASM и TASM)
; Если задан параметр exclam (любой), в конец сообщения добавляется ' !!!'
?err		macro	txt, exclam
	ifnb	<exclam>
	  if	isTASM
	    .err "&txt !!!"
	  else
	    .err <txt !!!!!!>
	  endif
	else
	  ifel	isTASM, <.err "&txt">, <.err <txt>>
	endif
endm

; Вывод сообщения txt об ошибке при выполнении условия cond
; Если exclam задан и не равен 0, в конец сообщения добавляется ' !!!'
?errif		macro	cond:req, txt, exclam
	ifx	< cond>, <?err <txt>, <exclam>>
endm

; Аналогично ?errif, но условие подставляется слитно с if, например: ?errifx <ndef ok>, <Error message>
; Если exclam задан и не равен 0, в конец сообщения добавляется ' !!!'
?errifx		macro	cond:req, txt, exclam
	ifx	<cond>, <?err <txt>, <exclam>>
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

; Определение идентификаторов регистров и инструкций, универсальных для 16 и 32-битных режимов.
if		@32Bit
  $ax		equ	<eax>
  $bx		equ	<ebx>
  $cx		equ	<ecx>
  $dx		equ	<edx>
  $si		equ	<esi>
  $di		equ	<edi>
  $bp		equ	<ebp>
  $sp		equ	<esp>
  $word		equ	<dword>
  j$cxz		equ	<jecxz>
  cwdq$		equ	<cdq>
  ; MASM в 32-битном режиме кодирует pushf/popf как pushfw/popfw (а вот pusha и popa - как положено: pushad, popad, поэтому для них идентификаторов не сделано)
  pushf$	equ	<pushfd>
  popf$		equ	<popfd>
else
  $ax		equ	<ax>
  $bx		equ	<bx>
  $cx		equ	<cx>
  $dx		equ	<dx>
  $si		equ	<si>
  $di		equ	<di>
  $bp		equ	<bp>
  $sp		equ	<sp>
  $word		equ	<word>
  j$cxz		equ	<jcxz>
  cwdq$		equ	<cwd>
  pushf$	equ	<pushf>
  popf$		equ	<popf>
endif ; @32Bit
; Включить режим оптимизации и использования локальных меток, начинающихся на @@
ifdo		isTASM,	<_ smart, locals>

; Проверка требуемой версии данного include-файла, заданной через константу xmacro_ver_req или xmac_ver_req
ifx		<def xmac_ver_req>, <?errif <xmac_ver_req gt xmac_ver>, <Version of 'xmac.inc' is less than required :(>>

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
	    ifdo <i eq n>, <pop r>
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
