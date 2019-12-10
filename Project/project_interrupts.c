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
#include "project_interrupts.h"

volatile Directions joy_dir;
volatile bool SW1_Pressed = false;
volatile bool timer_tick = false;
volatile bool mv_dragons = false;
volatile bool boss_fire = false;
volatile bool left = false;
volatile bool right = false;
volatile bool up = false;
volatile bool down = false;
volatile bool center = false;

volatile bool R = false;
volatile bool U = false;
volatile bool D = false;
volatile bool L = false;

//*****************************************************************************
// TIMER4 ISR
//*****************************************************************************
void TIMER4A_Handler(void)
{
	static int count = 0;
	
	count++;
	
	if(!(count % 5)) {
		mv_dragons = true;
	} else {
		mv_dragons = false;
	}
	
	if(!(count % 100)) {
		boss_fire = true;
	} else {
		boss_fire = false;
	}
	
	if(count >= 200) {
		timer_tick = true;
		count = 0;
	} else {
		timer_tick = false;
	}
	
	TIMER4->ICR = TIMER_ICR_TATOCINT;
}

//*****************************************************************************
// TIMER5 ISR
//*****************************************************************************
void TIMER5A_Handler(void)
{
	// begin sampling
	ADC0->PSSI |= ADC_PSSI_SS1;
	
	TIMER5->ICR = TIMER_ICR_TATOCINT;
}

//*****************************************************************************
// ADC0 SS1 ISR
//*****************************************************************************
void ADC0SS1_Handler(void) // NOT DONE
{
	float joy_x_data = ADC0->SSFIFO1 & 0xFFF; // Read 12 bits data
	float joy_y_data = ADC0->SSFIFO1 & 0xFFF; // Read 12 bits data
	int i;
	
	joy_x_data = joy_x_data * 1/4096;
	joy_y_data = joy_y_data * 1/4096;

	if (joy_x_data > 0.8484) {
		//delay
		for(i=0; i<5000; i++);
		left = true;
		right = false;
		up = false;
		down = false;
		center = false;
	} else if (joy_x_data < 0.1818) {
		//delay
		for(i=0; i<5000; i++);
		right = true;
		left = false;
		up = false;
		down = false;
		center = false;
	} else {
		if (joy_y_data > 0.8484) {
			//delay
			for(i=0; i<5000; i++);
			up = true;
			down = false;
			left = false;
			right = false;
			center = false;
		} else if (joy_y_data < 0.1818) {
			//delay
			for(i=0; i<5000; i++);
			down = true;
			up = false;
			right = false;
			left = false;
			center = false;
		} else {
			//delay
			for(i=0; i<5000; i++);
			center = true;
			down = false;
			up = false;
			right = false;
			left = false;
		}
	}
	// clear the interrupt
	ADC0->ISC |= ADC_ISC_IN1;
}

void GPIOF_Handler(void){
	char msg[80];
	uint8_t data;	
	io_expander_read_reg(MCP23017_GPIOB_R); // read GPIOB again to double check if the privious interrupt got properly clear
	data = io_expander_read_reg(MCP23017_INTFB_R);
	if(data == 0){}
	else if(data == 1){ // up SW0
		U = true;
	}
	else if(data == 2){ // down SW1
		D = true;
	}
	else if(data == 4){ // left SW2
		L = true;
	}
	else if(data == 8){ // right SW3
		R = true;
	}
	io_expander_read_reg(MCP23017_GPIOB_R);
	GPIOF->ICR |= PF0;
}



