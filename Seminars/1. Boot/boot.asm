bits 16

start:
  mov ax, 0x07C0      ; 0x07C00 is where we are
  add ax, 0x20        ; add 0x20 (when shifted 512)
  mov ss, ax          ; set the stack segment
  mov sp, 0x1000      ; set the stack pointer

  mov ax, 0x07C0      ; set data segment ...
  mov ds, ax          ; more about this later



