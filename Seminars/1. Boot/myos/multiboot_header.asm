section .multiboot_header

magic 	equ 0xE85250D6		; multibood 2
arch	equ 0				; protected mode i 386

header_start:
		dd magic						; magic number
		dd arch							; architecture
		dd header_end - header_start 	; header length
		dd 0x100000000 - (magic + arch + (header_end - header_start))
		dw 0							; type
		dw 0							; flags
		dd 8							; size
		header_end:
