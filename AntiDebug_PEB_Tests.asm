; Emanuel Durmaz
;

;nasm -fwin32 AntiDebug_PEB_Tests.asm
;golink.exe /console /entry start AntiDebug_PEB_Tests.obj kernel32.dll user32.dll msvcrt.dll

  section .data

  szDebugYes         db "Go away! I don't like to be debugged. Everyone deserves a little bit privacy.", 0Ah, 0Dh, 0
  szDebugNo          db "All right, I am free!", 0Ah, 0Dh, 0

  szBeingDebugged    db "BeingDebugged: %x", 0Ah, 0Dh, 0
  szNtGlobalFlag     db "NtGlobalFlag: %x", 0Ah, 0Dh, 0
  szCommandLine      dw 'C','o','m','m','a','n','d','l','i','n','e',' ',':',' ','%','s', 0  ;Unicode-String

  Test1              dd 0
  Test2              dd 0

 section .text

 global start
 extern printf, wprintf
  
start:
  
  
  ;*** Is Debugger Present? ***   
  mov   edx, 0                        ; edx holds the value of interest
  mov   eax, [fs:0x30]                ; fetch pointer to PEB
                                    
  ; --------------------------------
  ; load BeingDebugged into edx
  ; --------------------------------

  mov dl, byte [eax+0x02]			 ; fetch the BeingDebugged-Flag from PEB+0x02

  mov   dword [Test1], edx           ; save the first result 
  push  edx
  push  szBeingDebugged       
  call  printf
  add   esp, 8  
   
 ;*** Is NtGlobalFlag set? ***

  mov   edx, 0                        ; edx holds the value of interest
  mov   eax, [fs:0x30]                ; fetch pointer to PEB

 ; --------------------------------
 ; load NtGlobalFlag into edx
 ; --------------------------------
  mov dl, byte [eax+0x68]			 ; fetch the NtGlobalFlag from PEB+0x68
  
  mov   dword [Test2], edx           ; save the second result 
  push  edx
  push  szNtGlobalFlag       
  call  printf
  add   esp, 8  
 

 ;*** Combine the tests ***
 ; check if we are being debugged
  xor ebx, ebx
  add ebx, [Test1]
  add ebx, [Test2]
  
  cmp ebx, 0
  je _DebugNo ; for this case: BeingDebugged and NtGlobalFlag both must be 0
   
_DebugYes:  
  push  szDebugYes
  jmp   _output
  
_DebugNo:
  push  szDebugNo    
  
_output:
  call  printf
  add   esp, 4


  ;*** Commandline Parameters ***
  mov   eax, [fs:0x30]                ;fetch pointer to PEB

  ; --------------------------------
  ; fetch pointer to ProcessParameters
  ;
  ; push  pointer to CommandLine-String
  ; push  offset szCommandLine 
  ; call  wprintf                  ;print CommandLine-String as UNICODE
  ; add   esp, 8
  ; --------------------------------

  mov eax, [eax+0x10] ; fetch pointer to ProcessParameters from PEB and overwrite eax (pointer to structure RTL_USER_PROCESS_PARAMTERS)
  mov edx, [eax+0x44] ; fetch pointer to CommandLine-String from ProcessParameters

  push edx
  push szCommandLine
  call wprintf
  add esp, 8
  
ret
 
