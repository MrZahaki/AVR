/*
 * Img_Process.c
 *
 * Created: 6/22/2019 10:44:44 PM
 *  Author: ZahakI
 */ 
#include "Img_Process.h"



//element
#define MaxFactor(Nmbr)   (log(Nmbr)/log(2))

uint32_t VerticalMagnify_Process(int inp,unsigned int ScaleFactor){//OutElement_Size in byte scale
//max =3byte
uint32_t   Buff,Result;
//uint32_t *Ptr;
uint32_t Max_FC,Tmp;//convert to pointer

//_______________________________initial step
if( !(ScaleFactor && inp) ) return 0;

Max_FC=(int)MaxFactor(inp);

Buff=( 1UL<<Max_FC );
Result=Buff;
for(Tmp=0;Tmp<ScaleFactor-1;Tmp++) Result*=Buff; //result=buff^scale factor

//________________________________calculations step
inp-=Buff;
Buff=0;Tmp=1;
while(inp){
    if(inp%2){
            Result+= ( 1UL<<(Buff*ScaleFactor) );
            Tmp=Buff;
    }//if

    inp/=2;
    Buff++;
}//while
Result*=((1UL<<ScaleFactor)-1);
return Result;
}
#define VerticalMagnifyOutSize_OneByte  1
#define VerticalMagnifyOutSize_TwoByte  2
#define VerticalMagnifyOutSize_FourByte  4
/*
 * Output Frame: [NumberOfElements, Data0,Data1, Data2,...]
 * Eg0: VerticalMagnify(145,VerticalMagnifyOutSize_OneByte,5)
 *      Out(uint32_t):  [9,31,0,240,1,248,0,0,0,]
 * scale factor tested between 1,10
 */

//__________________________________________________________________________________
void *VerticalMagnify(int inp,uint8_t OutElement_Size,int ScaleFactor){
uint32_t *_Res;
uint32_t	*Ptr;


uint16_t  MaxF,Counter,Buff,period;
uint32_t  Big_Factor;

//if(!inp)  return 0;
Big_Factor=(int) MaxFactor(inp);//bozorg tarin tavon mazrab 2 kamter as inp
// Uart_sendstring("inp=  ");
// Uart_sendstring( IntToStr(inp) );
// Uart_sendstring("  ");


if( ( (Big_Factor/8+1)*ScaleFactor) > 4 ){//check out size

  MaxF=(int)(32/ScaleFactor);//maximum size of each elements in _Res list with 32 bit frame
  Buff= 32-(MaxF* ScaleFactor);//elements additional bit numbers
  period = (int)(Big_Factor/MaxF)+1;//number of elements
  _Res=calloc(period,4*sizeof (char));

  _Res[0] = VerticalMagnify_Process (inp&( (1UL<<MaxF)-1 ) , ScaleFactor);
  inp=inp>>MaxF;

  for(Counter=1;Counter<period;Counter++){
      _Res[Counter]=VerticalMagnify_Process (inp&( (1<<MaxF)-1 ) , ScaleFactor);
      if(Counter>1 && Buff>0) Buff= Buff*Counter/(Counter-1);//shift dar har marhale 2 vahed ezafe mishavad
      _Res[Counter-1]= _Res[Counter-1] | (  ( _Res[Counter] & ((1UL<<Buff)-1) ) << (32-Buff)   );/*32 bit frame*/
      _Res[Counter]=_Res[Counter]>>(Buff);

      inp=inp>>MaxF;
  }//for
// 	for(Counter=0;Counter<period;Counter++)	{
// 	Uart_sendstring("_res=  ");
// 	Uart_sendstring( IntToStr(_Res[Counter]) );
// 	Uart_sendstring("  "); 
// 	}
}//if2
else
  {
    period=0;
    _Res=calloc(1,4*sizeof (char));
    *_Res = VerticalMagnify_Process (inp , ScaleFactor);
	
  }
//________________________________allocation step
MaxF=(  (period==0?             \
         ( (int)ScaleFactor*Big_Factor/(8*OutElement_Size) +2):       \
         (period*4/OutElement_Size+1)  )/*32 bit output format*/ )    \
          -1;

//MaxF=( (Big_Factor/8+1)*ScaleFactor )/OutElement_Size;		  
//Number of output array members that size of each member is OutElement_Size;
OutElement_Size*=8;//convert byte to bit

Big_Factor = ( OutElement_Size==32 ?(-1/*32 bit frame*/) : ((1UL<<OutElement_Size)-1) );//OutElement_Size maximum value
inp=(int)32/OutElement_Size;
//process will continue until each 32bit buffer flushed. inp in here is number of output element that we can take from one 32bit buffer.
Ptr=calloc( MaxF, OutElement_Size);//allocation


//Uart_sendstring(" part:  ");
for( Counter=0,Buff=0 ; Counter < MaxF; Counter++){
    if( (Counter!=0) && (Counter%inp == 0) )  Buff++;
    
    *(Ptr+Counter+1)=  (_Res[Buff] & Big_Factor  );
							
						
    _Res[Buff]=( OutElement_Size==32 ? 0 :(_Res[Buff]>>OutElement_Size) );
}
(*Ptr)=MaxF+1;
free(_Res);

return Ptr;
}//function
//_____________________________________________________________________________________________________________
/*#define Scale_Mode_SetChild	0//default_mode(improve memory usage)(warning child object is changed  when you set this)
#define Scale_Mode_SetFather	1
#define Scale_Mode_SendToScreen	*/



#define	Scale_Mode_Normal	0
#define	Scale_Mode_XCoordinate	2
#define	Scale_Mode_YCoordinate	4
/* 
magnify value must be protected
*/
Child Magnify( Child inp_obj, uint8_t Magnify_Value,	uint8_t	Scale_Mode){
	Child	out_obj;
	
if(Scale_Mode){
	
	
	uint16_t	Bitmap_Size,counter160,counter161;
	uint8_t		_data,counter80;
	//_____________________________________________________________________initial values
	struct BS_Strct{
		unsigned LocateMode0:1;//Locate On Sram or flash.
		unsigned LocateMode1:1;//relate to	BitmapMode_FunctionAddress
		
		//uint8_t	width;
		//uint8_t	height;
		//uint16_t	bitmap_size;/*
			}magnify;//bitmapset Flags
	//out_obj
	out_obj=Pregnant_Mother(Pregnant_Mother_EnableAutoFree);
	Set_Equal__(out_obj,inp_obj);
// 	Uart_sendchar(inp_obj->Width);11
// 	Uart_sendchar(inp_obj->Height);16
		
	magnify.LocateMode0=(inp_obj->BitmapMode>>2)&1	;	//bitmap read FA and sram
	magnify.LocateMode1=(inp_obj->BitmapMode>>4)&1	;
	
	//if(Scale_Mode & 1){//Scale_Mode_SetFather
		
	//}
	Bitmap_Size=( ((inp_obj->Height%8==0)?((int)inp_obj->Height/8 ):((int)inp_obj->Height/8 +1))* inp_obj->Width );
	//Uart_sendchar(Bitmap_Size);
	if(	(Bitmap_Size*Magnify_Value) /*input bitmap max size*/ > Screen._size	/*Screen max size*/)
			Magnify_Value	=	 Screen._size/Bitmap_Size ;//max size of magnify_value
	//else{//Scale_Mode_SetChild(default)
//_______________________________________________________________________________
		if((Scale_Mode>>1) & 1){//Scale_Mode_XCoordinate
			//Outout size caculations
			
			out_obj->BitmapFile=calloc( (Magnify_Value*Bitmap_Size) , sizeof(char) );
			ChildCTL_Add(out_obj->BitmapFile,ChildCTL_Add_AutoFree);
			out_obj->Width*=Magnify_Value;

//_____________________________________________________magnifyign process
						
			counter161=0;
			for(counter160=0;counter160 < Bitmap_Size;counter160++){
				
				_data=magnify.LocateMode0?/*READ SRAM*/ inp_obj->BitmapFile[counter160] :	\
				(!magnify.LocateMode1)?/*READ From FA*/inp_obj->BitmapFuncAddrss(counter160):	\
				/*READ From FLASH*/pgm_read_byte(&inp_obj->BitmapFile[counter160]);
				//Uart_sendchar(_data);
				for(counter80=0; counter80<Magnify_Value; counter80++)	out_obj->BitmapFile[counter161++]=_data;	
				
			}//outer for
			
		}//if
//_______________________________________________________________________________		
		else if((Scale_Mode>>2) & 1){//Scale_Mode_YCoordinate
			uint32_t	*_scale;
			uint16_t counter162,counter163;
			
			counter163/*max height value*/ =Bitmap_Size/inp_obj->Width;//see bitmap_size syntax
			
			//Uart_sendchar(counter163);
			
			out_obj->Height *=Magnify_Value;
			
			Bitmap_Size/*out size*/=	(( (out_obj->Height%8==0)?((int)out_obj->Height/8 ):((int)out_obj->Height/8 +1) ) * out_obj->Width 	);
			out_obj->BitmapFile=calloc( Bitmap_Size , sizeof(char) );
			ChildCTL_Add(out_obj->BitmapFile,ChildCTL_Add_AutoFree);
			
			
			Scale_Mode= Screen.segment_size * Magnify_Value;//y coordinate jump value
			
				for(	Bitmap_Size=0/*Data counter*/,counter160=0/*ypos*/;counter160<	counter163	;	counter160++){
					for(	counter161=0/*xpos*/;	counter161<	inp_obj->Width;	counter161++){
					
					_data=magnify.LocateMode0?/*READ SRAM*/ inp_obj->BitmapFile[Bitmap_Size++] :	\
						(!magnify.LocateMode1)?/*READ From FA*/inp_obj->BitmapFuncAddrss(Bitmap_Size++):	\
												/*READ From FLASH*/pgm_read_byte(&inp_obj->BitmapFile[Bitmap_Size++]);
					
					//_delay_ms(1000);
					counter162= counter160/*ypos*/	*	Scale_Mode	*	out_obj->Width + counter161/*xpos*/;
					if(_data){ 
							_scale=VerticalMagnify(	_data,	Screen.segment_size,	Magnify_Value);
							
// 							Uart_sendstring("data=  ");
// 							Uart_sendstring( IntToStr(_data) );
// 							Uart_sendstring("  ");
							
							 for(counter80=0;(counter80<*_scale-1) && (counter80<Scale_Mode);counter80++){
								out_obj->BitmapFile[counter162+counter80 * out_obj->Width] =	_scale[counter80+1];
								counter162+counter80 * out_obj->Width;
							}// inner for
							
							for(;counter80<Scale_Mode;counter80++) /*check*/
								out_obj->BitmapFile[counter162+counter80 * out_obj->Width] =  0;
								
							free(_scale);
					}//if
					else
						for(counter80=0;counter80<Scale_Mode;counter80++){
							out_obj->BitmapFile[counter162+counter80 * out_obj->Width] =	0;
							counter162+counter80 * out_obj->Width;
							
					}// inner for
					
				}//inner for
				
			}//outer for 
			
		}//else if
}
else{//Scale_Mode_Normal
		Child	__buffer;		
		__buffer=Magnify(inp_obj,Magnify_Value,Scale_Mode_XCoordinate);
		out_obj=Magnify(__buffer,Magnify_Value,Scale_Mode_YCoordinate);
		
		Child_Control(__buffer,	ChildControl_KILLALL);	
		//Child_Control(__buffer->BitmapFile,	ChildControl_KILL);	
		
		}//else
		
	
	return	out_obj;
}//function
//_______________________________________________________________________________________________________
