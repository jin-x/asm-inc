include		callx.inc			; ��堭��� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ���� (� ��砥 ������⢨� ������� 䠩�� ���� ������������ ��� ��ப�)

ifdef		?usecallx			; �஢�ઠ �ᯮ�짮����� ��堭���� �᪫�祭�� ���ᯮ��㥬�� ��楤�� �� ����
  prelx,	AddAbs, <AbsAX>
  pdefx,	<AddAbs, AbsAX, UIntToStr, Mul10>
endif

ifdef		??Version			; TASM
  locals
endif

; ������� ��᮫�⭮� ���祭�� ॣ���� EAX (�� ����� ॣ���஢)
ifndef		?exclAbsAX
AbsAX		proc
		push	edx
		cdq
		xor	eax,edx
		sub	eax,edx
		pop	edx
		ret
AbsAX		endp
endif ; ?exclAbsAX

; ������� ��᮫��� ���祭�� ECX ������� ᫮� �� ����� ESI, १���� ����頥��� � EAX (����� ॣ���� EDX)
ifndef		?exclAddAbs
AddAbs		proc
		cld
		xor	edx,edx
	@@Next:
		lodsd
		call	AbsAX
		add	edx,eax
		loop	@@Next
		xchg	eax,edx
		ret
AddAbs		endp
endif ; ?exclAddAbs

; �������� �᫮ EAX �� 10 (����� ॣ���� EDX)
ifndef		?exclMul10
Mul10		proc
		mov	edx,eax
		shl	eax,2
		add	eax,edx
		shl	eax,1
		ret
Mul10		endp
endif ; ?exclMul10

; �८�ࠧ����� �᫮ EAX � ASCIIZ-��ப� �� ����� EDI (����� ॣ����� EBX, ECX, EDX)
ifndef		?exclUIntToStr
UIntToStr	proc
		xor	ecx,ecx
		mov	ebx,10			; ��⥬� ��᫥���
	@@NextDigit:
		xor	edx,edx			; dx = 0 (�㦭� ��� �������)
		div	ebx			; ax = dx:ax/��⥬�_��᫥���, dx = ���⮪
		push	edx			; ��࠭塞 ���� � �⥪�
		inc	ecx			; 㢥��稢��� ���-�� ���
		test	eax,eax
		jnz	@@NextDigit		; ���室, �᫨ ��⭮� ��� �� ࠢ�� 0

	@@OutDigit:
		pop	eax			; ��������� ���� �� �⥪�
		add	al,'0'			; �८�ࠧ㥬 �� � ᨬ���
		stosb				; �����뢠�� ᨬ��� (����)
		loop	@@OutDigit		; �����塞 横� ����� ᨬ�����
		xor	al,al
		stosb				; �����뢠�� ᨬ��� ���� ��ப�
		ret
UIntToStr	endp
endif ; ?exclUIntToStr

.data
ifndef		?exclAddAbs
  Nums		dd	4096,-7075,-333,20500,-1,98765,-2533
  cNums		=	($-Nums)/4
endif

.code
