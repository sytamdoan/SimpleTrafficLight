// Documentation
// CECS346 Lab 2 - Switch and LED Interface
// Description: The code runs like a traffic light. There is a green, red and yellow light with 2 switches controlling the movement of each light.
// Switch moves the light to its next order, like green to yellow, and yellow to red. Switch 2 makes the current light blink. Finally
// pressing both lights make the board move and blinks.
// Student Name: Tam Doan

// Input/Output:
//   PE0 - SW1_MASK;
//   PE1 - SW2_MASK;
//   PB0 - 0x01;
//   PB1 - 0x02;
//   PB2 - 0x04;

// Preprocessor Directives
#include <stdint.h>


// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
// Replace the ? on the following lines with address for the corresponding registers

// PORT E register definitions
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

// PORT B register definitions
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

// system control register RCGC2 definition
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// define constants and alies
#define LIGHT 		GPIO_PORTB_DATA_R
#define SW 				GPIO_PORTE_DATA_R
#define SW1_MASK 	0x01          // PE0 for switch 1
#define SW2_MASK 	0x02          // PE1 for switch 2
#define RED 			0x04			    // PB2 for red LED
#define YELLOW 		0x02				  // PB1 for yellow LED
#define GREEN 		0x01					// PB0 for green LED

// Function Prototypes - Each subroutine defined
void Delay(uint8_t n_50ms);
void PortB_Init(void);
void PortE_Init(void);

int main(void) {
	// Initialize GPIO Ports B, E
  PortB_Init();
	PortE_Init();
	// Initial state: Green LED on, the other two LEDs off
  LIGHT = GREEN;

	while(1) {
		Delay(2);
		
		// Check the following conditions and set Out appropriately:
		// If the sw1 is pressed, the currently on LED will be turned off, the next LED will be turned on. 
    // If sw2 is pressed, currently on LED will flash at a speed of 50 ms.
    // If both sw1 and sw2 are pressed, current led will flash once at a speed of 50 ms 
		//    and then move to the next one.
		
		if(SW & SW1_MASK) { //if sw1 is pressed
			if(LIGHT == RED) {
				LIGHT = GREEN;
				Delay(2);
			} else {
				LIGHT = LIGHT << 1;
				Delay(2);
			}
		}
		
		if(SW & SW2_MASK) { //if sw2 is pressed
			if(LIGHT == RED) {
				LIGHT &= ~LIGHT;
				Delay(1);
				LIGHT = RED;
				Delay(1);
			} else if(LIGHT == GREEN){
				LIGHT &= ~LIGHT;
				Delay(1);
				LIGHT = GREEN;
				Delay(1);
			} else {
				LIGHT &= ~LIGHT;
				Delay(1);
				LIGHT = YELLOW;
				Delay(1);
			}
		}
		
		if(SW & SW2_MASK & SW1_MASK) { 
			if(LIGHT == RED) {
				LIGHT &= ~LIGHT;
				Delay(1);
				LIGHT = RED;
				Delay(1);
				LIGHT &= ~RED;
				Delay(1);
				LIGHT = GREEN;
				Delay(1);

			} else if(LIGHT == GREEN){
				LIGHT &= ~LIGHT;
				Delay(1);
				LIGHT = GREEN;
				Delay(1);
				LIGHT &= ~GREEN;
				Delay(1);
				LIGHT = YELLOW;

			} else {
				LIGHT &= ~LIGHT;
				Delay(1);
				LIGHT = YELLOW;
				Delay(1);
				LIGHT &= ~YELLOW;
				Delay(1);
				LIGHT = RED;

			}
		}

	}

}

// Subroutine to initialize port B pins for output
// PB2,PB1,PB0 are outputs for red, green, and yellow LEDs
// Inputs: None
// Outputs: None
void PortB_Init(void){
		
	SYSCTL_RCGC2_R |= 0x00000002;
	while ((SYSCTL_RCGC2_R&0x00000002)!=0x00000002){}
	
  GPIO_PORTB_AMSEL_R &= ~0x3F;
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF; 
  GPIO_PORTB_DIR_R |= 0x3F;    
  GPIO_PORTB_AFSEL_R &= ~0x3F; 
  GPIO_PORTB_DEN_R |= 0x3F;    
}

// Subroutine to initialize port E pins PE0 & PE1 for input
// Inputs: None
// Outputs: None
void PortE_Init(void){
	
	SYSCTL_RCGC2_R |= 0x00000010;
	while ((SYSCTL_RCGC2_R&0x00000010)!=0x00000010){}
		
  GPIO_PORTE_AMSEL_R &= ~0x03; 
  GPIO_PORTE_PCTL_R &= ~0x000000FF; 
  GPIO_PORTE_DIR_R &= ~0x03;   
  GPIO_PORTE_AFSEL_R &= ~0x03; 
  GPIO_PORTE_DEN_R |= 0x03;    
}

// Subroutine to wait about 0.05 sec
// Inputs: None
// Outputs: None
// Notes: the Keil simulation runs slower than the real board
void Delay(uint8_t n_50ms) { unsigned long volatile time;
	time = 727240*100/91;
  while(time){
		time--;
  }
	for (time=0;time<1000;time=time+10) {
	}
}