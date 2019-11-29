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
}
