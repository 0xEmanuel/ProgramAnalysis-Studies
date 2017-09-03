; Emanuel Durmaz

;nasm -fwin32 primetest.asm
;golink.exe /console /entry _start primetest.obj kernel32.dll user32.dll msvcrt.dll

section .data
    szEnter: db "Enter prime:",0xa, 0 ;0 = end of string
    szScan:  db "%d", 0
    szIsPrime: db "Is Prime", 0xa,0
    szIsNotPrime: db "Is not Prime", 0xa,0

section .bss
; reserve for variables
input: resb 4

section .text
    global _start
	; tell NASM that printf and scanf are symbols 
	; defined in another module   
    extern printf, scanf
    
_start:
    push szEnter ;parameter for printf
    call    printf
    add     esp, 4; cdecl calling convention --> caller cleans the stack
    
    push input ; address of variable
    push szScan
    call scanf
    add esp,8
    
    mov ebx, [input]
    mov ecx, ebx
    dec ecx; use input-1 for the loop cancel condition
    
	;check special cases
	cmp ebx, 1
	je _notPrime
	cmp ebx, 2
	je _isPrime
    
    push 2; B
    push ebx ; A
    call _mod ; A mod B
    add esp, 8
    
    cmp eax, 0 
        je _notPrime ;if divided by 2
    mov edx, 3 ; start value
    
_loop: ; if I find a divisor --> not prime
    cmp edx, ecx ; loop cancel condition
    ja _isPrime ; exits loop

    push edx; B
    push ebx ; A
    call _mod ; A mod B
    add esp, 4 ; remove only one param...B is still on stack
    pop edx ; restore edx (B)
    
    cmp eax, 0 
        je _notPrime ;if divided by edx value
    
    add edx, 2 ; we just want to test odd numbers
    
    ;cmp edx, ebx
    ;jna _loop
    jmp _loop
    
_isPrime:
    push szIsPrime
    call printf
    add esp, 4
    ret
    
_mod:
    ;Prolog
    push ebp
    mov ebp, esp
    ;sub esp, 4 ; dont need local variable

    mov eax, DWORD [ebp+0x8] ; first parameter
    xor edx, edx ; erase edx, since it could be used as extension for the dividend
    idiv DWORD [ebp+0xC] ; second parameter
    mov eax, edx; copy remainder in edx to eax as return value	

    ;Epilog
    mov esp, ebp			
    pop ebp					
    ret	
    
_notPrime:
    push szIsNotPrime
    call printf
    add esp, 4
    ret
