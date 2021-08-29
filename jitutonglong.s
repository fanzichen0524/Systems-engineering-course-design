.data
msg1:	.asciiz  "\n"
msg2:	.asciiz "Input a integer: "
.text
main:
	j  label1
label1:
	li  $v0,  5
	syscall
	move  $12,  $v0
	move  $11,  $12
	li  $v0,  5
	syscall
	move  $12,  $v0
	move  $10,  $12
	li  $12  4
	move  $13,  $11
	mul  $12  $12  $13
	move  $13,  $10
	sub  $12  $12  $13
	li  $13  2
	div  $12  $13
	mflo  $12
	move  $8,  $12
	move  $12,  $11
	move  $13,  $8
	sub  $12  $12  $13
	move  $9,  $12
	move  $12,  $8
	move  $a0,  $12
	li  $v0,  1
	syscall
	li  $v0,  4
	la  $a0,  msg1
	syscall
	move  $12,  $9
	move  $a0,  $12
	li  $v0,  1
	syscall
	li  $v0,  4
	la  $a0,  msg1
	syscall
	j  exit
exit:
	li  $v0, 10
	syscall
