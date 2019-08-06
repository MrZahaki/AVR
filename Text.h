/*
 * Text.h
 *
 * Created: 6/22/2019 5:40:46 PM
 *  Author: ZahakI
 */ 


#ifndef TEXT_H_
#define TEXT_H_

#include "NiPpY_str.h"
#include "Fonts.h"


//___________________________________
//Fonts location mode
#define	Font_Mode_ReadSram	4
#define	Font_Mode_FunctionAddress	0
#define Font_Mode_ConstFlash	16


typedef struct{
char	*Font_Name;
uint8_t const	*BitmapFile;
uint8_t BitmapMode;

}Nippy_Font;

//__________________________________
struct Text_strct{
	Nippy_Font	Default_Font;
		
}Nippy_Text;

//____________________________________________________________________
char *SET_FONT_NAME(char	*Name);//used for setting Font_Name attribute  in Nippy_Font object

#endif 