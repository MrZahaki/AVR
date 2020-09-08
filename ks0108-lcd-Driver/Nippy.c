#include "Nippy.h"

//____________________________________________________________
#ifdef Init_BufferMode
	
	uint8_t nippy_buffer[Screen._size];
	Child	Father;
				
				0
#endif

//_______________________________________________________________________________
char Nippy_Init(uint8_t	MODE,void(*INIT_FUNCTION)(void)){
	
	Font	fnt;  
	  
	fnt=Pregnant_Mother(Pregnant_Mother_Font);
	fnt->BitmapMode =Font_Mode_ConstFlash|Font_Mode_MergeOnScreen;
	fnt->BitmapFile=bradley_hand_itc_italic;
	fnt->default_size=1;
	fnt->Font_Name=SET_FONT_NAME("bradley");
	//Uart_sendstring(fnt->Font_Name);

		
	//__________________________________________
	fnt=Pregnant_Mother(Pregnant_Mother_Font);
	fnt->BitmapFile=calisto_mt;
	fnt->BitmapMode=Font_Mode_ConstFlash|Font_Mode_MergeOnScreen;
	fnt->default_size=1;
	fnt->Font_Name=SET_FONT_NAME("calisto");
	//Uart_sendstring(fnt->Font_Name);
	//FontBox_Access(fnt);
	//__________________________________________________________________
	
	fnt=Pregnant_Mother(Pregnant_Mother_Font);
	fnt->BitmapMode =Font_Mode_ConstFlash|Font_Mode_MergeOnScreen;
	fnt->BitmapFile = arial;
	fnt->default_size=1;
	fnt->Font_Name=SET_FONT_NAME("arial");
	
	
	//__________________________________________________________________TEST STEP
	/*
 	Uart_sendstring(IntToStr(FontBox_Check(fnt)));
	 Uart_sendstring(IntToStr(FontBox_Check(fnt1)));
	 Uart_sendstring(IntToStr(FontBox_Check(fnt2)));
	 Uart_sendstring(IntToStr(FontBox_Check(fnt3)));*/
	
	//Name_Box("bradley");
	
	/*Uart_sendstring("  bradley: ");
	if(fnt==Nippy_Text.Default_Font)    Uart_sendstring("  true ");
	else    Uart_sendstring("  false ");

	Uart_sendstring("  arial: ");
	if(fnt1==Nippy_Text.Default_Font)    Uart_sendstring("true ");
	else    Uart_sendstring("  false ");

	Uart_sendstring("  calisto: ");
	if(fnt2==Nippy_Text.Default_Font)    Uart_sendstring("  true ");
	else    Uart_sendstring("  false ");*/
	
	
	//__________________________________________________________________set default x y pos
	Family___Mode.X_Pos=0;
	Family___Mode.Y_Pos=0;
	//__________________________________________________________________initial step
		cycle=0;
		Protect_period=(	(MODE>>1)&1 ? 350:	(	(MODE>>2)&1 ? 1000:100	)	);

	
	
	#ifdef Init_BufferMode
		Father->BitmapFile=nippy_buffer,
		Father->BitmapMode= BitmapMode_ConstSram|BitmapMode_SetOnScreen,
		Father->X_Pos= 0,Father->Y_Pos= 0,
		Father->Width= 128,Father->Height= 64,
	#endif
	
	#ifdef		KS0108_CS1_CS2
		Init_ks0108(( (MODE&1)?255/*dark*/:0 ),INIT_FUNCTION);	
	#elif
	
	#endif	
	
	
return	0;	
}
//_________________________________________________________________________________________________________
void Nippy_Main(void(*MAIN_FUNCTION)(void)){//auto  modes works  in here 
	MAIN_FUNC=MAIN_FUNCTION;
	while(1){
		MAIN_FUNCTION();
		if(		Family___Mode.Auto_Free_Mode/*Family_Mode_Enable*/	&&		!(cycle%Protect_period)		)
				ChildCTL_AutoFree(cycle);
		cycle++;
	}//while 1
	
}//nippy main

//_________________________________________________________________________________________________________
void Nippy_Send(Child	_NEW_){
	#ifdef		KS0108_CS1_CS2
	if((_NEW_->BitmapMode>>3)&1/*Free page mode used buffer mode*/){

	}
	else if((_NEW_->BitmapMode>>5)&1/*Pixel Mode*/){
		PixelSet_ks0108(_NEW_->X_Pos,_NEW_->Y_Pos,_NEW_->PixelColor);
	}
	else{
		
		BitmapSet_ks0108(_NEW_->BitmapFile,_NEW_->X_Pos,_NEW_->Y_Pos,_NEW_->Width,_NEW_->Height,_NEW_->BitmapFuncAddrss,_NEW_->BitmapMode);
		
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

void	Nippy_Mop(void * Mop_mode){
#ifdef		KS0108_CS1_CS2
	if(Mop_mode){
		
		Child TMP;
		TMP=Mop_mode;
		
		/*Uart_sendstring( IntToStr(TMP->Width));
		Uart_sendchar(' ');
		Uart_sendstring(IntToStr(TMP->Height));
		Uart_sendchar(' ');*/
		
		BitmapClear_ks0108( (TMP->BitmapFile), (TMP->X_Pos), (TMP->Y_Pos), (TMP->Width), (TMP->Height), (TMP->BitmapFuncAddrss), (TMP->BitmapMode) );
	}	
	else	ScreenFill_ks0108(Flag_Buffer.BackgroundDark?0xff:0);
	
#elif

#endif	
}
//___________________________________________________________________________
void Set_Equal__(Child dest_obj,Child source_obj){//dest_obj will be equal to source_obj
	
	dest_obj->Behave_Func=source_obj->Behave_Func;
	dest_obj->BitmapFile=source_obj->BitmapFile;
	dest_obj->BitmapFuncAddrss=source_obj->BitmapFuncAddrss;
	dest_obj->BitmapMode=source_obj->BitmapMode;
	dest_obj->Height=source_obj->Height;
	dest_obj->PixelColor=source_obj->PixelColor;
	dest_obj->Width=source_obj->Width;
	dest_obj->X_Pos=source_obj->X_Pos;
	dest_obj->Y_Pos=source_obj->Y_Pos;
}
//_______________________________________________________________________________
typedef struct {
	long int _address;//used in indexremover function
	uint16_t access_counter	;
	unsigned int access_cycle		;
	unsigned	Enable_Protect:1	;
}nippy_Var;

nippy_Var **ChildCTL=0	;
uint16_t Child_no=0		;
unsigned int Protect_period=400;
unsigned long int cycle=0;

int ChildCTL_Check(void *ptr){//variable address 1:true  0:false
	unsigned int _cntr=0;
	if( (ptr==0) || (Child_no==0) ) return 0;
	for(;_cntr<Child_no;_cntr++){
		if( ChildCTL[_cntr]  ){
			if( ChildCTL[_cntr]->_address == (long)ptr  ) return 1;
			ChildCTL[_cntr]->access_counter++;
			ChildCTL[_cntr]->access_cycle=cycle;
		}//if
	}
	return 0;
}
//_______________________________________________________________________________
//Add new variable in list
Child ChildCTL_Add(void *ptr,uint8_t	_mode){// variable address
	unsigned int _cntr=0;
	
	for(;_cntr<Child_no;_cntr++)
	if(*(ChildCTL+_cntr) == 0){
		*(ChildCTL + _cntr)=malloc(sizeof(nippy_Var));
		ChildCTL[ _cntr]->_address=(long)ptr;
		ChildCTL[ _cntr]->access_counter=6;
		ChildCTL[_cntr]->access_cycle=cycle;
		ChildCTL[_cntr]->Enable_Protect=_mode&1;
		return ptr;
	}

	ChildCTL =realloc(ChildCTL ,(Child_no+1)*sizeof (ChildCTL));
	*(ChildCTL + Child_no)=malloc(sizeof(nippy_Var));
	ChildCTL[ Child_no]->_address=(long)ptr;
	ChildCTL[ Child_no++]->access_counter=10;
	ChildCTL[_cntr]->access_cycle=cycle;
	ChildCTL[_cntr]->Enable_Protect=_mode&1;
	//printf("in function=%x\n",ptr);
	return (Child) ptr;
}
//_______________________________________________________________________________
//free variable from list

void ChildCTL_Free(void *ptr){//force child to free from auto free list
	unsigned int _cntr=0;



	for(;_cntr<Child_no;_cntr++)
	if( ChildCTL[_cntr] && ChildCTL[_cntr]->_address == (long)ptr ){
		free(ChildCTL[_cntr]);
		ChildCTL[_cntr]=0;
	}//if
}//free func

void ChildCTL_AutoFree(uint32_t current_cycle ){
	static  uint32_t _cntr=0;
	if(  !Child_no ) return ;

	if(_cntr>Child_no)	_cntr=0;

	if( ChildCTL[_cntr] && \
	ChildCTL[_cntr]->Enable_Protect && \
	(abs(current_cycle - ChildCTL[_cntr]->access_cycle )> Protect_period) ){


		(ChildCTL[_cntr]->access_counter)--;
		// printf("Enable protect: %d cntr=%d access counter=%d\n", ChildCTL[_cntr]->Enable_Protect,_cntr,ChildCTL[_cntr]->access_counter );
		if( (ChildCTL[_cntr]->access_counter) == 0){
			free(ChildCTL[_cntr]);
			ChildCTL[_cntr]=0;

		}//if
	}//if
	_cntr++;
}//function
/*----------------------------------------*/


Nippy_Obj * ChildProtect(void *ptr){
	if( !ChildCTL_Check(ptr)/*known child*/ )  ChildCTL_Add(ptr,ChildCTL_Add_AutoFree) ;
	return  ptr;

}
//______________________________________________________________________
/*
#define ChildAutoFree_Enable	0//default
#define ChildAutoFree_Disable	1//default

#define ChildControl_AutoFree	 need for access to child auto free mode 
#define ChildAutoFree_SETAcsCntr(x)	(8<<x)	//0-255

#define ChildControl_KILL	4//lock off door
#define KILL_ChildBitmap	1

*/
void Child_Control(void* _child,uint16_t	_mode){//child control auto free mode
	unsigned int _cntr=0;
	if( (_child==0) || (Child_no==0) ) return ;
	
	if((_mode>>3)&1/*KILLALL mode 4*/){
		Child	__buffer;
		__buffer=_child;
		Child_Control(__buffer->BitmapFile,	ChildControl_KILL);	
		Child_Control(__buffer,	ChildControl_KILL);	
	}
	
	if((_mode>>2)&1/*KILL mode 4*/){
		for(;_cntr<Child_no;_cntr++)
			if( ChildCTL[_cntr] && ChildCTL[_cntr]->_address == (long)_child ){
				//Uart_sendstring("Ok ");
				//if(_mode & 1/*KILL_ChildBitmap*/)	free(__buff->BitmapFile)	;
				free(ChildCTL[_cntr]);
				ChildCTL[_cntr]=0;
				return	;
			}//for
		free(_child);	
		}//if
	else if((_mode>>1)&1/*Child_AutoFree mode 2*/)/*first check higher conditions */
		for(;_cntr<Child_no;_cntr++)
			if( ChildCTL[_cntr] && ( ChildCTL[_cntr]->_address == (long)_child  ) )	{		
						ChildCTL[_cntr]->Enable_Protect= !(_mode&1);
						ChildCTL[_cntr]->access_counter+= (_mode>>8);
			}//if
			
		
}//function
//____________________________________________________________________________
	//#define FamilyMode_DisableAutoFree	0
	//#define FamilyMode_EnableAutoFree	1
void FamilyMode(uint8_t	_mode){
	Family___Mode.Auto_Free_Mode= (_mode&1) ?	FamilyMode_EnableAutoFree:	FamilyMode_DisableAutoFree;
	
}//func
//______________________________________________________________________
/*
	e.g.	
		Nippy_Font	Test_font;
		Test
			
*/
char *SET_FONT_NAME(char	*Name){
	 char *PTR;
	 PTR=malloc(Str_len(Name)+1);
	 StrCpy(PTR,Name);
	 return	PTR;
}
