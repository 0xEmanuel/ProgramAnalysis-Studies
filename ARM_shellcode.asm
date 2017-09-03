@ Emanuel Durmaz
@
@ arm-linux-gnueabi-as -o ARM_shellcode.o ARM_shellcode.asm
@ arm-linux-gnueabi-ld -o ARM_shellcode ARM_shellcode.o
@ qemu-arm ARM_shellcode
@ cat rub


.data
arm_string: .asciz "ARM"  							@ .asciz adds a null-byte to the end of the string
after_string:
.set size_of_string, after_string - arm_string 		@ difference of addresses

filename_string: .asciz "rub"

.text
.global _start

_start:
	@open: int open(const char *pathname, int flags, mode_t mode)
	ldr r0, addr_of_filename
	mov r1, #0101 									@ O_WRONLY | O_CREAT (octal value): write-only, create file if not existing 
	ldr r2, =0666 									@ file permissions (octal value)
	mov r7, #5 										@ 5 is system call number for open
	svc #0											@ invoke syscall , note: svc (supervisor call) is the same like swi (software interrupt)
	
	cmp r0, #0										@ check if we got valid file descriptor
	blt _exit										@ jump to _exit, if we get no file descriptor
	
	mov r5, r0										@ save fd to r5

	@write: ssize_t write(int fd, const void *buf, size_t count)
	@ file descriptor is still in r0
	ldr r1, addr_of_string   						@ memory address of string
	mov r2, #size_of_string  						@ size of arm_string
	mov r7, #4               						@ write syscall
	svc #0                   						@ invoke syscall

	@close: int close(int fd)
	mov r0, r5										@ restore fd for param
	mov r7, #6 										@ close syscall
	svc #0											@ invoke syscall
	
	@exit: void exit(int status)
_exit:
    mov r0, #0				 						@ return code
	mov r7, #1               						@ exit syscall
	svc 0                    						@ invoke syscall

addr_of_string: .word arm_string
addr_of_filename: .word filename_string
