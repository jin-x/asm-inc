;############################################
;##                                        ##
;##   -= EXTRA MACROSES (FULL EDITION) =-  ##
;##      ������� ��� MASM/TASM (16/32)     ##
;##             ������ ।����            ##
;##         [ v1.04 :: 10.03.2017 ]        ##
;##                                        ##
;##  (c) 2017 by Jin X (jin.x@sources.ru)  ##
;##           http://xk7.ru/p/a/i          ##
;##                                        ##
;############################################

xmacro_ver	=	104h			; ����� ������� 䠩�� (word: ���訩 ���� - 楫�� ����, ����訩 - �஡���)

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
; [+] ��������� ������ ifnsetel � ifseteel (� ���������� � ifsetel) � �ᥢ������ ��� ��� 3-� ����ᮢ: ifset, ifnset � ifsete.
; [+] �������� ����� fastnops � ����⠭� fastnopsprefix, � ����� aligncode ����稫 �������⥫�� ��ࠬ��� prefix � �ᯮ���� (⠪ ��, ��� � alignc � aligncs) nop'� � ��䨪ᠬ�.
; [*] ��ࠢ���� �訡�� � ����� ifsetel, ���������� �� �ᯮ�짮����� 㣫���� ᪮��� � do1 �/��� do0.
; [*] ����� _ ���७ �� 16 ��ࠬ��஢.
; [*] ����� ifelif ��९�ᠭ ��� �ᯮ�짮����� ��������� ����ᮢ, � ��㣨� ������ doif-������� ������ �������� �� ifdo-�������.
;
; v1.03 (08.03.2017)
; [+] ��������� ������ doif, doifn � doifx, ࠡ���騥 �������筮 ifdo, ifndo � ifx, �� ��������騥 㪠�뢠�� ��ࠬ��� do ��� <㣫���� ᪮���> (ifdo, ifndo � ifx ��࠭��� ��� ���⭮� ᮢ���⨬���).
; [+] ��������� ������ doifset, doifnset, doifsete � ifsetel, �஢����騥 � ��।������ �����䨪���, � ��� ���祭��.
; [+] ��������� ������ aligndata, aligncode, alignd, alignc, alignds, aligncs.
; [+] �������� �����䨪��� dw$, ��।����騩 dw ��� dd � ����ᨬ��� �� ࠧ�來��� ����.
; [+] ��������� �����䨪���� proto = procdesc � invoke = call ��� TASM.
; [+] �� 㬮�砭�� ����� include-䠩� ����砥� ��४⨢� locals � smart ��� TASM; �᫨ �㦭� �� �⪫����, ��⠭���� ����⠭�� xnodir = 1 �� ����祭�� 䠩��.
; [+] �ॡ㥬�� ����� ������� include-䠩�� ����� ⥯��� �஢����� �� ⮫쪮 �१ ����⠭�� xmacro_ver_req, �� � �१ ����� check_xmacro_ver (��᫥ ������祭�� 䠩��), �� �᪫�砥� ����������� ����⪨.
; [*] ������� ������� ����⠭���� � ��������� � ������ (���ਬ��, ifel ��९�ᠭ ��� �ᯮ�짮����� ��������� ����ᮢ, �� ������ 㢥����� ᪮���� � ������ �����㦥��� �訡�� � ����).
;
; v1.02 (01.02.2017)
; [+] �������� ����� loopx, ��������騩 dec + jnz (jns).
; [*] � ����� movt ���祭�� ��ࠬ��� temp �� 㬮�砭�� �������� � ax �� $ax.
; [*] � ����� ?err ��� ���������� ' !!!' � ����� ᮮ�饭�� ��ࠬ��� exclam ������ ���� <> 0 (࠭�� �����筮 �뫮 ������ �� ���祭��, � �.�. 0).
;
; v1.01 (26.01.2017)
; [+] ��������� �����䨪���� $ax, $bx, $cx, $dx, $si, $di, $bp, $sp, $word, j$cxz, cwdq$, pushf$, popf$.
; [+] �������� �ᥢ����� ifndo.
; [*] � ������ defdef � _defdef �������� �������⥫�� ��ࠬ���, ��������騩 ������ �஢��� ��� ����� ����⠭��.
; [*] � ������ ?err, ?errif, ?errifx �������� �������⥫�� ��ࠬ���, ��������騩 ��������� � ᮮ�饭�� 3 ��᪫��⥫��� �����.
; [*] ��ࠢ��� ��� � ����� movt.
;
; v1.00 (09.01.2017)
; [!] ����� ��ࢠ� �����.
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
ifeel		equ	<ifnel>
ifndo		equ	<ifnel>

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

; �������筮 ifnel, �� �᫮��� ����⠢����� ᫨⭮ � if, ���ਬ��: ifnelx <def debug>, nop, <int 3>
ifnelx		macro	cond:req, do1, do0
	ifelx	<cond>, <do0>, <do1>
endm
ifeelx		equ	<ifnelx>

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

; �᫨ id ��।��� � ࠢ�� 0, �믮����� do (do ���㦭� �������� � <>)
  doifsete	macro	id:req, do:rest
	ifdef	id
	  ife	id
		do
	  endif
	endif
  endm
else ; ��������� ������ ��� MASM
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

; �᫨ id ��।��� � �� ࠢ�� 0, �믮����� do1, ���� do0
ifsetel		macro	id:req, do1, do0
	ifandelx <def id>, < id>, <do1>, <do0>
endm
ifset		equ	<ifsetel>

; �᫨ id ����।��� ��� ࠢ�� 0, �믮����� do1, ���� do0
ifnsetel	macro	id:req, do1, do0
	iforelx <ndef id>, <e id>, <do1>, <do0>
endm
ifnset		equ	<ifnsetel>

; �᫨ id ��।��� � ࠢ�� 0, �믮����� do1, ���� do0
ifseteel	macro	id:req, do1, do0
	ifandelx <def id>, <e id>, <do1>, <do0>
endm
ifsete		equ	<ifseteel>

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
;-- ����������� �������� �������� --------------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------------------------------------------

; ��⠭���� �����䨪��� @32Bit (32-���� ०��) � MASM
ifdo		isMASM, <@32Bit = (@WordSize eq 4)>

; ����襭� �� ������樨/ॣ����� �����஢ (� TASM @Cpu = 0 � ०��� .586)
@186		=	(@Cpu and 2) or (@Cpu eq 0)
@286		=	(@Cpu and 4) or (@Cpu eq 0)
@386		=	(@Cpu and 8) or (@Cpu eq 0)
@486		=	(@Cpu and 10h) or (@Cpu eq 0)
@586		=	(@Cpu and 20h) or (@Cpu eq 0)
@686		=	@Cpu and 40h

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

; ����� ��䨪�, �ᯮ��㥬��� �� 㬮�砭�� ��� ��᫥����⥫쭮�� nop'�� (90h=nop - ��� ��䨪�)
; [����� �ᯮ�짮����: 3Eh=ds, 2Eh=cs, 26h=es, 36h=ss, 0F2h=repne, 386+: 66h=operand resize, 67h=address resize, 64h=fs, 65h=gs; �� ⮫쪮 �� 0F3h=repe, 0F0h=lock ��� ���� ���祭�� !!!]
ifel		@386, <fastnopsprefix = 66h>, <fastnopsprefix = 3Eh>

; ������� ०�� ��⨬���樨 � �ᯮ�짮����� �������� ��⮪, ��稭������ �� @@
ifnset		xnodir, <ifdo isTASM, <_ smart, locals>>

; �஢�ઠ �ॡ㥬�� ���ᨨ ������� include-䠩��, �������� ��ࠬ��஬ ver
check_xmacro_ver macro	ver:req
	?errif <ver gt xmacro_ver>, <Version of 'xmac.inc' is less than required>, true
endm
check_xmac_ver	equ	<check_xmacro_ver>

; �஢�ઠ �ॡ㥬�� ���ᨨ ������� include-䠩��, �������� �१ ����⠭�� xmacro_ver_req � xmac_ver_req
ifx		<def xmacro_ver_req>, <check_xmacro_ver xmacro_ver_req>
ifx		<def xmac_ver_req>, <check_xmacro_ver xmac_ver_req>

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

; ��ࠢ������� ���� �� �����, ��⭮�� value (� ᤢ���� �� shift) �⭮�⥫쭮 ����樨 rel, �������� ����࠭�⢮ nop'��� � ��䨪ᮬ fastnopsprefix (�. fastnops)
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
; ��ࠢ������� ������ �� �����, ��⭮�� value � �믮������� do (do ���㦭� �������� � <>)
  alignd	macro	value:=<@WordSize>, do:rest
	aligndata	value
	do
  endm

; ��ࠢ������� ���� �� �����, ��⭮�� value � �믮������� do (do ���㦭� �������� � <>)
  alignc	macro	value:=<@WordSize>, do:rest
	aligncode	value
	do
  endm

; ��ࠢ������� ������ �� �����, ��⭮�� value � ᤢ���� �� shift � �믮������� do (do ���㦭� �������� � <>)
  alignds	macro	value:=<@WordSize>, shift:=<0>, do:rest
	aligndata	value, shift
	do
  endm

; ��ࠢ������� ���� �� �����, ��⭮�� value � ᤢ���� �� shift � �믮������� do (do ���㦭� �������� � <>)
  aligncs	macro	value:=<@WordSize>, shift:=<0>, do:rest
	aligncode	value, shift
	do
  endm
else ; ��������� ������ ��� MASM
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
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (movx ax,%val), ���� ����� xor �㤥� �ᯮ�짮��� mov
movx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <xor dst,dst>, <mov dst,src>
endm

; �믮������ �ࠢ����� ॣ���� � ������� cmp ��� test (�᫨ src = 0)
; �⮡� ��।��� � ��ࠬ��� src ����⠭��, ����室��� ���⠢��� ��। ��� % (cmpx ax,%val), ���� ����� test �㤥� �ᯮ�짮��� cmp
cmpx		macro	dst:req, src:req
	ifidnel	<src>, <0>, <test dst,dst>, <cmp dst,src>
endm

; �믮������ ��᢮���� op = val �१ �६���� ॣ���� temp (�� 㬮�砭�� $ax; ���ਬ��, movt ds,@data ��� movt es,0A000h,dx)
movt		macro	op:req, val:req, temp:=<$ax>
		movx	temp,val
		mov	op,temp
endm

; �믮������ ��᢮���� op = val �१ �⥪
movp		macro	op:req, val:req
		push	val
		pop	op
endm

; ����� ���祭�ﬨ op1 � op2 �१ �⥪
; ����� �� �஢���� ᮮ⢥��⢨� ࠧ�來��� ���࠭��� !!!
xchgp		macro	op1:req, op2:req
		push	op1
		push	op2
		pop	op1
		pop	op2
endm

; ������ loop ��ன ������権 dec+jnz (��� dec+jns, �᫨ plusone<>0)
; �᫨ 㪠��� ॣ���� reg, � ����� $cx �ᯮ������ ������ ��.
loopx		macro	lab:req, reg:=<$cx>, plusone:=<0>
		dec	reg
	ifel	plusone, <jns lab>, <jnz lab>
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
	    ifdo <i eq n>, <pop r>
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

; ������஢��� nop'� � ������⢥ num ���, ࠧ�������� ��䨪ᮬ prefix (�� 㬮�砭�� fastnopsprefix, ���஬� ����砫쭮 ��ᢠ������� ���祭�� 3Eh = ds)
; �믮������ ����⥫쭮 ����॥, 祬 ���筠� ��᫥����⥫쭮��� nop'��; ���ᨬ��쭮 �����⨬� ࠧ��� ������樨 (15 ����) �� �ॢ�蠥���
fastnops	macro	num:req, prefix:=<fastnopsprefix>
	local	n
		n = 0
	rept	num
		n = n + 1
	  ifel <((n mod 15) eq 0) or (n eq num)>, nop, <db prefix>
	endm
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
