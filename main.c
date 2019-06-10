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
		
		//ScreenFill_ks0108(ScreenFill_Dark);
		
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

void nippy(void){
	world.BitmapFile=WOLD;
	world.BitmapMode=BitmapMode_SetOnScreen|BitmapMode_ConstFlash;
	world.Width=128;
	world.Height=64;
	world.X_Pos=0;
	world.Y_Pos=0;
	
	
	
	mycar.BitmapFile=car;
	mycar.BitmapMode=BitmapMode_ConstSram|BitmapMode_SetOnScreen;
	mycar.X_Pos=0;
	mycar.Y_Pos=30;
	mycar.Width=15;
	mycar.Height=13;
	
	Nippy_Send(world);
	_delay_ms(750);
	
	/*for(i=0;i<128;i+=5){
		mycar.X_Pos=i;
		//Nippy_Send(world);
		Nippy_Send(mycar);
		_delay_ms(50);
		Nippy_Mop();
	}*/
//Nippy_Send(world);

}

void nippy_main(void){
	
	for(i=0;i<64;i+=8){
		world.Y_Pos=i;
		world.X_Pos=2*i;
		//circuit.X_Pos=2*i;
		
		
		
		Nippy_Send(world);
		_delay_ms(350);
		Nippy_Mop();

		//Nippy_Send(world);
		
	}
	
}
