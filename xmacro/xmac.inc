;############################################
;##                                        ##
;##   -= EXTRA MACROSES (MINI EDITION) =-  ##
;##      ������� ��� MASM/TASM (16/32)     ##
;##          �����񭭠� ।����          ##
;##         [ v1.04 :: 10.03.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmac_ver	=	104h			; ����� ������� 䠩�� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

;------------------------------------------------------------------------------------------------------------------------
;
; -= ������� ������ =-
;
; v1.04 (10.03.2017)
; [!] �����㦥� ���, ����� ����� ����� � ����ᠬ� doifXXX � �।��饩 ���ᨨ (1.03), � � ���� �� ���� ��� ��� ��ࠢ��� (�᫨ �����, ������ ���) !!!
;     MASM �뤠�� �訡�� �� ᮢ���⭮� �ᯮ�짮����� ����ᮢ doifXXX � _ � ��ࠬ��ࠬ�, ᮤ�ঠ騬� ������ � 㣫���� ᪮���� (���ਬ��, doif x, _ <mov ax,bx>), ��ࠬ���
;     ����� _ ࠧ������� �� ��᪮�쪮, ��� 㣫���� ᪮��� ��� (����� �ਬ�� ࠡ�⠥� �������筮 doif x, _ mov ax,bx; �.�. ����� _ ����砥� 2 ��ࠬ���: mov ax � bx).
;     ���� � ���� ⮫쪮 ��� ��ਠ�� �襭�� ������ �஡����:
;     1. �ᯮ�짮���� ������ ifXXX, ������� ⠪�� � ����� _ � 㣫��� ᪮��� (ifdo x, <_ <mov ax,bx>>) - �� ������ �������,
;     2. �ᯮ�짮���� ������ 㣫��� ᪮��� (doif x, _ <<mov ax,bx>>), ������ ��� ��ਠ�� �� ࠡ�⠥� � TASM, �� ����, ���뢠�, �� ����� include-䠩� ���� 㭨���ᠫ�� ��� TASM � MASM.
;     p.s. �� �஡���� ������ � ����ᮢ doifXXX �� ⮫쪮 � ����ᮬ _, �� � � ��㣨��, �ਭ����騬� ��᪮�쪮 ��ࠬ��஢, ����� ����� ᮤ�ঠ�� ������ (���ਬ��, doif x, ?errif <hello, world>).
; [+] �������� ����� ifseteel (� ���������� � ifsetel) � �ᥢ������ ��� ����� ����ᮢ: ifset, ifsete.
; [*] ��ࠢ���� �訡�� � ����� ifsetel, ���������� �� �ᯮ�짮����� 㣫���� ᪮��� � do1 �/��� do0.
; [*] ����� _ ���७ �� 16 ��ࠬ��஢.
; [*] ����� ifelif ��९�ᠭ ��� �ᯮ�짮����� ��������� ����ᮢ, � ��㣨� ������ doif-������� ������ �������� �� ifdo-�������.
;
; v1.03 (08.03.2017)
; [+] ��������� ������ doif, doifn � doifx, ࠡ���騥 �������筮 ifdo, ifndo � ifx, �� ��������騥 㪠�뢠�� ��ࠬ��� do ��� <㣫���� ᪮���> (ifdo, ifndo � ifx ��࠭��� ��� ���⭮� ᮢ���⨬���).
; [+] ��������� ������ doifset, doifnset � ifsetel, �஢����騥 � ��।������ �����䨪���, � ��� ���祭��.
; [+] ��������� ������ aligndata, aligncode.
; [+] �������� �����䨪��� dw$, ��।����騩 dw ��� dd � ����ᨬ��� �� ࠧ�來��� ����.
; [+] ��������� �����䨪���� proto = procdesc � invoke = call ��� TASM.
; [+] �� 㬮�砭�� ����� include-䠩� ����砥� ��४⨢� locals � smart ��� TASM; �᫨ �㦭� �� �⪫����, ��⠭���� ����⠭�� xnodir = 1 �� ����祭�� 䠩��.
; [+] �ॡ㥬�� ����� ������� include-䠩�� ����� ⥯��� �஢����� �� ⮫쪮 �१ ����⠭�� xmac_ver_req, �� � �१ ����� check_xmac_ver (��᫥ ������祭�� 䠩��), �� �᪫�砥� ����������� ����⪨.
; [*] ������� ������� ����⠭���� � ��������� � ������ (���ਬ��, ifel ��९�ᠭ ��� �ᯮ�짮����� ��������� ����ᮢ, �� ������ 㢥����� ᪮���� � ������ �����㦥��� �訡�� � ����).
;
; v1.02 (01.02.2017)
; [+] �������� ����� loopx, ��������騩 dec + jnz (jns).
; [*] � ����� ?err ��� ���������� ' !!!' � ����� ᮮ�饭�� ��ࠬ��� exclam ������ ���� <> 0 (࠭�� �����筮 �뫮 ������ �� ���祭��, � �.�. 0).
;
; v1.01 (26.01.2017)
; [+] ��������� �����䨪���� $ax, $bx, $cx, $dx, $si, $di, $bp, $sp, $word, j$cxz, cwdq$, pushf$, popf$.
; [+] ��������� ������ iforel, ifandel, iforelx, ifandelx (�� ������ ।��樨).
; [+] �������� �ᥢ����� ifndo.
; [*] � ������ defdef � _defdef �������� �������⥫�� ��ࠬ���, ��������騩 ������ �஢��� ��� ����� ����⠭��.
; [*] � ������ ?err, ?errif, ?errifx �������� �������⥫�� ��ࠬ���, ��������騩 ��������� � ᮮ�饭�� 3 ��᪫��⥫��� �����.
; [-] �� ������ ।��樨 㤠���� ������ movt, movp, xchgp, calli, outsXXX, intfn.
; [-] �� ������ ।��樨 㤠���� �஢�ઠ xmacro_ver_req (��᪮��� �� ����� ���� ��稭�� ᮮ�饭�� �� �訡�� �� ����祭�� ����� ।��権).
;
; v1.00 (09.01.2017)
; [!] ����� ��ࢠ� �����.
;
;-----------------------------------------------------------------------------------------------------------------------

; �믮������ ������権/��४⨢/����ᮢ, �������� ����� ��ப��
_		macro	p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16
	irp	x, <<p1>, <p2>, <p3>, <p4>, <p5>, <p6>, <p7>, <p8>, <p9>, <p10>, <p11>, <p12>, <p13>, <p14>, <p15>, <p16>>
		x
	endm
endm

;-----------------------------------------------------------------------------------------------------------------------
;-- �������� ������� ---------------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; �᫨ cond, �ᯮ����� do1; ���� do0
ifel		macro	cond:req, do1, do0
	if	cond
		do1
	else
		do0
	endif
endm
ifdo		equ	<ifel>

; �᫨ �� cond, �ᯮ����� do1; ���� do0
ifnel		macro	cond:req, do1, do0
	ifelx	<e cond>, <do1>, <do0>
endm
ifndo		equ	<ifnel>
ifeel		equ	<ifnel>

ifn		equ	<ife>

; �������筮 ifel, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ifelx <def debug>, <int 3>, nop
ifelx		macro	cond:req, do1, do0
	if&cond
		do1
	else
		do0
	endif
endm
ifx		equ	<ifelx>

;-= ������ ��������� -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
false		=	0
true		=	not false

; ��⠭���� isTASM = true � isMASM = false, �᫨ ⥪�騩 ��������� - TASM, ���� �������
ifelx		<def ??Version>, <_ <isTASM = true>, <isMASM = false>>, <_ <isMASM = true>, <isTASM = false>>
;-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

if		isTASM
; �᫨ cond, �ᯮ����� do (do ���㦭� �������� � <>)
  doif		macro	cond:req, do:rest
	if	cond
		do
	endif
  endm

; �᫨ �� cond, �ᯮ����� do (do ���㦭� �������� � <>)
  doifn		macro	cond:req, do:rest
	doifx	<e cond>, do
  endm

; �������筮 doif, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: doifx <def debug>, int 3 (do ���㦭� �������� � <>)
  doifx		macro	cond:req, do:rest
	if&cond
		do
	endif
  endm
else ; ��������� ������ ��� MASM
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

; �᫨ cond1, �ᯮ����� do1; ���� �᫨ cond2, �ᯮ����� do2; ���� do0
ifelif		macro	cond1:req, do1, cond2:req, do2, do0
	if	cond1
		do1
	elseif	cond2
		do2
	else
		do0
	endif
endm

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

; �᫨ cond1 ��� cond2 (�᫮��� cond2 �஢������ ⮫쪮 � ⮬ ��砥, �᫨ cond1 �� �믮�������), �ᯮ����� do1; ���� do0
iforel		macro	cond1:req, cond2:req, do1, do0
	ifelif	<cond1>, <do1>, <cond2>, <do1>, <do0>
endm

; �᫨ cond1 � cond2 (�᫮��� cond2 �஢������ ⮫쪮 � ⮬ ��砥, �᫨ cond1 �믮�������), �ᯮ����� do1; ���� do0
ifandel		macro	cond1:req, cond2:req, do1, do0
	ifandelx < cond1>, < cond2>, <do1>, <do0>
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

if		isTASM
; �᫨ id ��।��� � �� ࠢ�� 0, �믮����� do (do ���㦭� �������� � <>)
  doifset	macro	id:req, do:rest
	ifdef	id
	  if	id
		do
	  endif
	endif
  endm

; �᫨ id ����।��� ��� ࠢ�� 0, �믮����� do (do ���㦭� �������� � <>)
  doifnset	macro	id:req, do:rest
	ifndef	id
		do
	elseife	id
		do
	endif
  endm
else ; ��������� ����� ��� MASM
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
endif ; else/if isTASM

; �᫨ id ��।��� � �� ࠢ�� 0, �믮����� do1, ���� do0
ifsetel		macro	id:req, do1, do0
	ifandelx	<def id>, < id>, <do1>, <do0>
endm
ifset		equ	<ifsetel>

; �᫨ id ��।��� � ࠢ�� 0, �믮����� do1, ���� do0
ifseteel	macro	id:req, do1, do0
	ifandelx <def id>, <e id>, <do1>, <do0>
endm
ifsete		equ	<ifseteel>

;-----------------------------------------------------------------------------------------------------------------------

; �᫨ ����⠭� value1 ��।�����, ��⠭����� ���祭�� const = value1, ����:
; �) �᫨ ������ 2 ��ࠬ���, � ��⠭����� ���祭�� const = value2
; �) �᫨ ������ 3 ��ࠬ���, � �᫨ ����⠭� value2 ��।�����, ��⠭����� ���祭�� const = value2, ���� ��⠭����� ���祭�� const = value3
defdef		macro	const:req, value1:req, value2:req, value3
	ifelifx	<def value1>, <const = value1>, <b <value3>>, <const = value2>, <ifelx <def value2>, <const = value2>, <const = value3>>
endm

; �᫨ ����⠭� const ��।�����, ��⠭����� ���祭�� _const = const, ����:
; �) �᫨ ������ 2 ��ࠬ���, � ��⠭����� ���祭�� _const = defval1
; �) �᫨ ������ 3 ��ࠬ���, � �᫨ ����⠭� defval1 ��।�����, ��⠭����� ���祭�� _const = defval1, ���� ��⠭����� ���祭�� _const = defval2
_defdef		macro	const:req, defval1:req, defval2
	defdef	<_&const>, <const>, <defval1>, <defval2>
endm

;-----------------------------------------------------------------------------------------------------------------------

; �뢮� ᮮ�饭�� txt �� �訡�� (��������� ᨭ⠪�� ��� MASM � TASM)
; �᫨ ��ࠬ��� exclam ����� � �� ࠢ�� 0, � ����� ᮮ�饭�� ���������� ' !!!'
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

; �뢮� ᮮ�饭�� txt �� �訡�� �� �믮������ �᫮��� cond
; �᫨ exclam ����� � �� ࠢ�� 0, � ����� ᮮ�饭�� ���������� ' !!!'
?errif		macro	cond:req, txt, exclam
	ifdo	<cond>, <?err <txt>, <exclam>>
endm

; �������筮 ?errif, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ?errifx <ndef ok>, <Error message>
; �᫨ exclam ����� � �� ࠢ�� 0, � ����� ᮮ�饭�� ���������� ' !!!'
?errifx		macro	cond:req, txt, exclam
	ifx	<cond>, <?err <txt>, <exclam>>
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

;-----------------------------------------------------------------------------------------------------------------------
;-- ����������� �������� �������� --------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; ��⠭���� �����䨪��� @32Bit (32-���� ०��) � MASM
ifdo		isMASM, <@32Bit = (@WordSize eq 4)>

; ����襭� �� ������樨/ॣ����� ������ 80386
@386		=	(@Cpu and 8) or (@Cpu eq 0)	; � TASM @Cpu = 0 � ०��� .586

; ��।������ �����䨪��஢ ॣ���஢ � ������権, 㭨���ᠫ��� ��� 16 � 32-����� ०����.
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
  ; MASM � 32-��⭮� ०��� ������� pushf/popf ��� pushfw/popfw (� ��� pusha � popa - ��� ��������: pushad, popad, ���⮬� ��� ��� �����䨪��஢ �� ᤥ����)
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

; ��४⨢� ��� TASM �� MASM
ifdo		isTASM, <proto equ <procdesc>>
ifdo		isTASM, <invoke equ <call>>

; ������� ०�� ��⨬���樨 � �ᯮ�짮����� �������� ��⮪, ��稭������ �� @@
ifnset		xnodir, <ifdo isTASM, <_ smart, locals>>

; �஢�ઠ �ॡ㥬�� ���ᨨ ������� include-䠩��, �������� ��ࠬ��஬ ver
check_xmac_ver	macro	ver:req
	?errif <ver gt xmac_ver>, <Version of 'xmac.inc' is less than required>, true
endm

; �஢�ઠ �ॡ㥬�� ���ᨨ ������� include-䠩��, �������� �१ ����⠭�� xmac_ver_req
ifx		<def xmac_ver_req>, <check_xmac_ver xmac_ver_req>

;-----------------------------------------------------------------------------------------------------------------------
;-- ��������������� ������� --------------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; ��ࠢ������� ������ �� �����, ��⭮�� value (� ᤢ���� �� shift) �⭮�⥫쭮 ����樨 rel, �������� ����࠭�⢮ ᨬ������ chr
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

; ��ࠢ������� ���� �� �����, ��⭮�� value (� ᤢ���� �� shift) �⭮�⥫쭮 ����樨 rel, �������� ����࠭�⢮ nop'���
aligncode	macro	value:=<@WordSize>, shift:=<0>, rel:=<0>
	aligndata	value, shift, rel, 90h
endm

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

;-----------------------------------------------------------------------------------------------------------------------

; �믮������ ��᢮���� ���祭�� ॣ����� � ������� mov ��� xor (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (movx ax,%val), ���� ����� xor �㤥� �ᯮ�짮��� mov
movx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <xor dst,dst>, <mov dst,src>
endm

; �믮������ �ࠢ����� ॣ���� � ������� cmp ��� test (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (cmpx ax,%val), ���� ����� test �㤥� �ᯮ�짮��� cmp
cmpx		macro	dst:req, src:req
	ifelx	<idn <src>, <0>>, <test dst,dst>, <cmp dst,src>
endm

; ������ loop ��ன ������権 dec+jnz (��� dec+jns, �᫨ plusone<>0)
; �᫨ 㪠��� ॣ���� reg, � ����� $cx �ᯮ������ ������ ��.
loopx		macro	lab:req, reg:=<$cx>, plusone:=<0>
		dec	reg
	ifel	plusone, <jns lab>, <jnz lab>
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
	    ifdo <i eq n>, <pop r>
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
