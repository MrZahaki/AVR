#ifndef NIPPY_H_
#define NIPPY_H_

#define KS0108_CS1_CS2	
//#define Init_BufferMode	


#ifdef KS0108_CS1_CS2
#include "ks0108.h"
#endif

void(*MAIN_FUNC)(void);
//________________________________________________________________________________________________
#define BitmapMode_SetOnScreen		2
#define BitmapMode_MergeOnScreen		0//default

#define BitmapMode_FunctionAddress 0
#define BitmapMode_ConstSram 4//default
#define BitmapMode_ConstFlash	16

#define BitmapMode_FreePage 8

typedef struct{
	uint8_t const	*BitmapFile;
	uint8_t BitmapMode;
	//BitmapMode_ConstSram
	//BitmapMode_ConstFlash
	//BitmapMode_FunctionAddress
	//FreePage
	uint8_t	X_Pos;
	uint8_t	Y_Pos;//0~7
	uint8_t	Width;
	uint8_t	Height;
	
uint8_t (*BitmapFuncAddrss)(/*unsigned char suggested*/const void *);//bitmap function address EX:pgm_read_byte
	void (*Behave_Func)(void *);
}Nippy_Obj;
//_______________________________________________________________________________________
#define  Child	Nippy_Obj
#define Nippy_Mop()	ScreenFill_ks0108(Flag_Buffer.BackgroundDark);
//________________________________________________________________________________________________
#define  Nippy_Init_DarkBackground	1
#define  Nippy_Init_LightBackground	0

char Nippy_Init(uint8_t	MODE,void(*INIT_FUNCTION)(void));

//________________________________________________________________________________________________
void Nippy_Main(void(*MAIN_FUNCTION)(void));//start main graphic process

//________________________________________________________________________________________________
void Nippy_Send(Nippy_Obj	_NEW_);
//________________________________________________________________________________________________


#endif /* NIPPY_H_ */