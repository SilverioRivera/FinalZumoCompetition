/*
 * ZumoShield Robot Movement and Wireless Connection
 *
 * Reads the joystick and sends on command character
 * over Bluetooth.
 *
 
 * Created/Completed: 11/17/2026 
 * Author : Silverio Rivera-Lopez
 */ 

#define F_CPU 7372800UL    // Ready AVR Crystal: 7.3728 MHz
#define BAUD 9600		   // Bluetooth Speed: 9600 bits per second
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1) // Baud Setting Value = 47

#include <avr/io.h>
#include <util/delay.h>  // Allows me to use the delay_ms() 

// Joystick PINS for PORTB in Active LOW

#define RIGHT PB0 
#define DOWN  PB1
#define LEFT  PB2
#define UP	  PB3
#define Joystick_PINS 0x1F


// Joystick Operation

char JoystickMovement(void){
	uint8_t pins = PINB;					// Reading all PORTB pins
	if (!(pins & (1 << UP))) return 'F';	// Joystick Up is pressed  = GO FORWARD
	if (!(pins & (1 << DOWN))) return 'B';	// Joystick Down is pressed  = GO DOWN
	if (!(pins & (1 << LEFT))) return 'L';	// Joystick Left is pressed  = GO LEFT
	if (!(pins & (1 << RIGHT))) return 'R'; // Joystick Right is pressed = GO RIGHT
	return 'S';								// S character for stop if there is no movement 
} 

// LED Joystick Detector
// PC0 - PC3 active low 

void LEDAction(char cmd){
	switch(cmd){						// Checks on what command is received 
		case 'F': PORTC = 0x0E; break;  // Turn ON LED0
		case 'B': PORTC = 0x0D; break;  // Turn ON LED1
		case 'L': PORTC = 0x0B; break;  // Turn ON LED2
		case 'R': PORTC = 0x07; break;  // Turn ON LED3
		default:  PORTC = 0x0F; break;  // Turn OFF LEDs   
	}
}


// UART Initialization 

void UARTinit(void){
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);  // High Byte
	UBRR0L = (uint8_t)UBRR_VALUE;		  // Low Byte
	UCSR0B = (1 << TXEN0);				  // Transmitting is ON 
	UCSR0C = (1 << UCSZ01)|(1 << UCSZ00); // 8 data bits and 1 stop bit
}

void UARTsend(char c){
	while (!(UCSR0A & (1 << UDRE0))); // Wait until its ready to send
	UDR0 = c;						  // Send the character
}

// Main Program

int main(void){
	DDRB  &= ~Joystick_PINS;   // Inputs
	PORTB |= Joystick_PINS;    // Pullup Resistors ON
	DDRC  |= 0x0F;			   // LEDs outputs
	UARTinit();				   // Bluetooth UART setup
	
	while (1)
	{
		char cmd = JoystickMovement();  // Read the joystick
		LEDAction(cmd);					// Display on LEDs
		UARTsend(cmd);					// Send to ZumoRobot
		_delay_ms(20);					// make it responsive 
	}
}

