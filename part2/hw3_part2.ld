/* TODO: complete the script */


SECTIONS
{
 
  . = 0x20000;
  .data : 
  {
	*(.data)
	*(.bss)
  } :data 

  //TODO: add other sections
}
