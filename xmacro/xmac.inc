;############################################
;##                                        ##
;##   -= EXTRA MACROSES (MINI EDITION) =-  ##
;##      ������� ��� MASM/TASM (16/32)     ##
;##           ������� ��ਠ��          ##
;##         [ v1.00 :: 09.01.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmac_ver	=	100h			; ����� ������� 䠩�� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

;-----------------------------------------------------------------------------------------------------------------------

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

; �������筮 ifel, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ifelx <def debug>, <int 3>, nop
ifelx		macro	cond:req, do1, do0
	if&cond
		do1
	else
		do0
	endif
endm
ifx		equ	<ifelx>

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
;-- ����������� �������� �������� --------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

false		=	0
true		=	not false

; ��⠭���� isTASM = true � isMASM = false, �᫨ ⥪�騩 ��������� - TASM, ���� �������
ifelx		<def ??Version>, <_ <isTASM = true>, <isMASM = false>>, <_ <isMASM = true>, <isTASM = false>>

; ��⠭���� �����䨪��� @32Bit (32-���� ०��) � MASM
ifdo		isMASM, <@32Bit = (@WordSize eq 4)>

; ����襭� �� ������樨/ॣ����� ������ 80386
@386		=	(@Cpu and 8) or (@Cpu eq 0)	; � TASM @Cpu = 0 � ०��� .586

; ������� ०�� ��⨬���樨 � �ᯮ�짮����� �������� ��⮪, ��稭������ �� @@
ifdo		isTASM,	<_ smart, locals>

; �஢�ઠ �ॡ㥬�� ���ᨨ ������� include-䠩��, �������� �१ ����⠭�� xmacro_ver_req ��� xmac_ver_req
ifx		<def xmac_ver_req>, <?errif <xmac_ver_req gt xmac_ver>, <Version of 'xmac.inc' is less than required :(>>
?errifx		<def xmacro_ver_req>, <Full edition of 'xmacro.inc' is required :(>

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
	ifelx	<nb <seg>>, <dw ofs, seg>
endm

; call far seg:ofs (�᫨ seg:ofs �� 㪠���, ���������� ⮫쪮 1 ���� ������)
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

; �믮������ ��᢮���� ���祭�� ॣ����� � ������� mov ��� xor (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (movx ax,%val), ���� xor �� �믮������
movx		macro	dst:req, src:req
	ifelx	<idn <src>, <0>>, <xor dst,dst>, <mov dst,src>
endm

; �믮������ �ࠢ����� ॣ���� � ������� cmp ��� test (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (cmpx ax,%val), ���� test �� �믮������
cmpx		macro	dst:req, src:req
	ifelx	<idn <src>, <0>>, <test dst,dst>, <cmp dst,src>
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

; ���࠭���� ॣ���஢ � ���� ��ப�: push ax, bx, cx, dx
pushx		macro	regs:vararg
	irp	r, <regs>
		push	r
	endm
endm

; ����⠭������� ॣ���஢ � ���� ��ப�: pop dx, cx, bx, ax
popx		macro	regs:vararg
	irp	r, <regs>
		pop	r
	endm
endm

; ����⠭������� ॣ���஢ � ���⭮� ���浪�: popb ax, bx, cx, dx
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

; ����� ����� ��।���� ������⢮ ��㣨� ����ᮢ, ����� �믮����� lodsb/lodsw/lodsd � �ᯮ�짮������ ᥣ���⮢, �⫨��� �� ds
; lodsb_seg, lodsw_seg, lodsd_seg � ��������� ᥣ���⭮�� ॣ���� � ����⢥ ��㬥�� (���ਬ��, lodsb_seg es)
; lodsb_es, lodsb_cs, lodsb_ss
; lodsw_es, lodsw_cs, lodsw_ss
; lodsd_es, lodsd_cs, lodsd_ss ��� ��㬥�⮢
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

; ����� ����� ��।���� ������⢮ ��㣨� ����ᮢ, ����� �믮����� outsb/outsw/outsd � �ᯮ�짮������ ᥣ���⮢, �⫨��� �� ds
; outsb_seg, outsw_seg, outsd_seg � ��������� ᥣ���⭮�� ॣ���� � ����⢥ ��㬥�� (���ਬ��, outsb_seg es)
; outsb_es, outsb_cs, outsb_ss,
; outsw_es, outsw_cs, outsw_ss,
; outsd_es, outsd_cs, outsd_ss ��� ��㬥�⮢
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

; �맮� ���뢠��� no � ��⠭����� ॣ���஢ reg1 = val1, reg2 = val2...
; �᫨ 㪠��� ⮫쪮 reg1 ��� val1, � ���祭�� reg1 �������� � ah (��� ax, �᫨ �᫮ > 0FFh)
; ���ਬ��: intfn 16h,0 ��� intfn 15h,ah,86h,cx,0,dx,10000
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