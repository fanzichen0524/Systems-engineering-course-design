.data
msg1:	.asciiz  "\n"
msg2:	.asciiz "Input a integer: "
.text
main:
	j  label1
label1:
	li  $v0,  5
	syscall
	move  $11,  $v0
	move  $9,  $11
	move  $11,  $9
	move  $10,  $11
	li  $11  1
	move  $8,  $11
label8:
	move  $11,  $10
	li  $12  0
	sne  $11,  $11,  $12
	beq  $11, $0, label25
	move  $11,  $8
	li  $12  1
	add  $11  $11  $12
	move  $8,  $11
	move  $11,  $8
	li  $12  2
	mul  $11  $11  $12
	move  $8,  $11
	move  $11,  $10
	li  $12  1
	sub  $11  $11  $12
	move  $10,  $11
	j  label8
label25:
	move  $11,  $8
	move  $a0,  $11
	li  $v0,  1
	syscall
	li  $v0,  4
	la  $a0,  msg1
	syscall
	j  exit
exit:
	li  $v0, 10
	syscall
