/*
 * Text.c
 *
 *	
 * Created: 6/22/2019 5:41:29 PM
 *  Author: ZahakI
 *	
 *	
 *	The default	font is set in Nippy_Init method (loc:Nippy.c);
 *
 */ 

#include "Text.h"

//___________________________________Controling Fonts_____________________________________________________
typedef struct {
	Nippy_Font **Font_BUF;//used in index remover function
	int Font_num;
}FONT_Var;
FONT_Var __FontBox__={.Font_BUF=0,.Font_num=0};//struct
//________________________________________________________________________________________
/*
	Font: Default*Arial*

	#define	FontSetReadSram	4
	#define	FontSet_FunctionAddress	0
	#define FontSet_ConstFlash	16

*/
void __DefaultFont__(Font	_default_){
		Nippy_Text.Default_Font=_default_;
};
//________________________________________________________________________________
int FontBox_Check(void * ptr){//variable address
	 unsigned int _cntr=0;
  for(;_cntr<__FontBox__.Font_num;_cntr++)
      if(*(__FontBox__.Font_BUF+_cntr) == (long)ptr) return 1;
  if( (ptr==0) || (__FontBox__.Font_num==0) ) return 0;
  return 0;   
}
//________________________________________________________________________________
Font FontBox_Name_Check(char *ST){
		unsigned int _cntr=0;
		Font	_ft_;
		for(;_cntr<__FontBox__.Font_num;_cntr++){
			_ft_=(Font)(*(__FontBox__.Font_BUF+_cntr));
			
			if(	 StrComp( 	(_ft_->Font_Name) ,ST)	){				
				return _ft_;
			}//if
		}//for

		if( (ST==0) || (__FontBox__.Font_num==0) ) return 0;
		return 0;

}
//____________________________________________________________________________
int Name_Box(char *ST){
	Font	_ft_;
	ST=ToLower(ST);
	
	_ft_=FontBox_Name_Check(ST);
	if(	_ft_ ){		
		__DefaultFont__(_ft_);
		return	1;
	}
	return	0;
}

//______________________________________________________________________________________
Font FontBoxAdd(void){// variable address
	unsigned int _cntr=0;
	Font ptr;
	/*for(;_cntr<__FontBox__.Font_num;_cntr++)
	if(*(__FontBox__.Font_BUF+_cntr) == 0){
		ptr=__FontBox__.Font_BUF[_cntr]=calloc(1,sizeof(Nippy_Font));
		return	ptr ;
	}*/
	
	__FontBox__.Font_BUF =realloc(__FontBox__.Font_BUF ,(__FontBox__.Font_num+1)*sizeof (Font ));
	ptr=__FontBox__.Font_BUF[__FontBox__.Font_num++]=calloc(1,sizeof(Nippy_Font));
	
	__DefaultFont__(ptr);
	return	ptr;
}
//_______________________________________________________________________________
//Add new variable in list
/*Font FontBox_Access(void){// variable address
	if(	FontBox_Check(ptr)	){
		__DefaultFont__(ptr);
		return	ptr;
	}
	
	FontBoxAdd();
	
	//Uart_sendstring("font box access:");
	return ptr;
}*/

//________________________________________________________________________________________


//________________________________________________
uint16_t Read_Std(uint16_t _index){
	_index=( (Nippy_Text.Default_Font->BitmapMode>>2)&1 )?/*READ SRAM*/ Nippy_Text.Default_Font->BitmapFile[_index] :	\
	/*READ From FLASH*/pgm_read_byte(&Nippy_Text.Default_Font->BitmapFile[_index]);
	return	_index;
	
}
//_______________________________________________bitmap function address
//uint8_t Font_ReadBitmap(char _inp,uint16_t	_data){	//read font bitmap contents(BitmapFuncAddrss)
	
//}
//_________________________________________________________________________

/*
#define	 LcdSend_Char_Merge	0//default
#define	 LcdSend_Char_Set	2
*/

Child LcdSend_Char(char chr){//with default font format
	
#define _FONT_HEIGHT_()	Read_Std(1)	
#define _FIRST_CHAR_()	Read_Std(2)
#define _CHAR_NUMBER_()	Read_Std(3)	

	
	 static Child _text_=0;
	 Child	_out,*_buff;
	uint8_t	First_char;
	uint16_t	cntr160,counter161,offset;

	//if(chr==10/*\n*/)	
		
	if(!_text_){
			_text_=Pregnant_Mother(Pregnant_Mother_DisableAutoFree);
			_text_->BitmapFile=0;//preparing bitmapfile for use in realloc function 
	}
	_text_->BitmapMode= (Nippy_Text.Default_Font->BitmapMode & 3/*just accept set or merge on screen modes*/) |	BitmapMode_ConstSram;
	
	
	
	
	
	//_text_->BitmapFuncAddrss=Font_ReadBitmap;
	//see format of your font bitmap file   
	_text_->Height=	_FONT_HEIGHT_();//font height
	//Uart_sendchar(_text_->Height);
	//check validate input character 
	
	First_char=_FIRST_CHAR_();//read second element in bitmap file
	//Uart_sendchar(First_char);
	if(chr<First_char){
			if (chr==10 /*\n*/ ){
					 Family___Mode.Y_Pos+=_text_->Height-2;
					 Family___Mode.X_Pos=0;
			}
			return	0;
	}
	offset=chr-First_char;
	_text_->Width=Read_Std(offset+4);
	//Uart_sendchar(_text_->Width);	
	
	
	
	//mohasebe ye daghighe makon shorouee bitmape harfe madde nazar
	for(cntr160=0,counter161=0;cntr160<offset;cntr160++)	counter161+=Read_Std(cntr160+4);
	//Uart_sendchar(counter161);	
	
	//calculate size of output
	
	cntr160=(	(_text_->Height%8==0)?	 _text_->Height / 8	:	( (int)_text_->Height/8 +1)	);
	offset=	_text_->Width *cntr160;
	counter161*=cntr160;//height byte size
	
	_text_->BitmapFile=realloc(_text_->BitmapFile,offset);
	//Uart_sendchar(offset);	
	
	counter161=4+_CHAR_NUMBER_()+counter161;
	//Uart_sendchar(counter161);	
	
	for(cntr160=0;cntr160<offset/*size of out*/;cntr160++){//read character data

		if(Screen.color_mode==Screen_ColorMonochrom){//screen color mode
			
		_text_->BitmapFile[cntr160]=(	(Nippy_Text._color==Text_black)? \
										Read_Std(counter161+cntr160):	 \
										~Read_Std(counter161+cntr160)
									);
		}
	}
	
	_buff=_text_;
	if(	Nippy_Text._modes&1/*Text_Bold*/	){	
		_buff=Magnify( _text_ ,2 , Scale_Mode_XCoordinate );
	}
	//Nippy_Send(_buff);
	
	if(	Nippy_Text.Default_Font->default_size !=1	)	_out=Magnify( _buff , Nippy_Text.Default_Font->default_size , Scale_Mode_Normal );
	else	_out=_buff;
	
	
	if( (Family___Mode.X_Pos+_out->Width)>=Screen.width){//check position on the screen
		Family___Mode.X_Pos=0;
		Family___Mode.Y_Pos+=_out->Height-3;
	}
	if(Family___Mode.Y_Pos+_out->Height >= Screen.height){
		Family___Mode.Y_Pos=0;
		Family___Mode.X_Pos=0;
	}
	_out->X_Pos=Family___Mode.X_Pos;
	_out->Y_Pos=Family___Mode.Y_Pos;
	Family___Mode.X_Pos+=_out->Width+( (chr==0x20 || (Nippy_Text.Default_Font->default_size>1) )? 0:1);//space between words
	
	Nippy_Send(_out);
	return	_out;	
}
//____________________________________________________________________________
void LcdSend_String(char *chr){
	while(*chr){	
		/*_delay_ms(1000);*/
		LcdSend_Char(*chr++);
	}
	
}
//___________________________________________________________________________

