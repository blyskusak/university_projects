section .text
global mdiv

; Function mdiv values:
; rdi - x
; rsi - n
; rdx - y

mdiv:
        mov rcx, rsi        ; Moving rsi to rcx to use rcx as loop counter.
        mov r8, rdx         ; Moving rdx to r8 to keep y value.
        xor rdx, rdx        ; rdx has to be 0 before division.

	; xor used on rsi to set it as 0.
        ; esi is used to check which situation programs deals with.
        ; 0 means that x,y>0.
        ; 1 means y<0, x>0.
        ; 2 means x<0, y>0.
        ; And 3 means x,y<0.
	; Value of esi is changed depending on sign of x and y.
        xor rsi, rsi

; Checks x's sign. Depending on sign, it jumps directly to neg_y or not.
.sgn:
	mov rax, [rdi + (rcx - 1) * 8]

	test rax, rax
	jns .neg_y

	; In case x < 0 we add two to rsi.
	add esi, 2

; In set_values_before_neg_x, value r9 is set as 0 and r10 as 1 before function.
; r10 is used during negation process - if number after not and adding r10 equals
; one, we keep r10 value, else we change to zero and add new value to next number
; in array. It is related with number's changing sign method in U2. r9 is extra
; loop counter.
.set_values_before_neg_x:
	xor r9, r9
	mov r10, 1

; It is used to negate x if it is negative number or to change it to negative number
; after division when y<0, x>0 or y>0, x<0. It does not rax and adds value from r10
; to rax (r10 is 1 at the beginning).
.neg_x:
	inc r9

	mov rax, [rdi + (r9 - 1) * 8]

	; rax = 0 means that we did not on zero and then added one or did not on -1
	; (but this second case means that r10 = 0). If it is first case, 1 has to
	; be added to next number in array after doing on it not (we skip xor with
	; jump). If it is any other case, r10 will be 0. So case when we did not
	; on -1 will not cause any mistake in calculations.
	not rax
	add rax, r10
	jz .neg_x_continue
	xor r10, r10

; Continuation of neg_x. Used to jump there if we want to keep r10 value.
.neg_x_continue:
	mov [rdi + (r9 - 1) * 8], rax

	cmp r9, rcx
	jne .neg_x

	; This comparison is related with further addition of 4 to esi. We know that
	; in situation where esi equals 5, y<0, x>0 and number was divided. It means
	; that neg does not have to be used on reminder. If esi value is higher, it
	; meas that it has to be equal to 6 (because it is only other situation when
	; we have to do neg on x value after division). In that situation neg has to
	; be used on reminder.
	cmp esi, 5
	je .return
	ja .neg_reminder

; Function checks y sign. If it is negative, it uses neg on it.
.neg_y:
	mov r9, rcx

	test r8, r8
	jns .div_loop

	; In situation that y<0, esi is increased.
	inc esi
	neg r8

; Loop used to count abs(x)/abs(y).
.div_loop:
	mov rax, [rdi + (rcx - 1) * 8]
	div r8
	mov [rdi + (rcx - 1) * 8], rax

	loop .div_loop

	; After division 4 is added to esi to know that number was divided.
	add esi, 4

; Depending on esi value, .after_loop decides if answer can be returned or
; x and/or reminder has to be negated.
.after_loop:
	; If esi = 4, x,y>0 so all values are with good signs.
	cmp esi, 4
	je .return

        ; r9 value (length of array) is moved to rcx.
        mov rcx, r9

	; If esi < 7 (and != 4) it means that it is x>0, y<0 or x<0, y>0.
	; In this case x has to be negated again, because result in those
	; cases must be negative.
	cmp esi, 7
	jb .set_values_before_neg_x

; Function checks if there was overflow.
.check_if_overflowed:
	; If x,y<0, there is possibility of overflow. This situation is when
	; x is the smallest possible number and y = -1. In this situation, after
	; division, x keeps in [rdi + (rcx - 1) * 8] the smallest value for
	; int64_t. After negating it, overflow flag will be set to 1 and it is
	; the only case when it is set to 1.
	mov rax, [rdi + (rcx - 1) * 8]
	neg rax
	jo .overflowed

; neg_reminder uses neg on rdx to change sign of rdx.
.neg_reminder:
	neg rdx

; Function used to return answer.
.return:
	mov rax, rdx
	ret

.overflowed:
	; If y = 0 we do not have to check it because during dividing process
	; something will be divided by 0 and return error.
	xor rax, rax
	div rax
