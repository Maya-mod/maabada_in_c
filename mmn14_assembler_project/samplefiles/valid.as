X: .string "Heo, Word! *()"
testting123: .data -1, 1, -2, 78, 90, 45328, -95743
abcxyz9: .string "StrCatChrTokLen.h"
.extern label1
XYZ123XYZ: .data 0	 ,  	0 	,	 0  ,  0 ,	0 	
.entry XYZ123XYZ

; mov 013,13
mov #0, testting123
mov #-1, r0
mov r0, r1
mov r0, testting123
mov testting123, label1
mov testting123, r0


cmp #-1, r0
cmp r0, r1
cmp r0, testting123
cmp testting123, label1
cmp testting123, r0
;.;;,; ; Colicated;;9352@!$&#(%$*) Comment


add #395, testting123
add r7, abcxyz9
add testting123, r6

sub #-1, r0
sub r2, r3
sub verylonglabelnamewithextralen, label11
sub testting123, r6

lea testting123, verylonglabelnamewithextralen
lea label11, r4

; Although the symbol is defined later, that should not be a problem!
clr LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

not r6
not X

inc r7
inc X1234YZASFJKFDSA524bsdasfjdgdaf

dec r0
dec verylonglabelnamewithextralen

C0: jmp testting123
jmp &C0

; Put some data here randomally:

DataIsNiceSoLearnDataSc: .string "DataIsNiceSoLearnDataSc"
.entry DataIsNiceSoLearnDataSc

CCC1: bne X
bne &CCC1
;
C5: jsr X
jsr &C5
;;
; red 13
red r4
red testting123
;;
; prn 013
prn r5
prn #-32
prn mychars
;
;rts
rts
;
stop

label11: .data 9
LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL: .string " "
.entry LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
X1234YZASFJKFDSA524bsdasfjdgdaf: .data 5
.entry X1234YZASFJKFDSA524bsdasfjdgdaf
label01: .data -000000, +000000, +000001, -000004
mychars: .string "RANDOM1234567890!*&#(@%!&E(1"
	
.extern verylonglabelnamewithextralen
