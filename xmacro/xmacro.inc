;############################################
;##                                        ##
;##   -= EXTRA MACROSES (FULL EDITION) =-  ##
;##      ������� ��� MASM/TASM (16/32)     ##
;##             ����� ��ਠ��             ##
;##         [ v1.00 :: 09.01.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmacro_ver	=	100h			; ����� ������� 䠩�� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

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

; �믮������ ������権/��४⨢/����ᮢ, �������� ����� ��ப��
_		macro	p1, p2, p3, p4, p5, p6, p7, p8, p9, p10
	irp	x, <<p1>, <p2>, <p3>, <p4>, <p5>, <p6>, <p7>, <p8>, <p9>, <p10>>
		x
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------
;-- �������� ������� ---------------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; �᫨ cond, �ᯮ����� do1; ���� do0
ifel		macro	cond:req, do1, do0
	ifelx	< cond>, <do1>, <do0>
endm
ifdo		equ	<ifel>

ifn		equ	<ife>

; �᫨ �� cond, �ᯮ����� do1; ���� do0
ifnel		macro	cond:req, do1, do0
	ifelx	<e cond>, <do1>, <do0>
endm
ifeel		equ	<ifnel>

; �᫨ cond1, �ᯮ����� do1; ���� �᫨ cond2, �ᯮ����� do2; ���� do0
ifelif		macro	cond1:req, do1, cond2:req, do2, do0
	ifelifx	< cond1>, <do1>, < cond2>, <do2>, <do0>
endm

; ifdef id (�᫨ �����䨪��� ��।���), �ᯮ����� do1; ���� do0
ifdefel		macro	id:req, do1, do0
	ifelx	<def id>, <do1>, <do0>
endm

; ifndef id (�᫨ �����䨪��� �� ��।���), �ᯮ����� do1; ���� do0
ifndefel	macro	id:req, do1, do0
	ifelx	<ndef id>, <do1>, <do0>
endm

; ifb exp (�᫨ ��ࠦ���� ���⮥), �ᯮ����� do1; ���� do0
ifbel		macro	exp, do1, do0
	ifelx	<b <exp>>, <do1>, <do0>
endm

; �᫨ ��ࠦ���� exp ���⮥ ��� ��⮨� �� �஡����, �ᯮ����� do1; ���� do0
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

; ifnb exp (�᫨ ��ࠦ���� �����⮥), �ᯮ����� do1; ���� do0
ifnbel		macro	exp, do1, do0
	ifelx	<nb <exp>>, <do1>, <do0>
endm

; �᫨ ��ࠦ���� exp �����⮥ � �� ��⮨� �� �஡����, �ᯮ����� do1; ���� do0
ifnbbel		macro	exp, do1, do0
	ifbbel	<exp>, <do0>, <do1>
endm

; ifidn exp1, exp2 (�᫨ ���祭�� ᮢ������ � ���⮬ ॣ���� ᨬ�����), �ᯮ����� do1; ���� do0
ifidnel		macro	exp1:req, exp2:req, do1, do0
	ifelx	<idn <exp1>, <exp2>>, <do1>, <do0>
endm

; ifidni cond (�᫨ ���祭�� ᮢ������ ��� ���� ॣ���� ᨬ�����), �ᯮ����� do1; ���� do0
ifidniel	macro	exp1:req, exp2:req, do1, do0
	ifelx	<idni <exp1>, <exp2>>, <do1>, <do0>
endm

; ifidn exp1, exp2 (�᫨ ���祭�� ࠧ������� � ���⮬ ॣ���� ᨬ�����), �ᯮ����� do1; ���� do0
ifdifel		macro	exp1:req, exp2:req, do1, do0
	ifelx	<dif <exp1>, <exp2>>, <do1>, <do0>
endm

; ifidni cond (�᫨ ���祭�� ࠧ������� ��� ���� ॣ���� ᨬ�����), �ᯮ����� do1; ���� do0
ifdifiel	macro	exp1:req, exp2:req, do1, do0
	ifelx	<difi <exp1>, <exp2>>, <do1>, <do0>
endm

; �᫨ cond1 ��� cond2 (�᫮��� cond2 �஢������ ⮫쪮 � ⮬ ��砥, �᫨ cond1 �� �믮�������), �ᯮ����� do1; ���� do0
iforel		macro	cond1:req, cond2:req, do1, do0
	ifelifx	< cond1>, <do1>, < cond2>, <do1>, <do0>
endm

; �᫨ cond1 � cond2 (�᫮��� cond2 �஢������ ⮫쪮 � ⮬ ��砥, �᫨ cond1 �믮�������), �ᯮ����� do1; ���� do0
ifandel		macro	cond1:req, cond2:req, do1, do0
	ifandelx < cond1>, < cond2>, <do1>, <do0>
endm

; �������筮 ifel, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ifelx <def debug>, <int 3>, nop
ifelx		macro	cond:req, do1, do0
	if&cond
		do1
	else
		do0
	endif
endm
ifx		equ	<ifelx>

; �������筮 ifnel, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ifnelx <def debug>, nop, <int 3>
ifnelx		macro	cond:req, do1, do0
	ifelx	<cond>, <do0>, <do1>
endm
ifeelx		equ	<ifnelx>

; �������筮 ifelif, �� �᫮��� ����⠢������ ᫨⭮ � if, ���ਬ��: ifelifx <def debug>, <int 3>, <idn <spec>, <stosb>>, stosb, nop
ifelifx		macro	cond1:req, do1, cond2:req, do2, do0
	if&cond1
		do1
	elseif&cond2
		do2
	else
		do0
	endif
endm

; �������筮 iforel, �� �᫮��� ����⠢������ ᫨⭮ � if
iforelx		macro	cond1:req, cond2:req, do1, do0
	ifelifx	<cond1>, <do1>, <cond2>, <do1>, <do0>
endm

; �������筮 ifandel, �� �᫮��� ����⠢������ ᫨⭮ � if
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

; ��⠭����� ���祭�� var = true, �᫨ �믮������ ��� �� ���� �� �᫮��� cX; ���� ��⠭����� var = false
setifor		macro	var:req, c1:req, c2:req, c3, c4, c5, c6, c7, c8
	setiforx <var>, < c1>, < c2>, < c3>, < c4>, < c5>, < c6>, < c7>, < c8>
endm

; ��⠭����� ���祭�� var = true, �᫨ �믮������� �� �᫮��� cX; ���� ��⠭����� var = false
setifand	macro	var:req, c1:req, c2:req, c3, c4, c5, c6, c7, c8
	setifandx <var>, < c1>, < c2>, < c3>, < c4>, < c5>, < c6>, < c7>, < c8>
endm

; ��⠭����� ���祭�� var1 = true, � var2 = false, �᫨ �믮������ �᫮��� cond (����� ����⠢������ ᫨⭮ � if); ���� ��⠭����� var1 = false, � var2 = true
setifelx	macro	cond:req, var1, var2
	if&cond
	  ifnbel <var1>, <var1 = true>
	  ifnbel <var2>, <var2 = false>
	else
	  ifnbel <var1>, <var1 = false>
	  ifnbel <var2>, <var2 = true>
	endif
endm

; ��⠭����� ���祭�� var = true, �᫨ �믮������ ��� �� ���� �� �᫮��� cX (����� ����⠢������ ᫨⭮ � if); ���� ��⠭����� var = false
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

; ��⠭����� ���祭�� var = true, �᫨ �믮������� �� �᫮��� cX (����� ����⠢������ ᫨⭮ � if); ���� ��⠭����� var = false
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

; �᫨ �����䨪��� value ��।���, ��⠭����� ���祭�� const = value, ���� ��⠭����� ���祭�� _const = defval
defdef		macro	const:req, value:req, defval:req
  ifdef		value
	const	=	value
  else
	const	=	defval
  endif
endm

; �᫨ ����⠭� const ��।�����, ��⠭����� ���祭�� _const = const, ���� ��⠭����� ���祭�� _const = defval
_defdef		macro	const:req, defval:req
	defdef	<_&const>, <const>, <defval>
endm

;-----------------------------------------------------------------------------------------------------------------------

; �뢮� ᮮ�饭�� txt �� �訡�� (��������� ᨭ⠪�� ��� MASM � TASM)
?err		macro	txt
  ifel	isTASM, <.err "&txt">, <.err <&txt>>
endm

; �뢮� ᮮ�饭�� txt �� �訡�� �� �믮������ �᫮��� cond
?errif		macro	cond:req, txt
  ifx	< cond>, <?err <txt>>
endm

; �������筮 ?errif, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ?errifx <ndef ok>, <Error message>
?errifx		macro	cond:req, txt
  ifx	<cond>, <?err <txt>>
endm

;-----------------------------------------------------------------------------------------------------------------------

; ������騥 �����䨪���� ��।����� ��� ����� 㤮����� �ᯮ�짮����� ��������� �᫮���� ��४⨢ �������樨, ���ਬ��:
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

;=======================================================================================================================

;-----------------------------------------------------------------------------------------------------------------------
;-- ����������� �������� �������� --------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

false		=	0
true		=	not false

; ��⠭���� isTASM = true � isMASM = false, �᫨ ⥪�騩 ��������� - TASM, ���� �������
setifelx	<def ??Version>, isTASM, isMASM

; ��⠭���� �����䨪��� @32Bit (32-���� ०��) � MASM
ifdo		isMASM, <@32Bit = (@WordSize eq 4)>

; ����襭� �� ������樨/ॣ����� �����஢ (� TASM @Cpu = 0 � ०��� .586)
@186		=	(@Cpu and 2) or (@Cpu eq 0)
@286		=	(@Cpu and 3) or (@Cpu eq 0)
@386		=	(@Cpu and 8) or (@Cpu eq 0)
@486		=	(@Cpu and 10h) or (@Cpu eq 0)
@586		=	(@Cpu and 20h) or (@Cpu eq 0)
@686		=	@Cpu and 40h

; ������� ०�� ��⨬���樨 � �ᯮ�짮����� �������� ��⮪, ��稭������ �� @@
ifdo		isTASM,	<_ smart, locals>

; �஢�ઠ �ॡ㥬�� ���ᨨ ������� include-䠩��, �������� �१ ����⠭�� xmacro_ver_req ��� xmac_ver_req
ifx		<def xmacro_ver_req>, <?errif <xmacro_ver_req gt xmacro_ver>, <Version of 'xmacro.inc' is less than required :(>>
ifx		<def xmac_ver_req>, <?errif <xmac_ver_req gt xmacro_ver>, <Version of 'xmacro.inc' is less than required :(>>

;=======================================================================================================================

;-----------------------------------------------------------------------------------------------------------------------
;-- �������, ����������� ��� ���������� ������������� ���������� ���������� --------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; �����㬥��஢����� �������� salc (setalc)
salc		macro
		db	0D6h
endm
setalc		equ	<salc>

; �������� rdtsc
if		isTASM
  rdtsc		macro
		db	0Fh,31h
  endm
endif

;-----------------------------------------------------------------------------------------------------------------------

; ��릮� �१ 1 ����; �᫨ 㪠��� 1-���⮢� ॣ���� reg, ��� ���祭�� ���������, �� ��࠭����� 䫠��, ���� ���������� 䫠��, �� ��࠭����� ॣ�����
jmp_1		macro	reg
	local	x
	x	=	$
	ifnbel	reg, <mov reg,0>, <test al,0>
		org	$-1
	?errif <$-x ne 1>, <Wrong register '&reg' for this macro>
endm

; ��릮� �१ 2 ����; �᫨ 㪠��� 2-���⮢� ॣ���� reg, ��� ���祭�� ���������, �� ��࠭����� 䫠��, ���� ���������� 䫠��, �� ��࠭����� ॣ�����
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

; ��릮� �१ 4 ����; �᫨ 㪠��� 4-���⮢� ॣ���� reg, ��� ���祭�� ���������, �� ��࠭����� 䫠��, ���� ���������� 䫠��, �� ��࠭����� ॣ�����
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

; jmp far seg:ofs (�᫨ seg:ofs �� 㪠���, ���������� ⮫쪮 1 ���� ������)
jmpf		macro	seg, ofs
		db	0EAh
	ifnbel	<seg>, <dw ofs, seg>
endm

; call far seg:ofs (�᫨ seg:ofs �� 㪠���, ���������� ⮫쪮 1 ���� ������)
callf		macro	seg, ofs
		db	09Ah
	ifnbel	<seg>, <dw ofs, seg>
endm

; call int (pushf + callf)
calli		macro	seg, ofs
		pushf
		callf	seg, ofs
endm

; ��।���� nop'� � ������⢥ cnt ��� 
nops		macro	cnt:req
	rept	cnt
		nop
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------

; �믮������ ��᢮���� ���祭�� ॣ����� � ������� mov ��� xor (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (movx ax,%val), ���� xor �� �믮������
movx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <xor dst,dst>, <mov dst,src>
endm

; �믮������ �ࠢ����� ॣ���� � ������� cmp ��� test (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (cmpx ax,%val), ���� test �� �믮������
cmpx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <test dst,dst>, <cmp dst,src>
endm

; �믮������ ��᢮���� op = val �१ �⥪
movp		macro	op:req, val:req
		push	val
		pop	op
endm

; �믮������ ��᢮���� op = val �१ �६���� ॣ���� temp (�� 㬮�砭�� ax; ���ਬ��, movt ds,@data ��� movt es,0A000h,dx)
movt		macro	op:req, val:req, temp:=<ax>
		mov	temp,val
		pop	op2
endm

; ����� ���祭�ﬨ op1 � op2 �१ �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� ���࠭��� !!!
xchgp		macro	op1:req, op2:req
		push	op1
		push	op2
		pop	op1
		pop	op2
endm

; ���࠭���� ॣ���஢ � ���� ��ப�: push ax bx cx dx
pushx		macro	regs:vararg
	irp	r, <regs>
		push	r
	endm
endm

; ����⠭������� ॣ���஢ � ���� ��ப�: pop dx cx bx ax
popx		macro	regs:vararg
	irp	r, <regs>
		pop	r
	endm
endm

; ����⠭������� ॣ���஢ � ���⭮� ���浪�: popb ax bx cx dx
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

; ������ ip (���� ᫥���饩 ��ப�) � op, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� ���� � ���࠭�� !!!
getip		macro	op:req
	local	x
		call	x
	x:	pop	reg
endm

; ������ 䫠��� � op, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� 䫠��� � ���࠭�� !!!
getf		macro	op:req
		pushf
		pop	op
endm

; ������ ᫮�� 䫠��� � op, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� 䫠��� � ���࠭�� !!!
getfw		macro	op:req
		pushfw
		pop	$w op
endm

; ������ �������� ᫮�� 䫠��� � op, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� 䫠��� � ���࠭�� !!!
getfd		macro	op:req
		pushfd
		pop	$d op
endm

; ������ op � ॣ���� 䫠���, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� 䫠��� � ���࠭�� !!!
setf		macro	op:req
		push	op
		popf
endm

; ������ ᫮�� op � ॣ���� 䫠���, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� 䫠��� � ���࠭�� !!!
setfw		macro	op:req
		push	$w op
		popfw
endm

; ������ �������� ᫮�� op � ॣ���� 䫠���, �ᯮ���� �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� 䫠��� � ���࠭�� !!!
setfd		macro	op:req
		push	$d op
		popfd
endm

; ��⠭���� 䫠�� ZF=ZR=1 (����� � �� ��㣨� 䫠��: CF=0, PF=1, AF=0, SF=0, OF=0)
stz		macro
		cmp	al,al
endm

; ������� ��᮫�⭮� (������⥫쭮�) ���祭�� op (�������쭮� ����⥫쭮� ���祭�� ������� ���������, �� �⮬ SF=1, OF=1, ���� SF=0, SF=0)
absx		macro	op:req
	local	x
	x:	neg	op
		jl	x
endm

; ������� ��᮫�⭮� (������⥫쭮�) ���祭�� ॣ���� al (���祭�� -128 ������� ���������, �� �⮬ SF=1, OF=1, ���� SF=0, SF=0)
; ����⠥� ����॥, 祬 absx; �� �⮬: ah = 0 � CF=0, �᫨ ��室��� ���祭�� al ������⥫쭮�, ���� ah = -1, CF=1
absal		macro
		cbw
		xor	al,ah
		sub	al,ah
endm

; ������� ��᮫�⭮� (������⥫쭮�) ���祭�� ॣ���� ax (���祭�� -32768 ������� ���������, �� �⮬ SF=1, OF=1, ���� SF=0, SF=0)
; ����⠥� ����॥, 祬 absx; �� �⮬: dx = 0 � CF=0, �᫨ ��室��� ���祭�� ax ������⥫쭮�, ���� dx = -1, CF=1
absax		macro
		cwd
		xor	ax,dx
		sub	ax,dx
endm

; ������� ��᮫�⭮� (������⥫쭮�) ���祭�� ॣ���� ax (���祭�� -2147483648 ������� ���������, �� �⮬ SF=1, OF=1, ���� SF=0, SF=0)
; ����⠥� ����॥, 祬 absx; �� �⮬: edx = 0 � CF=0, �᫨ ��室��� ���祭�� eax ������⥫쭮�, ���� edx = -1, CF=1
abseax		macro
		cdq
		xor	eax,edx
		sub	eax,edx
endm

;-----------------------------------------------------------------------------------------------------------------------

; ����� ����� ��।���� ������⢮ ��㣨� ����ᮢ, ����� �믮����� lodsb/lodsw/lodsd � �ᯮ�짮������ ᥣ���⮢, �⫨��� �� ds
; lodsb_seg, lodsw_seg, lodsd_seg � ��������� ᥣ���⭮�� ॣ���� � ����⢥ ��㬥�� (���ਬ��, lodsb_seg es)
; lodsb_es, lodsb_cs, lodsb_ss, lodsb_fs, lodsb_gs,
; lodsw_es, lodsw_cs, lodsw_ss, lodsw_fs, lodsw_gs,
; lodsd_es, lodsd_cs, lodsd_ss, lodsd_fs, lodsd_gs ��� ��㬥�⮢
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

; ����� ����� ��।���� ������⢮ ��㣨� ����ᮢ, ����� �믮����� outsb/outsw/outsd � �ᯮ�짮������ ᥣ���⮢, �⫨��� �� ds
; outsb_seg, outsw_seg, outsd_seg � ��������� ᥣ���⭮�� ॣ���� � ����⢥ ��㬥�� (���ਬ��, outsb_seg es)
; outsb_es, outsb_cs, outsb_ss, outsb_fs, outsb_gs,
; outsw_es, outsw_cs, outsw_ss, outsw_fs, outsw_gs,
; outsd_es, outsd_cs, outsd_ss, outsd_fs, outsd_gs ��� ��㬥�⮢
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

; �맮� ���뢠��� no � ��⠭����� ॣ���஢ reg1 = val1, reg2 = val2...
; �᫨ 㪠��� ⮫쪮 reg1 ��� val1, � ���祭�� reg1 �������� � ah (��� ax, �᫨ �᫮ > 0FFh)
; ���ਬ��: intfn 16h,0 ��� intfn 15h,ah,86h,cx,0,dx,10000
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