[bits 32]

; void __attribute__((cdecl)) i686_GDT_Load(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
section .text
    global i686_GDT_Load
    global load_page_directory

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


load_page_directory:

    push ebp             ; Save old base pointer
    mov ebp, esp         ; Set new base pointer

    mov eax, [ebp + 8]   ; Get the page directory address from the stack
    mov cr3, eax         ; Load the page directory address into CR3

    ; Enable paging by setting the PG bit (bit 31) of CR0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    pop ebp              ; Restore old base pointer
    ret                  ; Return to caller
