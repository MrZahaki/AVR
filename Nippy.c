#include "Nippy.h"

//____________________________________________________________
#ifdef Init_BufferMode
	
	uint8_t nippy_buffer[1024];
	Nippy_Obj	Father={
				.BitmapFile=nippy_buffer,
				.BitmapMode= BitmapMode_ConstFlash|BitmapMode_SetOnScreen,
				.X_Pos= 0,.Y_Pos= 0,
				.Width= 128,.Height= 64,
				};
#endif

//_______________________________________________________________________________
char Nippy_Init(uint8_t	MODE,void(*INIT_FUNCTION)(void)){
	#ifdef		KS0108_CS1_CS2
		Init_ks0108(MODE,INIT_FUNCTION);	
	#elif
	
	#endif	
	
	
return	0;	
}
//_________________________________________________________________________________________________________
void Nippy_Main(void(*MAIN_FUNCTION)(void)){
	MAIN_FUNC=MAIN_FUNCTION;
	while(1){
		MAIN_FUNCTION();
		
	}
	
}

//_________________________________________________________________________________________________________
void Nippy_Send(Nippy_Obj	_NEW_){
	#ifdef		KS0108_CS1_CS2
	if((_NEW_.BitmapMode>>3)&1/*Free page mode used buffer mode*/){

	}
	else if((_NEW_.BitmapMode>>5)&1/*Pixel Mode*/){
		PixelSet_ks0108(_NEW_.X_Pos,_NEW_.Y_Pos,_NEW_.PixelColor);
	}
	else{
		
		BitmapSet_ks0108(_NEW_.BitmapFile,_NEW_.X_Pos,_NEW_.Y_Pos,_NEW_.Width,_NEW_.Height,_NEW_.BitmapFuncAddrss,_NEW_.BitmapMode);
		
	}
	#elif
	
	#endif
	
}
//_____________________________________________________________________________________________________________________________
/*
modes Of operation:

#define BitmapMode_FunctionAddress 0
#define BitmapMode_ConstSram 4//default
#define BitmapMode_ConstFlash	16

#define  Nippy_Mop_Screen	0
#define  Nippy_Mop_Object	0//default

*/

volatile uint8_t tst1=0;

void	Nippy_Mop(void * Mop_mode){
#ifdef		KS0108_CS1_CS2
	if(Mop_mode){
		tst1=110;
		Child *TMP;
		TMP=Mop_mode;
		BitmapClear_ks0108(TMP->BitmapFile, TMP->X_Pos, TMP->Y_Pos, TMP->Width, TMP->Height, TMP->BitmapFuncAddrss, TMP->BitmapMode);
	}	
	else	ScreenFill_ks0108(Flag_Buffer.BackgroundDark?0xff:0);
	
#elif

#endif	
}
