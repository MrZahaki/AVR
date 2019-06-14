#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "bmp.h"
#include "Nippy.h"

//volatile float X=0,Y=0,buff;
volatile int i=33;
volatile uint8_t X,buff;
float		Y=0;

void nippy(void);
void nippy_main(void);

int main(void)
{

    Nippy_Init(Nippy_Init_LightBackground,nippy);
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
		
		//ScreenFill_ks0108(Flag_Buffer.BackgroundDark);
		
		//for(Y=0,X=127;X>-1;X--,Y+=0.1){
		//for(Y=0;Y<15;Y++){
		//BitmapSetFLASH_ks0108(bmp1,40,30,52,45,Bitmap_Set);
		//_delay_ms(1000);
		
		//(Y==63)?(Y=0):1;
		//}
		Nippy_Main(nippy_main);
		
}

Child	world;
Child	mycar;
Child	car_out0;
Child	pixel;

void nippy(void){
	world.BitmapFile=example;
	world.BitmapMode=BitmapMode_MergeOnScreen|BitmapMode_ConstFlash;
	world.Width=128;
	world.Height=64;
	world.X_Pos=0;
	world.Y_Pos=0;
	
	
	
	mycar.BitmapFile=car;
	mycar.BitmapMode=BitmapMode_ConstSram|BitmapMode_SetOnScreen;
	mycar.X_Pos=0;
	mycar.Y_Pos=23	;
	mycar.Width=15;
	mycar.Height=13;
	
	car_out0.BitmapMode=BitmapMode_ConstSram|BitmapMode_MergeOnScreen;
	car_out0.X_Pos=0;
	car_out0.Y_Pos=28;
	car_out0.Width=7;
	car_out0.Height=3;
	
	/*pixel.BitmapMode=BitmapMode_SetPixelOnXY;
	pixel.PixelColor=PixelColor_Dark;
	pixel.X_Pos=70;
	pixel.Y_Pos=63;
	Nippy_Send(pixel);*/
	Nippy_Send(mycar);

 
}

void nippy_main(void){
	static	int j=0;


	/*for(i=0;i<128;i++){
		j^=1;
		if(j)	car_out0.BitmapFile=out0;
		else	car_out0.BitmapFile=out1;
		
		mycar.X_Pos=i;
		if(i>7)car_out0.X_Pos=mycar.X_Pos-6;
		//circuit.X_Pos=2*i;
		
		
		Nippy_Send(world);
		
		Nippy_Send(mycar);
		if(i>7)	Nippy_Send(car_out0);	
		_delay_ms(100);
		Nippy_Mop(&car_out0);
		//Nippy_Mop(&mycar);
		
		//Nippy_Send(world);
		
	}*/
	
}
