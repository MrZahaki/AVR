#ifndef KS0108_H_
#define KS0108_H_

#include "Nippy.h"


#define _NULL 0

	
#define Screen_ColorMonochrom	0

#define Screen_BackgroundColorDark	1//see l114:nippy.h
#define Screen_BackgroundColorWhite	0

struct __Screen__{
	uint8_t	width;
	uint8_t	height;
	unsigned	color_mode:1;
	unsigned	segment_size:1;//in byte
	uint16_t	_size;
	uint8_t		background_color;
}Screen;
	
struct Tmp_Buffer{
	unsigned RS:1;
	unsigned RW:1;
	unsigned BackupRX_Flg:1;//backup RW AND RS flag
	unsigned BackgroundDark:1;
	
	unsigned X_POS:7;//0-127	(keep xy pos)
	unsigned Y_POS:3;//0-7		(keep xy pos)
	unsigned BackupXY_Flg:2;//backup X,Y POSITION  flag	[backup ready,bacup flag]
	
	uint8_t DepPort_Backup;//used in Do function
	uint8_t Data_Backup;//used in Do function
	
	
	
	//unsigned	Buffer_UseFlag:1;
	//void(*Nippy_Init)(void);
	void(*Nippy_Main)(void);
}Flag_Buffer;	

#define Init_ks0108_DarkBackground	ScreenFill_Dark
#define Init_ks0108_LightBackground	ScreenFill_Light
void Init_ks0108(uint8_t	mode,void (*Function_Call)(void));

#define		XYSet_DontMatterPos	254	//don't change 'DontMatterPos'	variable
void XYSet_ks0108(uint8_t X, uint8_t Y);

#define Read_DisplayData	0
#define Read_busy	1
uint8_t	Read_ks0108(uint8_t mode,uint8_t Xaddress,uint8_t Yaddress);
#define busy_wait()	\
while(Read_ks0108(Read_busy,_NULL,_NULL))

#define PixelSet_Dark	1
#define PixelSet_Light	0
void PixelSet_ks0108(uint8_t X,uint8_t Y,uint8_t Color);

#define ScreenFill_Dark 0xff			//screen fill function
#define ScreenFill_Light 0
void ScreenFill_ks0108(uint8_t	Color);

#define Bitmap_Set		2
#define Bitmap_Merge	0/*default mode*/

#define	BitmapReadSram	4
#define	BitmapMode_FunctionAddress	0
#define BitmapMode_ConstFlash	16
void 	BitmapSet_ks0108(const uint8_t	*BMP, char	xPos, char	yPos, uint8_t	width, uint8_t	 height,uint8_t (*BitmapFuncAddrss)(unsigned int ), uint8_t		_mode);
//****************************
/*
modes of operation:

#define	BitmapReadSram	4
#define	BitmapMode_FunctionAddress	0
#define BitmapMode_ConstFlash	16

	BITMAP File format: rows of Vertical bytes(default for ks0108 data display)(bitmap file format)
	x~0-127  y~0-63
	modes of operation for monochrome display:
bit0	BitmapSetFLASH_Full_Screen	0
		BitmapSetFLASH_Custom_Screen	1
		
bit1	Bitmap_Merge	0
		Bitmap_Set		1	

bit2	BitmapReadFlash	0
bit3	BitmapReadSram	1		


*/

void 	BitmapClear_ks0108(const uint8_t	*BMP	\
, char	xPos, char	yPos	\
, uint16_t	width, uint16_t	 height	\
,uint8_t (*BitmapFuncAddrss)(/*unsigned char suggested*/unsigned int)	\
, uint8_t		_mode);

//_______________________________________________________________________________________________


#endif
