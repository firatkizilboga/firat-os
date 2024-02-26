[bits 32]

; void __attribute__((cdecl)) i686_GDT_Load(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
section .text
    global i686_GDT_Load
    global loadPageDirectory
    global enablePaging

i686_GDT_Load:
    
    ; make new call frame
    push ebp             ; save old call frame
    mov ebp, esp         ; initialize new call frame
    
    ; load gdt
    mov eax, [ebp + 8]
    lgdt [eax]

    ; reload code segment
    mov eax, [ebp + 12]
    push eax
    push .reload_cs
    retf

.reload_cs:

    ; reload data segments
    mov ax, [ebp + 16]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax,
    mov ss, ax

    ; restore old call frame
    mov esp, ebp
    pop ebp
    ret


loadPageDirectory:

    ; make new call frame
    push ebp             ; save old call frame
    mov ebp, esp         ; initialize new call frame

    ; load page directory
    mov eax, [ebp + 8]
    mov cr3, eax         ; set CR3 register to the new page directory address

    ; restore call frame and return
    mov esp, ebp
    pop ebp
    ret
enablePaging:

    ; make new call frame
    push ebp             ; save old call frame
    mov ebp, esp         ; initialize new call frame

    ; enable paging
    mov eax, cr0
    or eax, 0x80000000   ; set the paging bit in CR0
    mov cr0, eax

    ; restore call frame and return
    mov esp, ebp
    pop ebp
    ret
