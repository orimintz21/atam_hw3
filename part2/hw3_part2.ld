ENTRY(_hw3_dance)

PHDRS
{
	text PT_LOAD FLAGS ( 2 | 1 ) ;
	data PT_LOAD FLAGS ( 2 ) ;
	rodata PT_LOAD FLAGS ( 4 | 1 ) ;
}

EXTERN (greens blacks)

SECTIONS
{
  .data 0x20000 : 
    {
	    *(.data)
    } :data
  .bss :
    {
	    *(.bss)
    } :data
  
  .text 0x400000  : 
  {
    *(.text)
  } :text

  .rodata 0x90000000 : 
  {
	*(.rodata)
  } :rodata

}
