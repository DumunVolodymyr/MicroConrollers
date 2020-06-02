.include "m2560def.inc"

.macro mOut 
    .if @0 < 0x40 
        out @0, @1 
    .elif ((@0 >= 0x60) && (@0 < SRAM_START)) 
        sts @0,@1 
    .else 
       .error "mOut: Invalid I/O register address" 
    .endif 
.endmacro 

.macro mIn 
    .if @1 < 0x40 
        in @0, @1 
    .elif ((@1 >= 0x60) && (@1 < SRAM_START)) 
        lds @0,@1 
    .else 
       .error "mIn: Invalid I/O register address" 
    .endif 
.endmacro 



.DSEG
.def	_temp1		=r16
.def	_temp2		=r17
.def	_temp3		=r18

.def	_logic		=r24		;What algo is enabled 
.def	_algo1Count	=r19
.def	_algo2Count	=r20

.def	_tempA11	=r21		;temp for algo1 and algo 2
.def	_tempA12	=r22
.def	_tempA2		=r23
.def	_algo2  	=r25
.def	_buz1   	=r26
.def	_buz2  	  	=r27
.def	_buz3  		=r28
.def	_logic2  	=r29
.def	_tempA22  	=r30
.def	_tempA4  	=r31


.CSEG
	.org	$0000
	rjmp	Reset

	.org	$0054			
	rjmp 	TIMER3_COMPA
	

	
Reset:	

	;Init of stack
	ldi		_temp1, Low(RAMEND)
	out		SPL, _temp1
	ldi		_temp1, High(RAMEND)
	out		SPH, _temp1
	ldi _temp2, 0x00
	;Setting ports A and F as output
	;ldi		_logic, 0b0000_0000
	
	ser		_temp1
	mOut	DDRA, _temp1
	mOut	DDRF, _temp1
	
	clr		_temp1		
	mOut	PORTA, _temp1
	mOut	PORTF, _temp1
	
	mOut DDRK, _temp2
	ldi _temp2, 0b0000_0110
	mOut PORTK, _temp2

	
	;Buzzer PB0 as output
	sbi		DDRB, PB0
	cbi		PORTB, PB0
	
	cli
	ldi _temp1, 0x00
	mOut	TCCR3A, _temp1
	ldi		_temp1, (1 << WGM32) | (1 << CS32) | (1 << CS30) ;CTC mode for Timer 4 & prescaler 1024
	mOut	TCCR3B, _temp1
	ldi		_temp1, (1 << OCIE3A)	;enable interrupts on compare match
	mOut	TIMSK3, _temp1
	ldi		_temp1, 0b0000_1101	;0011110100001000 - 0x3D08 - 15624
	mOut	OCR3AH, _temp1
	ldi		_temp1, 0b0000_0000	; 0.85 sec - compare 
	mOut	OCR3AL, _temp1
	sei
	rjmp main


TIMER3_COMPA:
	ldi _tempA4, 0b0000_0000
	mOut	PORTF, _tempA4		;Turn off all leds
	mOut	PORTA, _tempA4
	sbrc	_logic, 0		;if algo1 flag is set
	rcall	Algo1
	sbrc	_logic2, 2		;if algo1 flag is set
	rcall	Algo2
	reti

main:
	sbrs	_logic, 0
	mIn _tempA2, PINK
	sbrs	_logic2, 2
	mIn _tempA22, PINK

	sbrs _tempA2, 0
	rjmp Start1
	sbrs _tempA22, 2
	rjmp Start2
	rjmp main


Algo1:
	inc		_algo1Count			;Increment
	cpi		_algo1Count, 9		;If algo is over
	breq	EndAlgo1
	mOut	PORTA, _temp3
	lsr	_temp3	;Write to the port
	ret
	
EndAlgo1:						;Ending Algo
	ldi _logic, 0b0000_0000
	ldi _tempA2, 0b0000_001
	clr _algo1Count;			;clear temp registry
	ret

Start1:
	ldi _logic, 0b0000_0001
	ldi _temp3, 0b1000_0000
	ldi _tempA2, 0b0000_0001
	rjmp Buzzer

	
	
Start2:
	ldi _logic2, 0b0000_0100
	ldi _tempA11, 0b1000_0000
	ldi _tempA12, 0b0000_0001
	ldi _tempA22, 0b0000_0100
	
	
	rjmp Buzzer


Algo2:
	inc		_algo2Count			;Increment
	cpi		_algo2Count, 5		;If algo is over
	breq	EndAlgo2
	ldi _algo2, 0b0000_0000
	clr		_algo2				;Combining regs _tempA11 and _tempA12 to _temp1
	or		_algo2, _tempA12
	or		_algo2,	_tempA11
	mOut	PORTF, _algo2
	lsr	_tempA11	;Write to the port
	lsl	_tempA12
	ret


EndAlgo2:						;Ending Algo
	ldi _logic2, 0b0000_0000
	ldi _tempA22, 0b0000_0101
	clr _algo2Count;			;clear temp registry
	ret
	
Buzzer:						;Buz for 200ms after the button is pressed
	sbi		PORTB, PB0
	rcall	delay200ms	
	cbi		PORTB, PB0
	rjmp	main
	
delay200ms:
	ldi		_buz1, 0x00
	ldi		_buz2, 0xC4
	ldi		_buz3, 0x09

delay:	
	subi	_buz1, 1
	sbci	_buz2, 0
	sbci	_buz3, 0
	brne	delay
	ret