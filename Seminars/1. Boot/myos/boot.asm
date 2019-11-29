bits 16

start:
	mov ax, 0x7C0		; 0x7c00 is the address that will be loaded into the data segment register
	add ax, 0x20		; add 0x20 to (512 bytes to set the stack segment there)
	mov ss, ax			; set the stack segment
	mov sp, 0x1000		; set the stack pointer

	mov ax, 0x07C0		; set data segment...
	mov ds, ax			;
	mov si, msg			; pointer to the message in SI
	mov ah, 0x0E		; print char BIOS procedure

.next:
	lodsb				; next byte to AL, increment SI
	cmp al, 0			; if the byte is zero
	je .done			; jump to done
	int 0x10			; invoke the BIOS system call
	jmp .next			; loop

.done:
	jmp $				; loop forever
	
msg: db 'Hello', 0		; The string we want to print

times 510-($-$$) db 0	; fill up to 510 bytes
dw 0xAA55				; last two bytes (master boot record signature at the end of the 512 bytes)
