#include "uart.h"
#include "main.h"

//atmega1284



//char SerialRxBuffer[size_RX_buffer],Rx_Buffer_ready=0,used_buffer_flag=0;

void handle_uart_data(char ch); 
 //___________________________________________________________________________________________________________________________
void usart_initial(void){

UCSR0A=(0<<RXC0)|(0<<TXC0)|(0<<UDRE0)|(0<<FE0)|(0<<DOR0)|(0<<UPE0)|(1<<U2X0)|(0<<MPCM0);
UCSR0B=(1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<RXB80)|(0<<TXB80);
UCSR0C=(0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
//ASYNCHRON MODE 
//U2X IS ENABLED
//LENGTH OF FRAME IS 8 BIT
//PARITY MODE OFF
//RX COMPLATE INTERRUPT ENABLED = 1
UBRR0=207;//~2400 b/s	fosc=4 M Hz
}
//_________________________________________________________________________________________________________________________________
void Uart_sendchar(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=data;
}
//___________________________________________________________________________________________________________________________________
void Uart_sendstring(char *s){
	unsigned int i=0;
	while(s[i]) Uart_sendchar(s[i++]);
}

//_____________________________________________________________________________________________________________________________

/*ISR(USART0_RX_vect){
	char status,_data;
	status=UCSR0A;
	_data =UDR0;
	if(( status & ((1<<FE0)|(1<<DOR0 )) )==0){ 
	handle_uart_data(_data);
	}
}

//___________________________________________________________________________________________________________________________________________
void handle_uart_data(char ch){
    static unsigned char count=0;
	unsigned char flag__=0;
	
	
	if(ch==0X0D){
		flag__=1;
		ch='\0';
	}
	if(ch==0X08) ch=0X00;
	
	if( (count==0) && (used_buffer_flag==0) ){ // Cleaning Serial Rx Buffer if the data is usable
		while(SerialRxBuffer[count]) SerialRxBuffer[count++]='\0';
		count=0;
	}
	Rx_Buffer_ready=0;
	SerialRxBuffer[count]=ch;
	
	if( (++count==size_RX_buffer) || (flag__==1) ){
		count=0;
		Rx_Buffer_ready=1;
	}
	
	
	Uart_sendchar(SerialRxBuffer[count-1]);
}*/
