#ifndef NIPPY_H_
#define NIPPY_H_

#include <stdlib.h>

#include "uart.h"


#define KS0108_CS1_CS2	
//#define Init_BufferMode	


#ifdef KS0108_CS1_CS2
#include "ks0108.h"
#include "Text.h"
#endif
//___________________________________screen_struct
#define Screen_ColorMonochrom	0//4bit token
#define Screen_ColorRGB	1

void(*MAIN_FUNC)(void);
//________________________________________________________________________________________________
#define BitmapMode_SetOnScreen		2
#define BitmapMode_MergeOnScreen		0//default
/*
BitmapMode_FunctionAddress integer argument changes from 0 to (size of bitmap)
*/
#define BitmapMode_FunctionAddress 0
#define BitmapMode_ConstSram 4//default
#define BitmapMode_ConstFlash	16

#define BitmapMode_FreePage 8

#define	BitmapMode_SetPixelOnXY	32//in this mode before setting pixel behavior function starts to run
#define	PixelColor_Dark	1
#define PixelColor_Light	0


typedef struct{
	uint8_t 	*BitmapFile;
	uint8_t BitmapMode;
	//BitmapMode_ConstSram
	//BitmapMode_ConstFlash
	//BitmapMode_FunctionAddress
	//FreePage
	//BitmapMode_SetPixelOnXY **Setting pixel on desired address 
	unsigned PixelColor:1;
	/*
	used in pixel set mode
	
	#define PixelSet_Dark	1
	#define PixelSet_Light	0
	*/
	uint8_t	X_Pos;
	uint8_t	Y_Pos;//0~7
	uint16_t	Width;
	uint16_t	Height;
	
	
	
	uint8_t (*BitmapFuncAddrss)(unsigned int);//bitmap function address EX:pgm_read_byte
	void (*Behave_Func)(void *);
}Nippy_Obj;
//*************************************************************
#define  Family_Mode_Enable	1
#define  Family_Mode_Disable	0
struct Nippy_state{
	unsigned Auto_Free_Mode:1;
	
}Family___Mode;
//_______________________________________________________________________________________
#ifdef	Init_BufferMode
extern Nippy_Obj*	Father;
#endif

#define  Child	Nippy_Obj*
//#define  BornedChild	Nippy_Obj*//its an pointer

#define	Pregnant_Mother_EnableAutoFree	1
#define	Pregnant_Mother_DisableAutoFree	0

#define Pregnant_Mother(Mode)	Natural_Child_Birth(Mode)
#define	Natural_Child_Birth(Mode)  ChildCTL_Add( malloc(sizeof( Nippy_Obj)),Mode )	
#define protect(pointer_obj) (*ChildProtect(pointer_obj))

//#define Nippy_Mop()	ScreenFill_ks0108(Flag_Buffer.BackgroundDark);
//________________________________________________________________________________________________
#define  Nippy_Init_DarkBackground	0x1
#define  Nippy_Init_LightBackground	0

#define	 Nippy_Init_ProtectPeriod350	2
#define	 Nippy_Init_ProtectPeriod1000	4
#define	 Nippy_Init_ProtectPeriod100	0 //(default mode)

char Nippy_Init(uint8_t	MODE,void(*INIT_FUNCTION)(void));

//________________________________________________________________________________________________
void Nippy_Main(void(*MAIN_FUNCTION)(void));//start main graphic process

//________________________________________________________________________________________________
/*
EG0:
pixel.BitmapMode=BitmapMode_SetPixelOnXY;
pixel.PixelColor=PixelColor_Dark;
pixel.X_Pos=70;
pixel.Y_Pos=63;
Nippy_Send(pixel);


*/

void Nippy_Send(Child	_NEW_);
//________________________________________________________________________________________________
#define  Nippy_Mop_Screen	0
#define  Nippy_Mop_Object	0//default

/*
Nippy_Mop(Nippy_Mop_Object|&world);
Nippy_Mop(Nippy_Mop_Screen);
*/
void	Nippy_Mop(void * Mop_mode);
//_______________________________________________________________________________________________
/*
dest_obj will be equal to source_obj
*/
void Set_Equal__(Child dest_obj,Child source_obj); 

//_____________________________________________________________________________________________
#define	ChildCTL_Add_AutoFree	1
uint16_t Protect_period;
uint32_t cycle;

Child ChildCTL_Add(void *ptr,uint8_t	_mode);
//_____________________________________________________________________________________________
void ChildCTL_Free(void *ptr);
//_____________________________________________________________________________________________
void ChildCTL_AutoFree(uint32_t current_cycle );
//_____________________________________________________________________________________________
int ChildCTL_Check(void *ptr);
//_____________________________________________________________________________________________
Nippy_Obj * ChildProtect(void *ptr);
//_____________________________________________________________________________________________
#define ChildAutoFree_Enable	0//default
#define ChildAutoFree_Disable	1//default

#define ChildControl_AutoFree	2//need for access to child auto free mode 
#define ChildAutoFree_AddAcsCntr(x)	(8<<x)

#define ChildControl_KILL	4//lock off door

#define ChildControl_KILLALL	8

void Child_Control(void* _child,uint16_t	_mode);

//______________________________________________________________________________
#define FamilyMode_DisableAutoFree	0
#define FamilyMode_EnableAutoFree	1
void FamilyMode(uint8_t	_mode);

#endif /* NIPPY_H_ */