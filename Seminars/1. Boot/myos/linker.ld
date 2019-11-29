ENTRY(start)

SECTIONS {
  . = 1M;

  .boot :
  {
    *(.multiboot_hader)
  }

  .text :
  {
    *(.text)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.bss)
  }
}
