/*
*****************************************************************************
**

**  File        : stm32f746_flash.ld
**
**  Abstract    : Linker script for STM32F746NGHx Device with
**                1024KByte FLASH, 320KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size  = __HEAP_SIZE;    /* required amount of heap  */
_Min_Stack_Size = __STACK_SIZE;   /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
    FLASH          (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM            (rwx) : ORIGIN = 0x20000000, LENGTH = 320K /*240K*/
   /* TCM            (rwx) : ORIGIN = 0x20000000, LENGTH = 64K*/
    GFX_DISPLAY    (rwx) : ORIGIN = 0xC0000000, LENGTH = 7M
    GFX_RAM_DBASE  (rwx) : ORIGIN = 0xC0700000, LENGTH = 1M - 64K
    RAM_DBASE      (rwx) : ORIGIN = 0xC07F0000, LENGTH = 64K
    GFX_QSPI_DBASE (rx)  : ORIGIN = 0x90000000, LENGTH = 128K
}

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH


  /* Uninitialized data section */
  . = ALIGN(4);
  .bss (NOLOAD) :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* User_heap section, used to check that there is enough RAM left */
  .heap (NOLOAD) :
  {
    . = ALIGN(4);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
	*(.heap*)
    . = . + _Min_Heap_Size;
    __HeapLimit = .;
    . = ALIGN(4);
  } >RAM


  /* Remove information from the standard libraries */
/*  /DISCARD/ : */
/*  {*/
    /*libc.a ( * )*/
    /*libm.a ( * )*/
    /*libgcc.a ( * )*/
  /*}*/

  .ARM.attributes 0 : { *(.ARM.attributes) }

   __StackLimit = _estack - _Min_Stack_Size;

  	__gfx_display_base__   = ORIGIN(GFX_DISPLAY);
	__gfx_ram_data_base__  = ORIGIN(GFX_RAM_DBASE);
	__user_ram_data_base__ = ORIGIN(RAM_DBASE);
	__gfx_qspi_data_base__ = ORIGIN(GFX_QSPI_DBASE);

   /* Check if data + heap + stack exceeds RAM limit */
   ASSERT(__StackLimit >= __HeapLimit, "Region RAM overflowed with stack")
}


