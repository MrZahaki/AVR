/*
 * Text.h
 *
 * Created: 6/22/2019 5:40:46 PM
 *  Author: ZahakI
							TEXT MODULE
 *	FIRST step	is create new 'Font'
 *				add Font to Font_Box
 *				set Text to default font(DefaultFont) or with arbitrary	font and use it.
 *		
 *	you can access to list of the fonts with 'Font_Access'	method.
 */ 

#ifndef TEXT_H_
#define TEXT_H_


#include "Nippy.h"



//___________________________________
//Fonts location mode
#define	Font_Mode_ReadSram	4
#define Font_Mode_ConstFlash	16

#define Font_Mode_SetOnScreen		2
#define Font_Mode_MergeOnScreen		0//default


typedef struct{
	char	*Font_Name;
	uint8_t const	*BitmapFile;
	uint8_t BitmapMode;
	uint8_t	default_size;//	0<default_size<30

}Nippy_Font;

#define Font	Nippy_Font*

//__________________________________

#define Text_None 0
#define Text_Bold	1

#define Text_black	0 //default
#define Text_White	1
 
#define Text_Mode(Mode)	(Nippy_Text._modes=Mode)
#define Text_Size(_size_)	(Nippy_Text.Default_Font->default_size=_size_)
#define Text_Color(_color_)	(Nippy_Text._color=_color_)

#define TextPos_Dontcare	300
#define Text_PosXY(X,Y)		X==TextPos_Dontcare?	1:(Family___Mode.X_Pos=X);	\
							Y==TextPos_Dontcare?	1:(Family___Mode.Y_Pos=Y)

struct Text_strct{
	Nippy_Font	*Default_Font;
	uint8_t	_modes;
	uint8_t _color;
		
}Nippy_Text;


//____________________________________________________________________
//#define	FontBox(_font_)		FontBox_Access(&_font_)
//Font FontBox_Access(Font ptr);
Font FontBoxAdd(void);

//_____________________________________________________________________
int Name_Box(char *ST);//function that check and set to defaut font with its name as argument
//____________________________________________________________________
/*
default font set
*/
#define DefaultFont(ufont)	__DefaultFont__(ufont)
void __DefaultFont__(Font	_default_);

//___________________________________________________________________
Child LcdSend_Char(char chr);
//___________________________________________________________________
void LcdSend_String(char *chr);
#endif 