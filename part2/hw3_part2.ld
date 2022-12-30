/* TODO: complete the script */

ENTRY(_hw3_dance)
SECTIONS
{
  . = 0x20000;
  .data : 
    {
	    *(.data)
    } AT (ADDR(.data) | PAGE_WRITEONLY)
  .bss :
    {
	    *(.bss)
    } AT (ADDR(.bss) | PAGE_WRITEONLY)

  //TODO: add other sections
  . = 0x400000;
  .text: 
  {
    *(.text)
  } AT (ADDR(.text) | PAGE_EXECUTE_WRITECOPY)

  . = 0x90000000;
  .rodata : 
  {
	*(.rodata)
  } AT (ADDR(.rodata) | PAGE_EXECUTE_READ) 

  .symtab : 
  {
    *(.symtab)
    greens (Ndx=UND)
    blacks (Ndx=UND)
  }

}