section .text
    global add_two

add_two:
    mov eax, [esp+4]  ; Get the first argument from the stack
    add eax, 2        ; Add 2 to the value
    ret               ; Return, with the result in EAX