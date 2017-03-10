;############################################
;##                                        ##
;##   -= EXTRA MACROSES (FULL EDITION) =-  ##
;##      МАКРОСЫ ДЛЯ MASM/TASM (16/32)     ##
;##             Полная редакция            ##
;##         [ v1.04 :: 10.03.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmacro_ver	=	104h			; версия данного файла (word: старший байт - целая часть, младший - дробная)

;------------------------------------------------------------------------------------------------------------------------
;
; -= ИСТОРИЯ ВЕРСИЙ =-
;
; v1.04 (10.03.2017)
; [!] Обнаружен баг, который появился вместе с макросами doifXXX в предыдущей версии (1.03), и я пока не знаю как его исправить (если знаете, напишите мне) !!!
;     MASM выдаёт ошибку при совместном использовании макросов doifXXX и _ с параметрами, содержащими запятые в угловых скобках (например, doif x, _ <mov ax,bx>), параметр
;     макроса _ разделяется на несколько, будто угловых скобок нет (данный пример работает аналогично doif x, _ mov ax,bx; т.е. макрос _ получает 2 параметра: mov ax и bx).
;     Пока я вижу только два варианта решения данной проблемы:
;     1. использовать макросы ifXXX, заключая также и макрос _ в угловые скобки (ifdo x, <_ <mov ax,bx>>) - это ЛУЧШИЙ ВАРИАНТ,
;     2. использовать двойные угловые скобки (doif x, _ <<mov ax,bx>>), однако этот вариант НЕ работает в TASM, что плохо, учитывая, что данный include-файл является универсальным для TASM и MASM.
;     p.s. Эта проблема проявляется у макросов doifXXX не только с макросом _, но и с другими, принимающими несколько параметров, которые могут содержать запятые (например, doif x, ?errif <hello, world>).
; [+] Добавлены макросы ifnsetel и ifseteel (в дополнение к ifsetel) и псевдонимы для всех 3-х макросов: ifset, ifnset и ifsete.
; [+] Добавлен макрос fastnops и константа fastnopsprefix, а макрос aligncode получил дополнительный параметр prefix и использует (так же, как и alignc и aligncs) nop'ы с префиксами.
; [*] Исправлена ошибка в макросе ifsetel, возникающая при использовании угловых скобок в do1 и/или do0.
; [*] Макрос _ расширен до 16 параметров.
; [*] Макрос ifelif переписан без использования вложенных макросов, в других макросах doif-подобные макросы заменены на ifdo-подобные.
;
; v1.03 (08.03.2017)
; [+] Добавлены макросы doif, doifn и doifx, работающие аналогично ifdo, ifndo и ifx, но позволяющие указывать параметр do без <угловых скобок> (ifdo, ifndo и ifx сохранены для обратной совместимости).
; [+] Добавлены макросы doifset, doifnset, doifsete и ifsetel, проверяющие и определение идентификатора, и его значение.
; [+] Добавлены макросы aligndata, aligncode, alignd, alignc, alignds, aligncs.
; [+] Добавлен идентификатор dw$, определяющий dw или dd в зависимости от разрядности кода.
; [+] Добавлены идентификаторы proto = procdesc и invoke = call для TASM.
; [+] По умолчанию данный include-файл включает директивы locals и smart для TASM; если нужно их отключить, установите константу xnodir = 1 ДО включения файла.
; [+] Требуемую версию данного include-файла можно теперь проверять не только через константу xmacro_ver_req, но и через макрос check_xmacro_ver (после подключения файла), что исключает возможность опечатки.
; [*] Сделаны некоторые перестановки и изменения в макросах (например, ifel переписан без использования вложенных макросов, что должно увеличить скорость и упростить обнаружение ошибок в коде).
;
; v1.02 (01.02.2017)
; [+] Добавлен макрос loopx, генерирующий dec + jnz (jns).
; [*] В макросе movt значение параметра temp по умолчанию изменено с ax на $ax.
; [*] В макросе ?err для добавления ' !!!' в конец сообщения параметр exclam должен быть <> 0 (ранее достаточно было задать любое значение, в т.ч. 0).
;
; v1.01 (26.01.2017)
; [+] Добавлены идентификаторы $ax, $bx, $cx, $dx, $si, $di, $bp, $sp, $word, j$cxz, cwdq$, pushf$, popf$.
; [+] Добавлен псевдоним ifndo.
; [*] В макросы defdef и _defdef добавлен дополнительный параметр, позволяющий делать проверку ещё одной константы.
; [*] В макросы ?err, ?errif, ?errifx добавлен дополнительный параметр, позволяющий добавлять к сообщению 3 восклицательных знака.
; [*] Исправлен баг в макросе movt.
;
; v1.00 (09.01.2017)
; [!] Самая первая версия.
;
;-----------------------------------------------------------------------------------------------------------------------

@		equ	<offset>
$b		equ	<byte ptr>
$w		equ	<word ptr>
$d		equ	<dword ptr>
$f		equ	<fword ptr>
$q		equ	<qword ptr>
$t		equ	<tbyte ptr>
$s		equ	<short>
$n		equ	<near>
$f		equ	<far>

; Выполнение инструкций/директив/макросов, заданных одной строкой
_		macro	p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16
	irp	x, <<p1>, <p2>, <p3>, <p4>, <p5>, <p6>, <p7>, <p8>, <p9>, <p10>, <p11>, <p12>, <p13>, <p14>, <p15>, <p16>>
		x
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------
;-- УСЛОВНЫЕ МАКРОСЫ ---------------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; Если cond, исполнить do1; иначе do0
ifel		macro	cond:req, do1, do0
	if	cond
		do1
	else
		do0
	endif
endm
ifdo		equ	<ifel>

; Если НЕ cond, исполнить do1; иначе do0
ifnel		macro	cond:req, do1, do0
	ifelx	<e cond>, <do1>, <do0>
endm
ifeel		equ	<ifnel>
ifndo		equ	<ifnel>

ifn		equ	<ife>

; Аналогично ifel, но условие подставляется слитно с if, например: ifelx <def debug>, <int 3>, nop
ifelx		macro	cond:req, do1, do0
	if&cond
		do1
	else
		do0
	endif
endm
ifx		equ	<ifelx>

;-= ВАЖНЫЕ КОНСТАНТЫ -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
false		=	0
true		=	not false

; Установка isTASM = true и isMASM = false, если текущий компилятор - TASM, иначе наоборот
ifelx		<def ??Version>, <_ <isTASM = true>, <isMASM = false>>, <_ <isMASM = true>, <isTASM = false>>
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

if		isTASM
; Если cond, исполнить do (do ненужно заключать в <>)
  doif		macro	cond:req, do:rest
	if	cond
		do
	endif
  endm

; Если НЕ cond, исполнить do (do ненужно заключать в <>)
  doifn		macro	cond:req, do:rest
	doifx	<e cond>, do
  endm
; Аналогично doif, но условие подставляется слитно с if, например: doifx <def debug>, int 3 (do ненужно заключать в <>)
  doifx		macro	cond:req, do:rest
	if&cond
		do
	endif
  endm
else ; аналогичные макросы для MASM
  doif		macro	cond:req, do:vararg
	if	cond
		do
	endif
  endm

  doifn		macro	cond:req, do:vararg
	doifx	<e cond>, do
  endm

  doifx		macro	cond:req, do:vararg
	if&cond
		do
	endif
  endm
endif ; else/if isTASM

; Аналогично ifnel, но условие подставляется слитно с if, например: ifnelx <def debug>, nop, <int 3>
ifnelx		macro	cond:req, do1, do0
	ifelx	<cond>, <do0>, <do1>
endm
ifeelx		equ	<ifnelx>

; Если cond1, исполнить do1; иначе если cond2, исполнить do2; иначе do0
ifelif		macro	cond1:req, do1, cond2:req, do2, do0
	if	cond1
		do1
	elseif	cond2
		do2
	else
		do0
	endif
endm

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

; ifdef id (если идентификатор определён), исполнить do1; иначе do0
ifdefel		macro	id:req, do1, do0
	ifelx	<def id>, <do1>, <do0>
endm

; ifndef id (если идентификатор НЕ определён), исполнить do1; иначе do0
ifndefel	macro	id:req, do1, do0
	ifelx	<ndef id>, <do1>, <do0>
endm

; ifb exp (если выражение пустое), исполнить do1; иначе do0
ifbel		macro	exp, do1, do0
	ifelx	<b <exp>>, <do1>, <do0>
endm

; Если выражение exp пустое или состоит из пробелов, исполнить do1; иначе do0
ifbbel		macro	exp, do1, do0
	local	temp
	ifb	<exp>
		do1
	else
	  temp	= true
	  irpc	x, <exp>
	    ifdif <x>, < >
	      ifdif <x>, <	>
		temp = false
		exitm
	      endif
	    endif
	  endm
	  ifel	temp, <do1>, <do0>
	endif
endm

; ifnb exp (если выражение НЕпустое), исполнить do1; иначе do0
ifnbel		macro	exp, do1, do0
	ifelx	<nb <exp>>, <do1>, <do0>
endm

; Если выражение exp НЕпустое и НЕ состоит из пробелов, исполнить do1; иначе do0
ifnbbel		macro	exp, do1, do0
	ifbbel	<exp>, <do0>, <do1>
endm

; ifidn exp1, exp2 (если значения совпадают С учётом регистра символов), исполнить do1; иначе do0
ifidnel		macro	exp1:req, exp2:req, do1, do0
	ifelx	<idn <exp1>, <exp2>>, <do1>, <do0>
endm

; ifidni cond (если значения совпадают БЕЗ учёта регистра символов), исполнить do1; иначе do0
ifidniel	macro	exp1:req, exp2:req, do1, do0
	ifelx	<idni <exp1>, <exp2>>, <do1>, <do0>
endm

; ifidn exp1, exp2 (если значения различаются С учётом регистра символов), исполнить do1; иначе do0
ifdifel		macro	exp1:req, exp2:req, do1, do0
	ifelx	<dif <exp1>, <exp2>>, <do1>, <do0>
endm

; ifidni cond (если значения различаются БЕЗ учёта регистра символов), исполнить do1; иначе do0
ifdifiel	macro	exp1:req, exp2:req, do1, do0
	ifelx	<difi <exp1>, <exp2>>, <do1>, <do0>
endm

; Если cond1 или cond2 (условие cond2 проверяется только в том случае, если cond1 не выполнилось), исполнить do1; иначе do0
iforel		macro	cond1:req, cond2:req, do1, do0
	ifelif	<cond1>, <do1>, <cond2>, <do1>, <do0>
endm

; Если cond1 и cond2 (условие cond2 проверяется только в том случае, если cond1 выполнилось), исполнить do1; иначе do0
ifandel		macro	cond1:req, cond2:req, do1, do0
	ifandelx < cond1>, < cond2>, <do1>, <do0>
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

if		isTASM
; Если id определён и НЕ равен 0, выполнить do (do ненужно заключать в <>)
  doifset	macro	id:req, do:rest
	ifdef	id
	  if	id
		do
	  endif
	endif
  endm

; Если id НЕопределён или равен 0, выполнить do (do ненужно заключать в <>)
  doifnset	macro	id:req, do:rest
	ifndef	id
		do
	elseife	id
		do
	endif
  endm

; Если id определён и равен 0, выполнить do (do ненужно заключать в <>)
  doifsete	macro	id:req, do:rest
	ifdef	id
	  ife	id
		do
	  endif
	endif
  endm
else ; аналогичные макросы для MASM
  doifset	macro	id:req, do:vararg
	ifdef	id
	  if	id
		do
	  endif
	endif
  endm

  doifnset	macro	id:req, do:vararg
	ifndef	id
		do
	elseife	id
		do
	endif
  endm

  doifsete	macro	id:req, do:vararg
	ifdef	id
	  ife	id
		do
	  endif
	endif
  endm
endif ; else/if isTASM

; Если id определён и НЕ равен 0, выполнить do1, иначе do0
ifsetel		macro	id:req, do1, do0
	ifandelx <def id>, < id>, <do1>, <do0>
endm
ifset		equ	<ifsetel>

; Если id НЕопределён или равен 0, выполнить do1, иначе do0
ifnsetel	macro	id:req, do1, do0
	iforelx <ndef id>, <e id>, <do1>, <do0>
endm
ifnset		equ	<ifnsetel>

; Если id определён и равен 0, выполнить do1, иначе do0
ifseteel	macro	id:req, do1, do0
	ifandelx <def id>, <e id>, <do1>, <do0>
endm
ifsete		equ	<ifseteel>

;-----------------------------------------------------------------------------------------------------------------------

; Установить значение var = true, если выполняется хотя бы одно из условий cX; иначе установить var = false
setifor		macro	var:req, c1:req, c2:req, c3, c4, c5, c6, c7, c8
	setiforx <var>, < c1>, < c2>, < c3>, < c4>, < c5>, < c6>, < c7>, < c8>
endm

; Установить значение var = true, если выполняются все условия cX; иначе установить var = false
setifand	macro	var:req, c1:req, c2:req, c3, c4, c5, c6, c7, c8
	setifandx <var>, < c1>, < c2>, < c3>, < c4>, < c5>, < c6>, < c7>, < c8>
endm

; Установить значение var1 = true, а var2 = false, если выполняется условие cond (которые подставляются слитно с if); иначе установить var1 = false, а var2 = true
setifelx	macro	cond:req, var1, var2
	if&cond
	  ifnbel <var1>, <var1 = true>
	  ifnbel <var2>, <var2 = false>
	else
	  ifnbel <var1>, <var1 = false>
	  ifnbel <var2>, <var2 = true>
	endif
endm

; Установить значение var = true, если выполняется хотя бы одно из условий cX (которые подставляются слитно с if); иначе установить var = false
setiforx	macro	var:req, c1:req, c2:req, c3, c4, c5, c6, c7, c8
	local	temp
	temp	=	false
	irp	x, <<c1>, <c2>, <c3>, <c4>, <c5>, <c6>, <c7>, <c8>>
	  ifnbbel <x>, <ifdo <x>, <temp = true>>
	  if	temp
	    exitm
	  endif
	endm
	var	=	temp
endm

; Установить значение var = true, если выполняются все условия cX (которые подставляются слитно с if); иначе установить var = false
setifandx	macro	var:req, c1:req, c2:req, c3, c4, c5, c6, c7, c8
	local	temp1, temp2
	temp1	=	0
	temp2	=	0
	irp	x, <<c1>, <c2>, <c3>, <c4>, <c5>, <c6>, <c7>, <c8>>
	  ifnbbel <x>, <_ <temp1 = temp1 + 1>, <ifx <x>, <temp2 = temp2 + 1>>>
	endm
	var	=	temp1 eq temp2
endm

;-----------------------------------------------------------------------------------------------------------------------

; Вывод сообщения txt об ошибке (одинаковый синтаксис для MASM и TASM)
; Если параметр exclam задан и не равен 0, в конец сообщения добавляется ' !!!'
?err		macro	txt, exclam:=<0>
	if	exclam
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
	ifdo	<cond>, <?err <txt>, <exclam>>
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
;    ??if ghi
;     ...
;    ??endif
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

??if		equ	<if>
??ifdef		equ	<ifdef>
??ifndef	equ	<ifndef>
??ifb		equ	<ifb>
??ifnb		equ	<ifnb>
??ifidn		equ	<ifidn>
??ifidni	equ	<ifidni>
??ifdif		equ	<ifdif>
??ifdifi	equ	<ifdifi>
??if1		equ	<if1>
??if2		equ	<if2>
??else		equ	<else>
??elseif	equ	<elseif>
??elseifdef	equ	<elseifdef>
??elseifndef	equ	<elseifndef>
??elseifb	equ	<elseifb>
??elseifnb	equ	<elseifnb>
??elseifidn	equ	<elseifidn>
??elseifidni	equ	<elseifidni>
??elseifdif	equ	<elseifdif>
??elseifdifi	equ	<elseifdifi>
??elseif1	equ	<elseif1>
??elseif2	equ	<elseif2>
??endif		equ	<endif>

;-----------------------------------------------------------------------------------------------------------------------
;-- ОПРЕДЕЛЕНИЕ ЗНАЧЕНИЙ КОНСТАНТ --------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; Установка идентификатора @32Bit (32-битный режим) в MASM
ifdo		isMASM, <@32Bit = (@WordSize eq 4)>

; Разрешены ли инструкции/регистры процессоров (в TASM @Cpu = 0 в режиме .586)
@186		=	(@Cpu and 2) or (@Cpu eq 0)
@286		=	(@Cpu and 4) or (@Cpu eq 0)
@386		=	(@Cpu and 8) or (@Cpu eq 0)
@486		=	(@Cpu and 10h) or (@Cpu eq 0)
@586		=	(@Cpu and 20h) or (@Cpu eq 0)
@686		=	@Cpu and 40h

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
  dw$		equ	<dd>
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
  dw$		equ	<dw>
endif ; @32Bit

; Директивы для TASM из MASM
ifdo		isTASM, <proto equ <procdesc>>
ifdo		isTASM, <invoke equ <call>>

; Опкод префикса, используемого по умолчанию для последовательности nop'ов (90h=nop - без префикса)
; [можно использовать: 3Eh=ds, 2Eh=cs, 26h=es, 36h=ss, 0F2h=repne, 386+: 66h=operand resize, 67h=address resize, 64h=fs, 65h=gs; но только не 0F3h=repe, 0F0h=lock или иное значение !!!]
ifel		@386, <fastnopsprefix = 66h>, <fastnopsprefix = 3Eh>

; Включить режим оптимизации и использования локальных меток, начинающихся на @@
ifnset		xnodir, <ifdo isTASM, <_ smart, locals>>

; Проверка требуемой версии данного include-файла, заданной параметром ver
check_xmacro_ver macro	ver:req
	?errif <ver gt xmacro_ver>, <Version of 'xmac.inc' is less than required>, true
endm
check_xmac_ver	equ	<check_xmacro_ver>

; Проверка требуемой версии данного include-файла, заданной через константы xmacro_ver_req и xmac_ver_req
ifx		<def xmacro_ver_req>, <check_xmacro_ver xmacro_ver_req>
ifx		<def xmac_ver_req>, <check_xmacro_ver xmac_ver_req>

;-----------------------------------------------------------------------------------------------------------------------
;-- ВСПОМОГАТЕЛЬНЫЕ МАКРОСЫ --------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; Выравнивание данных по адресу, кратному value (со сдвигом на shift) относительно позиции rel, заполняя пространство символами chr
aligndata	macro	value:=<@WordSize>, shift:=<0>, rel:=<0>, chr:=<?>
	local	x, y, z
	x	=	$
		org	rel
	y:	org	x
	z	=	(shift mod value + value) mod value
	while	((($-y) mod value + value) mod value) ne z
		db	chr
	endm
endm

; Выравнивание кода по адресу, кратному value (со сдвигом на shift) относительно позиции rel, заполняя пространство nop'ами с префиксом fastnopsprefix (см. fastnops)
aligncode	macro	value:=<@WordSize>, shift:=<0>, rel:=<0>, prefix:=<fastnopsprefix>
	local	x, y, z, n
	x	=	$
		org	rel
	y:	org	x
	z	=	(shift mod value + value) mod value
	n	=	0
	while	((($-y) mod value + value) mod value) ne z
		n = n+1
	  ifel <((n mod 15) eq 0)>, nop, <db prefix>
	endm
	ifdo	<n gt 0>, <_ <org $-1>, nop>
endm

if	isTASM
; Выравнивание данных по адресу, кратному value с выполнением do (do ненужно заключать в <>)
  alignd	macro	value:=<@WordSize>, do:rest
	aligndata	value
	do
  endm

; Выравнивание кода по адресу, кратному value с выполнением do (do ненужно заключать в <>)
  alignc	macro	value:=<@WordSize>, do:rest
	aligncode	value
	do
  endm

; Выравнивание данных по адресу, кратному value со сдвигом на shift с выполнением do (do ненужно заключать в <>)
  alignds	macro	value:=<@WordSize>, shift:=<0>, do:rest
	aligndata	value, shift
	do
  endm

; Выравнивание кода по адресу, кратному value со сдвигом на shift с выполнением do (do ненужно заключать в <>)
  aligncs	macro	value:=<@WordSize>, shift:=<0>, do:rest
	aligncode	value, shift
	do
  endm
else ; аналогичные макросы для MASM
  alignd	macro	value:=<@WordSize>, do:vararg
	aligndata	value
	do
  endm

  alignc	macro	value:=<@WordSize>, do:vararg
	aligncode	value
	do
  endm

  alignds	macro	value:=<@WordSize>, shift:=<0>, do:vararg
	aligndata	value, shift
	do
  endm

  aligncs	macro	value:=<@WordSize>, shift:=<0>, do:vararg
	aligncode	value, shift
	do
  endm
endif ; else/if isTASM

; Если константа value1 определена, установить значение const = value1, иначе:
; а) если задано 2 параметра, то установить значение const = value2
; б) если задано 3 параметра, то если константа value2 определена, установить значение const = value2, иначе установить значение const = value3
defdef		macro	const:req, value1:req, value2:req, value3
	ifelifx	<def value1>, <const = value1>, <b <value3>>, <const = value2>, <ifelx <def value2>, <const = value2>, <const = value3>>
endm

; Если константа const определена, установить значение _const = const, иначе:
; а) если задано 2 параметра, то установить значение _const = defval1
; б) если задано 3 параметра, то если константа defval1 определена, установить значение _const = defval1, иначе установить значение _const = defval2
_defdef		macro	const:req, defval1:req, defval2
	defdef	<_&const>, <const>, <defval1>, <defval2>
endm

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
	ifnbel	<seg>, <dw ofs, seg>
endm

; call far seg:ofs (если seg:ofs не указан, генерируется только 1 байт опкода)
callf		macro	seg, ofs
		db	09Ah
	ifnbel	<seg>, <dw ofs, seg>
endm

; call int (pushf + callf)
calli		macro	seg, ofs
		pushf
		callf	seg, ofs
endm

; Определяет nop'ы в количестве cnt штук 
nops		macro	cnt:req
	rept	cnt
		nop
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------

; Выполнение присвоения значения регистру с помощью mov или xor (если src = 0)
; Чтобы передать в параметре src константу, необходимо поставить перед ней % (movx ax,%val), иначе вместо xor будет использован mov
movx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <xor dst,dst>, <mov dst,src>
endm

; Выполнение сравнение регистра с помощью cmp или test (если src = 0)
; Чтобы передать в параметре src константу, необходимо поставить перед ней % (cmpx ax,%val), иначе вместо test будет использован cmp
cmpx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <test dst,dst>, <cmp dst,src>
endm

; Выполнение присвоения op = val через временный регистр temp (по умолчанию $ax; например, movt ds,@data или movt es,0A000h,dx)
movt		macro	op:req, val:req, temp:=<$ax>
		movx	temp,val
		mov	op,temp
endm

; Выполнение присвоения op = val через стек
movp		macro	op:req, val:req
		push	val
		pop	op
endm

; Обмен значениями op1 и op2 через стек
; Макрос не проверяет соответствие разрядности операндов !!!
xchgp		macro	op1:req, op2:req
		push	op1
		push	op2
		pop	op1
		pop	op2
endm

; Замена loop парой инструкций dec+jnz (или dec+jns, если plusone<>0)
; Если указан регистр reg, то вместо $cx используется именно он.
loopx		macro	lab:req, reg:=<$cx>, plusone:=<0>
		dec	reg
	ifel	plusone, <jns lab>, <jnz lab>
endm

; Сохранение регистров в одну строку: push ax bx cx dx
pushx		macro	regs:vararg
	irp	r, <regs>
		push	r
	endm
endm

; Восстановление регистров в одну строку: pop dx cx bx ax
popx		macro	regs:vararg
	irp	r, <regs>
		pop	r
	endm
endm

; Восстановление регистров в обратном порядке: popb ax bx cx dx
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

; Запись ip (адрес следующей строки) в op, используя стек
; Макрос не проверяет соответствие разрядности кода и операнда !!!
getip		macro	op:req
	local	x
		call	x
	x:	pop	reg
endm

; Запись флагов в op, используя стек
; Макрос не проверяет соответствие разрядности флагов и операнда !!!
getf		macro	op:req
		pushf
		pop	op
endm

; Запись слова флагов в op, используя стек
; Макрос не проверяет соответствие разрядности флагов и операнда !!!
getfw		macro	op:req
		pushfw
		pop	$w op
endm

; Запись двойного слова флагов в op, используя стек
; Макрос не проверяет соответствие разрядности флагов и операнда !!!
getfd		macro	op:req
		pushfd
		pop	$d op
endm

; Запись op в регистр флагов, используя стек
; Макрос не проверяет соответствие разрядности флагов и операнда !!!
setf		macro	op:req
		push	op
		popf
endm

; Запись слова op в регистр флагов, используя стек
; Макрос не проверяет соответствие разрядности флагов и операнда !!!
setfw		macro	op:req
		push	$w op
		popfw
endm

; Запись двойного слова op в регистр флагов, используя стек
; Макрос не проверяет соответствие разрядности флагов и операнда !!!
setfd		macro	op:req
		push	$d op
		popfd
endm

; Установка флага ZF=ZR=1 (влияет и на другие флаги: CF=0, PF=1, AF=0, SF=0, OF=0)
stz		macro
		cmp	al,al
endm

; Получить абсолютное (положительное) значение op (минимальное отрицательное значение остаётся неизменным, при этом SF=1, OF=1, иначе SF=0, SF=0)
absx		macro	op:req
	local	x
	x:	neg	op
		jl	x
endm

; Получить абсолютное (положительное) значение регистра al (значение -128 остаётся неизменным, при этом SF=1, OF=1, иначе SF=0, SF=0)
; Работает быстрее, чем absx; при этом: ah = 0 и CF=0, если исходное значение al положительное, иначе ah = -1, CF=1
absal		macro
		cbw
		xor	al,ah
		sub	al,ah
endm

; Получить абсолютное (положительное) значение регистра ax (значение -32768 остаётся неизменным, при этом SF=1, OF=1, иначе SF=0, SF=0)
; Работает быстрее, чем absx; при этом: dx = 0 и CF=0, если исходное значение ax положительное, иначе dx = -1, CF=1
absax		macro
		cwd
		xor	ax,dx
		sub	ax,dx
endm

; Получить абсолютное (положительное) значение регистра ax (значение -2147483648 остаётся неизменным, при этом SF=1, OF=1, иначе SF=0, SF=0)
; Работает быстрее, чем absx; при этом: edx = 0 и CF=0, если исходное значение eax положительное, иначе edx = -1, CF=1
abseax		macro
		cdq
		xor	eax,edx
		sub	eax,edx
endm

; Сгенерировать nop'ы в количестве num штук, разбавленные префиксом prefix (по умолчанию fastnopsprefix, которому изначально присваивается значение 3Eh = ds)
; Выполняется значительно быстрее, чем обычная последовательность nop'ов; максимально допустимый размер инструкции (15 байт) не превышается
fastnops	macro	num:req, prefix:=<fastnopsprefix>
	local	n
		n = 0
	rept	num
		n = n + 1
	  ifel <((n mod 15) eq 0) or (n eq num)>, nop, <db prefix>
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------

; Данный макрос определяет множество других макросов, которые выполняют lodsb/lodsw/lodsd с использованием сегментов, отличных от ds
; lodsb_seg, lodsw_seg, lodsd_seg с названием сегментного регистра в качестве аргумента (например, lodsb_seg es)
; lodsb_es, lodsb_cs, lodsb_ss, lodsb_fs, lodsb_gs,
; lodsw_es, lodsw_cs, lodsw_ss, lodsw_fs, lodsw_gs,
; lodsd_es, lodsd_cs, lodsd_ss, lodsd_fs, lodsd_gs без аргументов
irp		x, <byte, word, dword>
  local	t, m1, m2
  t	substr	<x>,1,1
  m1	catstr	<lods>,t,<_seg>
  m1	macro	seg:req
	lods x ptr seg:[si]
  endm
  irp		s, <es, cs, ss, fs, gs>
    m2	catstr	<lods>,t,<_>,<s>
    m2	macro
	lods x ptr s:[si]
    endm
  endm
endm

; Данный макрос определяет множество других макросов, которые выполняют outsb/outsw/outsd с использованием сегментов, отличных от ds
; outsb_seg, outsw_seg, outsd_seg с названием сегментного регистра в качестве аргумента (например, outsb_seg es)
; outsb_es, outsb_cs, outsb_ss, outsb_fs, outsb_gs,
; outsw_es, outsw_cs, outsw_ss, outsw_fs, outsw_gs,
; outsd_es, outsd_cs, outsd_ss, outsd_fs, outsd_gs без аргументов
irp		x, <byte, word, dword>
  local	t, m1, m2
  t	substr	<x>,1,1
  m1	catstr	<outs>,t,<_seg>
  m1	macro	seg:req
	outs x ptr seg:[si]
  endm
  irp		s, <es, cs, ss, fs, gs>
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
	ifnbel	<reg2>, <movx reg2,val2>
	ifnbel	<reg3>, <movx reg3,val3>
	ifnbel	<reg4>, <movx reg4,val4>
	ifnbel	<reg5>, <movx reg5,val5>
		int	no
endm
