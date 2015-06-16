/**
 ******************************************************************************
 * @file      startup_Cortex_M3.c
 * 				MDR 1986BE9x
 *******************************************************************************
 */
 
/*----------Stack Configuration-----------------------------------------------*/  
#define STACK_SIZE       0x00000800      /*!< Stack size (in Words)           */
__attribute__ ((section(".co_stack")))
unsigned long pulStack[STACK_SIZE];      

/*----------Macro definition--------------------------------------------------*/  
#define WEAK __attribute__ ((weak))           

/*----------Declaration of the default fault handlers-------------------------*/  
/* System exception vector handler */
void WEAK  Reset_Handler(void);   
void WEAK  NMI_Handler(void);       
void WEAK  HardFault_Handler(void); 
void WEAK  MemManage_Handler(void); 
void WEAK  BusFault_Handler(void);  
void WEAK  UsageFault_Handler(void);
void WEAK  SVC_Handler(void);       
void WEAK  DebugMon_Handler(void);  
void WEAK  PendSV_Handler(void);    
void WEAK  SysTick_Handler(void);   
   
void WEAK  CAN1_IRQHandler(void);
void WEAK  CAN2_IRQHandler(void);
void WEAK  USB_IRQHandler(void);
void WEAK  DMA_IRQHandler(void);
void WEAK  UART1_IRQHandler(void);
void WEAK  UART2_IRQHandler(void);
void WEAK  SSP1_IRQHandler(void);
void WEAK  I2C_IRQHandler(void);
void WEAK  POWER_IRQHandler(void);
void WEAK  WWDG_IRQHandler(void);
void WEAK  Timer1_IRQHandler(void);
void WEAK  Timer2_IRQHandler(void);
void WEAK  Timer3_IRQHandler(void);
void WEAK  ADC_IRQHandler(void);
void WEAK  COMPARATOR_IRQHandler(void);
void WEAK  SSP2_IRQHandler(void);
void WEAK  BACKUP_IRQHandler(void);
void WEAK  EXT_INT1_IRQHandler(void);
void WEAK  EXT_INT2_IRQHandler(void);
void WEAK  EXT_INT3_IRQHandler(void);
void WEAK  EXT_INT4_IRQHandler(void);


/*----------Symbols defined in linker script----------------------------------*/  
extern unsigned long _sidata;    /*!< Start address for the initialization 
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */    
extern unsigned long _edata;     /*!< End address for the .data section       */    
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */      
extern void _eram;               /*!< End address for ram                     */


/*----------Function prototypes-----------------------------------------------*/  
extern void SystemInit(void);
extern int main(void);           /*!< The entry point for the application.    */
void Default_Reset_Handler(void);   /*!< Default reset handler                */
static void Default_Handler(void);  /*!< Default exception handler            */


/**
  *@brief The minimal vector table for a Cortex M3.  Note that the proper constructs
  *       must be placed on this to ensure that it ends up at physical address
  *       0x00000000.  
  */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{	
  /*----------Core Exceptions------------------------------------------------ */
  (void *)&pulStack[STACK_SIZE-1],     /*!< The initial stack pointer         */
  Reset_Handler,             /*!< Reset Handler                               */
  NMI_Handler,               /*!< NMI Handler                                 */
  HardFault_Handler,         /*!< Hard Fault Handler                          */
  MemManage_Handler,         /*!< MPU Fault Handler                           */
  BusFault_Handler,          /*!< Bus Fault Handler                           */
  UsageFault_Handler,        /*!< Usage Fault Handler                         */
  0,0,0,0,                   /*!< Reserved                                    */
  SVC_Handler,               /*!< SVCall Handler                              */
  DebugMon_Handler,          /*!< Debug Monitor Handler                       */
  0,                         /*!< Reserved                                    */
  PendSV_Handler,            /*!< PendSV Handler                              */
  SysTick_Handler,           /*!< SysTick Handler                             */
  
  /*----------External Exceptions---------------------------------------------*/
  CAN1_IRQHandler,			// 0
  CAN2_IRQHandler,			// 1
  USB_IRQHandler,			// 2
  0,						// 3
  0,						// 4
  DMA_IRQHandler,			// 5
  UART1_IRQHandler,			// 6
  UART2_IRQHandler,			// 7
  SSP1_IRQHandler,			// 8
  0,						// 9
  I2C_IRQHandler,			// 10
  POWER_IRQHandler,			// 11
  WWDG_IRQHandler,			// 12
  0,						// 13
  Timer1_IRQHandler,		// 14
  Timer2_IRQHandler,		// 15
  Timer3_IRQHandler,		// 16
  ADC_IRQHandler,			// 17
  0,						// 18
  COMPARATOR_IRQHandler,	// 19
  SSP2_IRQHandler,			// 20
  0,						// 21
  0,						// 22
  0,						// 23 S_UART_IRQHandler
  0,						// 24
  0,						// 25
  0,						// 26
  BACKUP_IRQHandler,		// 27
  EXT_INT1_IRQHandler,		// 28
  EXT_INT2_IRQHandler,		// 29
  EXT_INT3_IRQHandler,		// 30
  EXT_INT4_IRQHandler		// 31
};


/**
  * @brief  This is the code that gets called when the processor first
  *         starts execution following a reset event. Only the absolutely
  *         necessary set is performed, after which the application
  *         supplied main() routine is called. 
  * @param  None
  * @retval None
  */
void Default_Reset_Handler(void)
{
  /* Initialize data and bss */
  unsigned long *pulSrc, *pulDest;

  /* Copy the data segment initializers from flash to SRAM */
  pulSrc = &_sidata;

  for(pulDest = &_sdata; pulDest < &_edata; )
  {
    *(pulDest++) = *(pulSrc++);
  }
  
  /* Zero fill the bss segment.  This is done with inline assembly since this
     will clear the value of pulDest if it is not kept in a register. */
  __asm("  ldr     r0, =_sbss\n"
        "  ldr     r1, =_ebss\n"
        "  mov     r2, #0\n"
        "  .thumb_func\n"
        "zero_loop:\n"
        "    cmp     r0, r1\n"
        "    it      lt\n"
        "    strlt   r2, [r0], #4\n"
        "    blt     zero_loop");
	
  /* Setup the microcontroller system. */
  SystemInit();
  /* Call the application's entry point.*/
  main();
}


/**
  *@brief Provide weak aliases for each Exception handler to the Default_Handler. 
  *       As they are weak aliases, any function with the same name will override 
  *       this definition.
  */
#pragma weak Reset_Handler = Default_Reset_Handler
#pragma weak NMI_Handler = Default_Handler     
#pragma weak HardFault_Handler = Default_Handler     
#pragma weak MemManage_Handler = Default_Handler     
#pragma weak BusFault_Handler = Default_Handler      
#pragma weak UsageFault_Handler = Default_Handler    
#pragma weak SVC_Handler = Default_Handler           
#pragma weak DebugMon_Handler = Default_Handler      
#pragma weak PendSV_Handler = Default_Handler        
#pragma weak SysTick_Handler = Default_Handler       
      
#pragma weak CAN1_IRQHandler = Default_Handler
#pragma weak CAN2_IRQHandler = Default_Handler
#pragma weak USB_IRQHandler = Default_Handler
#pragma weak DMA_IRQHandler = Default_Handler
#pragma weak UART1_IRQHandler = Default_Handler
#pragma weak UART2_IRQHandler = Default_Handler
#pragma weak SSP1_IRQHandler = Default_Handler
#pragma weak I2C_IRQHandler = Default_Handler
#pragma weak POWER_IRQHandler = Default_Handler
#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak Timer1_IRQHandler = Default_Handler
#pragma weak Timer2_IRQHandler = Default_Handler
#pragma weak Timer3_IRQHandler = Default_Handler
#pragma weak ADC_IRQHandler = Default_Handler
#pragma weak COMPARATOR_IRQHandler = Default_Handler
#pragma weak SSP2_IRQHandler = Default_Handler
#pragma weak BACKUP_IRQHandler = Default_Handler
#pragma weak EXT_INT1_IRQHandler = Default_Handler
#pragma weak EXT_INT2_IRQHandler = Default_Handler
#pragma weak EXT_INT3_IRQHandler = Default_Handler
#pragma weak EXT_INT4_IRQHandler = Default_Handler

/**
  * @brief  This is the code that gets called when the processor receives an 
  *         unexpected interrupt.  This simply enters an infinite loop, 
  *         preserving the system state for examination by a debugger.
  * @param  None
  * @retval None  
  */
static void Default_Handler(void) 
{
	/* Go into an infinite loop. */
	while (1) 
	{
	}
}

