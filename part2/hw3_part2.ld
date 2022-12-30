/* TODO: complete the script */

ENTRY(_hw3_dance)

PHDRS
{
  text PT_LOAD (6);
  data PT_LOAD (7);
}

SECTIONS
{
  .data : 
  {
    . = 0x20000;
	*(.data)
	*(.bss)
  } :data 

  //TODO: add other sections
  .text: 
  {
    . = 0x400000;
    *(.text)
  } :text


}

ENTRY(_hw3_dance)

SECTIONS
{
  .text : {
    *(.text)
  } >0x400000 : { *(.text) } : { *(.text) }

  .data : {
    *(.data)
    *(.bss)
  } >0x20000 : { *(.data) } : { *(.bss) }

  .rodata : {
    *(.rodata)
  } >0x9000000 : { *(.rodata) } : { *(.rodata) }

  .symtab : {
    *(.symtab)
    greens (Ndx=UND)
    blacks (Ndx=UND)
  }

  .strtab : {
    *(.strtab)
  }
}