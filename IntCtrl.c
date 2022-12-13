/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  IntCrtl.c
 *        \brief  Nested Vector Interrupt Controller Driver
 *
 *      \details  The Driver Configure All MCU interrupts Priority into gorups and subgroups
 *                Enable NVIC Interrupt Gate for Peripherals
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "IntCtrl.h"
#include "Mcu_Hw.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/	

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void IntCrtl_Init(void)                                      
* \Description     : initialize Nvic\SCB Module by parsing the Configuration 
*                    into Nvic\SCB registers                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void ADC_Init         (void)
{
	

	SYSCTL_RCGCGPIO_R                  |= (1UL<<4)                      ;    /*enable clock to port E*/
	RCGCADC_R                          |= (1UL<<0)                      ;    /*enable clock to ADC0*/
	GPIOAFSEL_PORTE_R                  |= (1UL<<3)                      ;    /*to enable alternate function from GPIO to PE3*/
	GPIO_PORTE_DEN_R                   &= ~(1UL<<3)                     ;    /*disable the digital functionality pf PE3*/
	GPIOAMSEL_PORTE_R                  |= (1UL<<3)                      ;    /*enable analog module AN0 of PE3*/
	ADCACTSS_ADC0_R                    &= ~(1UL<<3)                     ;    /*disable sample sequencer 3 during configuration*/
	ADCEMUX_ADC0_R                     &= ~(0xF000)                     ;    /*enable triggring the sampling through software*/
	ADCSSMUX3_ADC0_R                    = 0                             ;    /*get input from channel 0  */
	ADCSSCTL3_ADC0_R                   |= (1<<1)|(1<<2)                 ;    /*enable interrupt flag to be sit after conversion and end of sequence ???*/
	ADCACTSS_ADC0_R                    |= (1<<3)                        ;    /*enable sample sequencer 3 after configuration*/
	ADCPC_ADC0_R                        = 0x3                           ;    /*sit rate 250ksps*/
	ADCPSSI_ADC0_R                     |= (1UL<<3)                      ;    /*trigger sequencer to start by software*/
	
	/********************Iniitialize PF3 as a digital output pin */
	
	SYSCTL_RCGCGPIO_R                  |= 0x20                      ;    /*enable clock to port F*/
	GPIO_PORTF_DIR_R                   |= 0x08                      ;    /*set GREEN pin as a digital output pin*/
	GPIO_PORTF_DEN_R                   |= 0x08                      ;    /* Enable PF3 pin as a digital pin*/
	
}

/******************************************************************************
* \Syntax          : void IntCrtl_Init(void)                                      
* \Description     : initialize Nvic\SCB Module by parsing the Configuration 
*                    into Nvic\SCB registers                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void ADC_Process         (void)
{
	unsigned int adc_value                           ;
	while(1)
	{
		ADCPSSI_ADC0_R      |= (1<<3)                  ;       /* Enable SS3 conversion or start sampling data from AN0 */ 
    while((ADCRIS_ADC0_R & 8) == 0)                ;       /* Wait untill sample conversion completed*/	
		adc_value            = ADCSSFIFO3_ADC0_R       ;       /* read adc coversion result from SS3 FIFO*/
		ADCDCISC_ADC0_R      = 8                       ;       /* clear coversion clear flag bit*/
		
		
		if(adc_value >= 2048)
			GPIO_PORTF_PIN3_DATA_R  = 0x00               ;        /* turn on green LED*/
	else if(adc_value < 2048)
	    GPIO_PORTF_PIN3_DATA_R  = 0x08               ;        /* turn off green LED*/
	}
	
}
/******************************************************************************
* \Syntax          : void IntCrtl_Init(void)                                      
* \Description     : initialize Nvic\SCB Module by parsing the Configuration 
*                    into Nvic\SCB registers                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void IntCtrl_Init(void)
{
	volatile unsigned long ulLoop                      ;
	/*TODO Configure Grouping\SubGrouping System in APINT register in SCB*/
	SYSCTL_RCGCGPIO_R             |= 0x20              ;  /******** Enable clock for PORTF*/
	ulLoop                         = SYSCTL_RCGCGPIO_R ;  /*to halt for 3 time cycles which is amust in tiva */
	GPIO_PORTF_DIR_R               = 0x0E              ;  /* Configure ORTF Pin1, 2 and 3 digital output pins and pin 4,0 as input*/
	GPIO_PORTF_LOCK_R              = 0x4C4F434B        ;  /*unlockGPIOCR register*/
	GPIO_PORTF_CR_R                = 0x01              ;  /*Enable GPIOPUR register enable to commit*/
	GPIO_PORTF_PUR_R               = 0x11              ;  /*Enable Pull Up resistor PF4,PF0*/
	GPIO_PORTF_DEN_R               = 0x1F              ;  /**** Enable PORTF Pin1, 2 and 3 and 4 as a digital pins*/

    
    /*TODO : Assign Group\Subgroup priority in NVIC_PRIx Nvic and SCB_SYSPRIx Registers*/  


	/*TODO : Enable\Disable based on user configurations in NVIC_ENx and SCB_Sys Registers */


	

}


 
/******************************************************************************
* \Syntax          : void IntCrtl_Init(void)                                      
* \Description     : initialize Nvic\SCB Module by parsing the Configuration 
*                    into Nvic\SCB registers                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void Blink_leds(void)
{
	/*TODO Configure Grouping\SubGrouping System in APINT register in SCB*/
	while (1) 
	{  


		if ((~(GPIO_PORTF_PIN4_DATA_R) & 0x10))      /* Monitored if the SW0 or SW4 has been pressed. */
		{
      GPIO_PORTF_PIN1_DATA_R   |= 0x02               ;
			GPIO_PORTF_PIN2_DATA_R   &= (~(0x04))          ;
		}
		if ((~(GPIO_PORTF_PIN0_DATA_R) & 0x01))
		{
			GPIO_PORTF_PIN1_DATA_R   &= ~(0x02)            ;
			GPIO_PORTF_PIN2_DATA_R   |= 0x04               ;
		}

	}
    
    /*TODO : Assign Group\Subgroup priority in NVIC_PRIx Nvic and SCB_SYSPRIx Registers*/  


	/*TODO : Enable\Disable based on user configurations in NVIC_ENx and SCB_Sys Registers */

}

/******************************************************************************
* \Syntax          : void IntCrtl_Init(void)                                      
* \Description     : initialize Nvic\SCB Module by parsing the Configuration 
*                    into Nvic\SCB registers                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void DELAY(unsigned long x)
{

	/*TODO Configure Grouping\SubGrouping System in APINT register in SCB*/
	unsigned long j=0;
	for (j=0; j<x ; j++);
 
    /*TODO : Assign Group\Subgroup priority in NVIC_PRIx Nvic and SCB_SYSPRIx Registers*/  

	/*TODO : Enable\Disable based on user configurations in NVIC_ENx and SCB_Sys Registers */

}

/**********************************************************************************************************************
 *  END OF FILE: IntCrtl.c
 *********************************************************************************************************************/
