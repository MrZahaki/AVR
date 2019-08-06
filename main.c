#include "main.h"
#include "bmp.h"
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
	mycar->BitmapFile=car;
	mycar->BitmapMode=BitmapMode_ConstSram|BitmapMode_MergeOnScreen; 
	mycar->X_Pos=0;
	mycar->Y_Pos=0	;
	mycar->Width=15;
	mycar->Height=13;
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
	
	tst_magnify= Magnify(mycar,3,Scale_Mode_Normal);
	//tst_magnify= Magnify(mycar,3,Scale_Mode_Normal);
	tst_magnify->X_Pos=20;
	Nippy_Send(tst_magnify);
	Nippy_Mop(tst_magnify);
	Nippy_Send(mycar);
	//Nippy_Send(*tst_magnify);
	
	
	//Magnify(mycar,2,4/*send to screen*/);
	
 
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
}
