// Copyright (c) 2015-19, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "main.h"

static void init_touch_screen(void)
{
	ft6x06_init();
}

static void init_eeprom(void)
{
	eeprom_init();
}

static void init_ioexpand_leds(void)
{
	// Setting the port A to be output (LEDs)
	io_expander_write_reg(MCP23017_IODIRA_R, 0x00);
	// Turn on the LEDS
	io_expander_write_reg(MCP23017_GPIOA_R, 0xFF);
}

static void init_accel(void)
{
	accel_initialize();
}

static void init_joystick(void)
{
	uint32_t pr_mask = SYSCTL_PRADC_R0;
	ADC0_Type *myADC = (ADC0_Type *) ADC0_BASE;
	
	SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
	
	// wait for ADC0 to become ready
	while( (pr_mask & SYSCTL->PRADC) != pr_mask){}
	
	// disable sample sequencer 1
	myADC->ACTSS &= ~ADC_ACTSS_ASEN1;
	
	// set the mux to trigger based on processor/software
	myADC->EMUX &= ~ADC_EMUX_EM1_M;
	myADC->EMUX |= ADC_EMUX_EM1_PROCESSOR;
	
	// enable averaging for 16 times over sampling (16)
	myADC->SAC |= ADC_SAC_AVG_16X;
	
	// set the control register to set sequence of 1
	myADC->SSCTL1 = ADC_SSCTL1_IE1 | ADC_SSCTL1_END1;
		
	// set interrupt mask register to enable interrupts for SS1
	myADC->IM |= ADC_IM_MASK1;
		
	// set priority of ADC0 interrupt to 2
	NVIC_SetPriority(ADC0SS1_IRQn, 2);
	
	// enable the interrupt in NVIC
	NVIC_EnableIRQ(ADC0SS1_IRQn);
	
	// input source !!!!!!!!!!
	myADC->SSMUX1 = (1<<4);
	
	// enable sample sequencer 1
	myADC->ACTSS |= ADC_ACTSS_ASEN1;
}

static void init_ioexpand_buttons(void)
{
	// Setting the port B to be inputs but only GPB0-GPB3
	io_expander_write_reg(MCP23017_IODIRB_R, 0x0F);	  
	
	// GPINTEN: INTERRUPT-ON-CHANGE PINS 1 - enable : 0 - disable
	io_expander_write_reg(MCP23017_GPINTENB_R, 0x0F);	
	// DEFVAL & INTCON
	io_expander_write_reg(MCP23017_DEFVALB_R, 0xFF);	 // set comparison value 
	io_expander_write_reg(MCP23017_INTCONB_R, 0xFF);   // 
	// Setting pull up resistor
	io_expander_write_reg(MCP23017_GPPUB_R, 0x0F);
}

static void init_timer4(void)
{   
    TIMER0_Type *gp_timer;
    uint32_t base_addr = TIMER4_BASE;
    
    // Turn on the clock for the timer
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R4;

    // Wait for the timer to turn on
    while( (SYSCTL->PRTIMER & SYSCTL_PRTIMER_R4) == 0) {};
    
    // Type cast the base address to a TIMER0_Type struct
    gp_timer = (TIMER0_Type *)base_addr;

    // Turn the timers off
    gp_timer->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);  
            
    // Set timer to 32 bits
    gp_timer->CFG = TIMER_CFG_32_BIT_TIMER;

    // Clear mode bits for the timer
    gp_timer->TAMR = 0; 
			
    // Set the timer to a periodic and count down timer
    gp_timer->TAMR |= 0x02;
    gp_timer->TAMR &= ~TIMER_TAMR_TACDIR;
    gp_timer->TAILR = 1000000;

    // Enable the interupt for the timer
    gp_timer->IMR  |= TIMER_IMR_TATOIM;

    // Clear the status bits
    gp_timer->ICR  |= TIMER_ICR_TATOCINT;

    //Enable the timer
    gp_timer->CTL  |= TIMER_CTL_TAEN;
		
		//gp_timer->RIS |= TIMER_RIS_TATORIS;

    NVIC_SetPriority(TIMER4A_IRQn, 0);
    NVIC_EnableIRQ(TIMER4A_IRQn);
}

static void init_timer5(void)
{
    TIMER0_Type *gp_timer;
    uint32_t base_addr = TIMER5_BASE;
    
    // Turn on the clock for the timer
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R5;

    // Wait for the timer to turn on
    while( (SYSCTL->PRTIMER & SYSCTL_PRTIMER_R5) == 0) {};
    
    // Type cast the base address to a TIMER0_Type struct
    gp_timer = (TIMER0_Type *)base_addr;

    // Turn the timers off
    gp_timer->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN);  
            
    // Set timer to 32 bits
    gp_timer->CFG = TIMER_CFG_32_BIT_TIMER;

    // Clear mode bits for the timer
    gp_timer->TAMR = 0; 

    // Enable the interupt for the timer
    gp_timer->IMR  |= TIMER_IMR_TATOIM;

    // Set the timer to a periodic and count down timer
    gp_timer->TAMR |= 0x02;
    gp_timer->TAMR &= ~TIMER_TAMR_TACDIR;
    gp_timer->TAILR = 50000;

    // Clear the status bits
    gp_timer->ICR  |= TIMER_ICR_TATOCINT;

    //Enable the timer
    gp_timer->CTL  |= TIMER_CTL_TAEN;

    NVIC_SetPriority(TIMER5A_IRQn, 1);
    NVIC_EnableIRQ(TIMER5A_IRQn);
}

static void init_gpios(void)
{
	// initliazes LCD screen for animations
	lcd_config_screen();
	io_expander_init();
	lp_io_init();
	// this configures the PS2 Joystick GPIO
	gpio_enable_port(PS2_GPIO_BASE);
	gpio_config_enable_input(PS2_GPIO_BASE, PS2_X_DIR_MASK);
	gpio_config_enable_input(PS2_GPIO_BASE, PS2_Y_DIR_MASK);
	gpio_config_analog_enable(PS2_GPIO_BASE, PS2_X_DIR_MASK);
	gpio_config_analog_enable(PS2_GPIO_BASE, PS2_Y_DIR_MASK);
	gpio_config_alternate_function(PS2_GPIO_BASE, PS2_X_DIR_MASK);
	gpio_config_alternate_function(PS2_GPIO_BASE, PS2_Y_DIR_MASK);	
}

static void init_GPIOF(void)
{
	// enables GPIO for io_expander buttons
	gpio_enable_port(IO_EXPANDER_IRQ_GPIO_BASE);
	gpio_config_digital_enable(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	gpio_config_enable_input(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	gpio_config_falling_edge_irq(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);  // configure the GPIOF to check interrupts on falling edge
	NVIC_SetPriority(GPIOF_IRQn, 0); // set priority to 0 - highest priority
	NVIC_EnableIRQ(GPIOF_IRQn);			 // enable interrupts
}

void initialize_hardware(void)
{
	DisableInterrupts();
	
	// init GPIOs for some of hardware
	init_gpios();
	init_GPIOF();
	// init timer hardware
	init_timer4();
	init_timer5();
	// inits 4 pushbuttons on baseboard
	init_ioexpand_buttons();
	// inits joystick
	init_joystick();
	// inits the accelometer
	init_accel();
	// inits the 8 leds
	init_ioexpand_leds();
	// inits the LCD touch screen
	init_touch_screen();
	// inits the EEprom
	init_eeprom();
	
	EnableInterrupts();
}

