; Input: two strings of 1's seperated by 0, eg '1111101111'

#Q = {start,cmp,one_sub_two,two_sub_one,accept}
#S = {0,1}
#G = {0,1,_}
#q0 = start
#B = _
#F = {accept}
#N = 2 

; the transition functions
start 1* 1* r* start ;Skip the first string of consecutive 1s
start 0* _* r* copy  ;Encountered 0, start copying the second number

copy 1_ _1 rr copy  ;Copy the number
copy __ __ ll copy  ;End copy
copy _1 _1 l* copy  ;Move the head of the first tape to the first 1 on the right               
copy 11 11 ** cmp   ;Start compare

cmp 11 11 ll cmp    ;Compare, move two heads at the same time
cmp __ __ rr accept ;Equal, accept

;The number on the first tape is greater than the number on the second tape,
;subtract the second one from the first one
cmp 1_ 1_ rr one_sub_two

;The number on the second tape is greater than the number on the first tape,
;subtract the first one from the second one
cmp _1 _1 rr two_sub_one

one_sub_two 11 _1 rr one_sub_two ;Do the subtraction and cover the 1 on the tape with '_'
one_sub_two __ __ l* one_sub_two ;Skip '_', move the head of the first tape to the first 1 on the right
one_sub_two 1_ 1_ *l cmp         ;Compare again

two_sub_one 11 1_ rr two_sub_one ;Do the subtraction and cover the 1 on the tape with '_'
two_sub_one __ __ *l two_sub_one ;Skip '_', move the head of the second tape to the first 1 on the right
two_sub_one _1 _1 l* cmp         ;Compare again