

#include "main.h"
#include "bmp.h"
#include <avr/interrupt.h>


//#include "uart.h"
//volatile float X=0,Y=0,buff;
/*volatile int i=33;
volatile uint8_t X,buff;
float		Y=0;*/

void nippy(void);
void nippy_main(void);

int main(void)
{
	usart_initial();
	sei();
	Nippy_Init(Nippy_Init_LightBackground,nippy);
	//Uart_sendstring("hello world\n");
	//pixel set test
	//for(i=0;i<128;i++) PixelSet_ks0108(i,0,PixelSet_Dark);

		/*for(Y=0;Y<64;Y++){
		//_delay_ms(50);
	PixelSet_ks0108(Y,Y,1);
		 }
		 for(Y=63,X=64;Y>=0,X<128;Y--,X++){
			
			 PixelSet_ks0108(X,Y,1);
		 }*/
		
//drawing sin && cos (current sin*cos)		
		/*for(Y=500,X=0;Y>-1;Y-=1){
			_delay_ms(10);
			buff=(int)32*cos((Y/127)*2*M_PI)*sin((Y/127)*8*M_PI)+31;
			PixelSet_ks0108((int)X++,(int)buff,1);
		}*/
		Nippy_Main(nippy_main);
		
		
}

Child	world;	
Child	mycar;
Child	car_out0;
Child	pixel;

Child	tst_magnify;


void nippy(void){
	
	/*world->BitmapFile=example;
	world->BitmapMode=BitmapMode_MergeOnScreen|BitmapMode_ConstFlash;
	world->Width=128;
	world->Height=64;
	world->X_Pos=0;
	world->Y_Pos=0;*/
	
	
	mycar=Pregnant_Mother(Pregnant_Mother_DisableAutoFree);
	mycar->BitmapFile=out0;
	mycar->BitmapMode=BitmapMode_ConstSram|BitmapMode_SetOnScreen; 
	mycar->X_Pos=10;
	mycar->Y_Pos=20	;
	mycar->Width=0xB;
	mycar->Height=0x10;
	
	//tst_magnify=mycar;
	/*Nippy_Send(mycar);
	tst_magnify=Magnify(mycar,3,Scale_Mode_YCoordinate);
	tst_magnify->X_Pos=40;
	Nippy_Send(tst_magnify);*/
	//Magnify()
	/*
	car_out0->BitmapMode=BitmapMode_ConstSram|BitmapMode_MergeOnScreen;
	car_out0->X_Pos=0;
	car_out0->Y_Pos=28;
	car_out0->Width=7;
 	car_out0->Height=3;*/
	
	/*pixel->BitmapMode=BitmapMode_SetPixelOnXY;
	pixel->PixelColor=PixelColor_Dark;
	pixel->X_Pos=70;
	pixel->Y_Pos=63;
	Nippy_Send(pixel);
	*/
	//
	
	//tst_magnify= Magnify(mycar,3,Scale_Mode_Normal);
	//tst_magnify->X_Pos+=10;
	//tst_magnify= Magnify(mycar,5,Scale_Mode_Normal);
	//mycar->Y_Pos=4; 
	//Nippy_Send(tst_magnify);
	
	
	
	//Nippy_Mop(mycar);
	
	//Nippy_Send(*tst_magnify);
	
	
	//Magnify(mycar,2,4/*send to screen*/);
	//LcdSend_Char('g');
	//char	a='a';
	/*while(a<110){
	LcdSend_Char(a++);
	_delay_ms(100);
	}*/
	//Uart_sendchar('\n');
	//Nippy_Text.Default_Font->default_size=4;
	
// 	Name_Box("Bradley") ;
// 	//Text_Mode(Text_Bold);
// 	//Uart_sendstring(Nippy_Text.Default_Font->Font_Name);
// 	LcdSend_String("in ");
// 	
// 	Name_Box("arial") ;
// 	//Text_Mode(Text_Bold);
// 	//Uart_sendstring(Nippy_Text.Default_Font->Font_Name);
// 	LcdSend_String("the ");
// 	
// 	Name_Box("calisto") ;
// 	//Text_Mode(Text_Bold);
// 	//Uart_sendstring(Nippy_Text.Default_Font->Font_Name);
// 	LcdSend_String("name of ");
		//LcdSend_String("hello");
	

		//Text_Size(1);
		Text_Mode(Text_Bold);
		//Uart_sendstring(Nippy_Text.Default_Font->Font_Name);
		LcdSend_String("GOD hello  im hossein " );
	
	
	//Uart_sendstring(Nippy_Text.Default_Font->Font_Name);

}

void nippy_main(void){//dont use delay in here
// 	static int x=1;
// 	Nippy_Mop(tst_magnify);
// 	Child_Control(tst_magnify,ChildControl_KILLALL);
// 	tst_magnify= Magnify(mycar,x++,Scale_Mode_Normal);
// 	Nippy_Send(tst_magnify);
// 	_delay_ms(1000);
	
	/*mycar->X_Pos++;
	Nippy_Mop(&mycar);
	
	Nippy_Send(mycar);
	_delay_ms(100);*/
	
	/*tst_magnify= Magnify(mycar,x++,Scale_Mode_XCoordinate);
	Nippy_Send(tst_magnify);
	_delay_ms(300);
	Nippy_Mop(tst_magnify);
	free(tst_magnify->BitmapFile);*/
	/*if(Rx_Buffer_ready){
			LcdSend_String(SerialRxBuffer);
			used_buffer_flag=1;
	}*/
}
