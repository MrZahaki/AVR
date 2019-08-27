#ifndef UART_H_
#define UART_H_

//#define size_RX_buffer	20
//extern	char SerialRxBuffer[size_RX_buffer],Rx_Buffer_ready,used_buffer_flag;

 extern void usart_initial(void);
 extern void Uart_sendchar(unsigned char data);
 //ISR(USART0_RX_vect);
 extern void Uart_sendstring(char *s);
#endif /* UART_H_ */