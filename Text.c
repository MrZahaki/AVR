/*
 * Text.c
 *
 * Created: 6/22/2019 5:41:29 PM
 *  Author: ZahakI
 */ 
#include "Text.h"

//________________________________________________________________________________________
char *SET_FONT_NAME(char	*Name){
	 char *PTR;
	 PTR=malloc(Str_len(Name)+1);
	 StrCpy(PTR,Name);
	 return	PTR;
}
//________________________________________________________________________________________
/*
Font: Default*Arial*

#define	FontSetReadSram	4
#define	FontSet_FunctionAddress	0
#define FontSet_ConstFlash	16

*/
void Text_FontSet(Nippy_Font	_FONT_){
	Nippy_Text.Default_Font=_FONT_;
};
//________________________________________________________________________________________

