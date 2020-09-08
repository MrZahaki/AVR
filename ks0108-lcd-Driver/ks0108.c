#include "ks0108.h"//ks0108 with two pin select


//volatile uint8_t test=0;

/* Image data for monochrome displays organized as rows of vertical bytes (Data RAM set)*/
//PROGMEM 
	


	
#define screen_width	128
#define screen_Height	64
 
#define RX_Backup() \
					 Flag_Buffer.DepPort_Backup=DEP_PORT;\
					 Flag_Buffer.RS=((Flag_Buffer.DepPort_Backup>>RS_BIT) & 0x01); \
					 Flag_Buffer.RW=((Flag_Buffer.DepPort_Backup>>RW_BIT) & 0x01);	\
					 Flag_Buffer.BackupRX_Flg=1
					 
#define RX_Restore() \
					  Flag_Buffer.RS ? (RS_HI()) :(RS_LOW()); \
					  Flag_Buffer.RW ? (RW_HI()) :(RW_LOW()); \
					  Flag_Buffer.BackupRX_Flg=0	
					  
//i8080 interface(8bit mode)

#define Read_DisplayData	0
#define Read_busy	1

#define busy_wait()	\
while(Read_ks0108(Read_busy,_NULL,_NULL))

#define Data_DIR DDRB
#define Data_PORT PORTB
#define Data_PIN PINB

#define DEP_DIR DDRA
#define DEP_PORT PORTA
#define RST_BIT 0
	#define Lcd_Reset() \
						DEP_PORT&=~(1<<RST_BIT); _delay_us(5); \
					    DEP_PORT|=(1<<RST_BIT)  //tested
#define E_BIT 1
	#define E_HI()	DEP_PORT |=(1<<E_BIT)
	#define E_LOW()	DEP_PORT &=~(1<<E_BIT)
#define RW_BIT 2
	#define RW_HI()		(DEP_PORT|=(1<<RW_BIT))
	#define RW_LOW()	(DEP_PORT &= ~(1<<RW_BIT))
#define RS_BIT 3
	#define RS_HI()		(DEP_PORT|=(1<<RS_BIT))
	#define RS_LOW()	(DEP_PORT &= ~(1<<RS_BIT))
#define CS1_BIT 4
#define CS2_BIT 5
	#define CS_ON 0
	#define CS_OFF 1
	#define	CS_Set12(L,R) \
							L? (/*CS1 OFF*/DEP_PORT|=(1<<CS1_BIT)):(/*CS1 ON*/DEP_PORT&=~(1<<CS1_BIT)); \
							R? (/*CS2 OFF*/DEP_PORT|=(1<<CS2_BIT)):(/*CS2 ON*/DEP_PORT&=~(1<<CS2_BIT))	//select ON or OFF CS1(LEFT PART) or CS2(RIGHT PART)
	//tested
//_____________________GENARAL State
#define _DoINSTRUCTION_Write 0 //INSTRUCTION WRITE MODE
#define _DoSTATUS_Read 1		//STATUS READ MODE
#define _DoDATA_Write 2		//DATA Write MODE
#define _DoDATA_Read 3			//DATA Read MODE	
#define _DoKeepRX_Tmps 4	//automatically Keep RW and RX Flags during execution in _Do _End function
#define _Do_SendBusy_wait	8	//busy wait operation in _Do function
#define _End_Do_SendKeepXY_POS	16	//automatically keep XY position during write or read operation in _Do _End function
#define _End_DoKeepData	32

#define _Do(x) \
				if( ((x)>>2) & 1 ) {	RX_Backup();	} \
				if(((x)>>3) & 1 ) {	busy_wait();	}	\
				if( ((x)>>4) & 1 )	Flag_Buffer.BackupXY_Flg=1;	\
				if( ((x)>>5) & 1 )	Flag_Buffer.Data_Backup=Data_PORT;	\
				 ((x) & 0x03)==_DoINSTRUCTION_Write ? (RS_LOW(),RW_LOW()): \
				 ((x) & 0x03)==_DoSTATUS_Read ? (Data_DIR=0,Data_PORT=0,RS_LOW(),RW_HI(),\
									 /*dummy*/E_HI(),_delay_us(1),E_LOW(),_delay_us(1),E_HI()): \
				 ((x) & 0x03)==_DoDATA_Write ? (RS_HI(),RW_LOW()): \
				 /*_DoDATA_Read*/(Data_DIR=0,Data_PORT=0,RS_HI(),RW_HI(),\
									/*dummy*/E_HI(),_delay_us(1),E_LOW(),_delay_us(1),E_HI()); \
				 if(1)   //general state
				 
#define _EndDont_EnTrigger	8						 
#define	_End(X)																																				\
				if(Flag_Buffer.BackupXY_Flg && ( ((X)>>4) & 1 ) ){ if( (Flag_Buffer.BackupXY_Flg>>1) )  XYSet_ks0108(Flag_Buffer.X_POS, Flag_Buffer.Y_POS);	\
																										Flag_Buffer.BackupXY_Flg=0;  };						\
				if( (~((X)>>3) & 1)){/*_EndDont_EnTrigger*/																									\
					if(Data_DIR==0) {_delay_us(1); E_LOW(); Data_DIR=0xff; _delay_us(5); }																	\
					else{ E_HI(); _delay_us(0.5); E_LOW(); _delay_us(5); }																					\
						}																																	\
			    if(Flag_Buffer.BackupRX_Flg) {RX_Restore();}																									\
			    if( ((X)>>5) & 1 )	Data_PORT=Flag_Buffer.Data_Backup																					
				
				
#define _Do_Send(X)																																			\
				if(Flag_Buffer.BackupXY_Flg && ( ((X)>>4) & 1 ) ){ if( (Flag_Buffer.BackupXY_Flg>>1) )  XYSet_ks0108(Flag_Buffer.X_POS, Flag_Buffer.Y_POS);	\
				Flag_Buffer.BackupXY_Flg=0;  };																												\
				E_HI(); _delay_us(0.5);																														\
				E_LOW();																																	\
				if(((X)>>3) & 1 ) busy_wait();	else _delay_us(5);																						\
				if( ((X)>>5) & 1 )	Data_PORT=Flag_Buffer.Data_Backup																				
	//tested			
														
//BASE INSTRUCTIONS START and put into data port(Always used in Ge_State() function)____________________________________________________________________________________________________
/********************************************** X ADDRESS *******************************************
																									*
																									*
										NiPpY														Y
																									*
																									*
*****************************************************************************************************/		
#define Set_Display_On(x)		x ?(Data_PORT=(0x3e | 1)) : (Data_PORT=0x3e) //1~display ON 0~display off	
#define Set_XAddress(y)		Data_PORT=(0x40|y) //0~63
#define Set_YAddress(x)		Data_PORT=(0xb8|x) //0~7 page_number		
#define Set_ZAddress(z)		Data_PORT=(0xc0|z) //0~63 (start line)
//BASE INSTRUCTIONS END_____________________________________________________________________________________________________
																																				 //tested																																 
void Init_ks0108(uint8_t	mode,void (*Function_Call)(void)){
	Data_DIR=0xff;
	Data_PORT=0;
	
	DEP_DIR|=(1<<RST_BIT)|(1<<E_BIT)|(1<<RW_BIT)|(1<<RS_BIT)|(1<<CS1_BIT)|(1<<CS2_BIT);
	
	Lcd_Reset();
	CS_Set12(CS_ON,CS_ON);//all cs pins set to 0(active mode)

	_Do(_DoINSTRUCTION_Write ){	
	Set_Display_On(1);
	_Do_Send(_NULL);
	Set_ZAddress(0);
	}_End(_NULL);//GE_State
	
	ScreenFill_ks0108(mode);
	Flag_Buffer.BackgroundDark=	 mode&1;
	Screen.height=64;
	Screen.width=128;
	Screen.color_mode= Screen_ColorMonochrom;
	Screen._size=1024;
	Screen.segment_size=1;//one byte
	Screen.background_color=mode&1;
	Function_Call();	//calling nippy initial function
	}//void
//_________________________________________________________________________________________________________________________________________DATABUFFER
/*
void	Buffer_Manager_ks0108(){//Use RLE format in save buffer
	static	uint8_t		*GRAPHICBuffer;
	GRAPHICBuffer= malloc(1*8);
	
}*/
//set X-Y Address__________________________________________________________________________________________________________________________
#define		XYSet_DontMatterPos	254	//don't change 'XYSet_DontMatterPos'	variable

void XYSet_ks0108(uint8_t X, uint8_t Y){//X 0~127    Y 0~7
	
if( (Flag_Buffer.BackupXY_Flg & 1) ){	
	Flag_Buffer.X_POS=X;
	Flag_Buffer.Y_POS=Y;
	Flag_Buffer.BackupXY_Flg=2;//just backup in First Encounter
}

	
_Do(_DoINSTRUCTION_Write | _DoKeepRX_Tmps | _End_DoKeepData){
if(X<128){
		CS_Set12((X>63),(X<64));
		X<64 ?  (Set_XAddress(X)):( Set_XAddress((X-64)));
		_Do_Send(_NULL);
}
if(Y<8)	Set_YAddress(Y);
}_End(_End_DoKeepData);
}//void	

//Check busy/ON-OFF/RESET flag_____________________________________________________________________________________________________________

uint8_t	Read_ks0108(uint8_t mode,uint8_t Xaddress,uint8_t Yaddress){		//Xaddress from 0-127 | Yaddress form 0-7
uint8_t result=0;	
	switch(mode){
		
		case Read_DisplayData:
				XYSet_ks0108(Xaddress,Yaddress);
				_Do(_DoDATA_Read | _DoKeepRX_Tmps | _End_DoKeepData){
					result=Data_PIN;
				}_End(_End_DoKeepData);
		break;
		case Read_busy:
				_Do(_DoSTATUS_Read | _DoKeepRX_Tmps | _End_DoKeepData){
				result=((Data_PIN >> 6) & 0x01);	
				}_End(_End_DoKeepData);	
		break;
	}
return result;
}
//draw pixel in ks0108_______________________________________________________________________________________________________________________
#define PixelSet_Dark	1
#define PixelSet_Light	0
void PixelSet_ks0108(uint8_t X,uint8_t Y,uint8_t Color){// x~0-127  y~0-63
uint8_t _buf;
		
		_Do(_DoDATA_Write | _Do_SendBusy_wait |_End_Do_SendKeepXY_POS){
		_buf = Read_ks0108(Read_DisplayData,X,(Y/8)) ;	
			(Color&1) ? (Data_PORT=( (1<<Y%8) | _buf  )) : (Data_PORT=( (~(1<<Y%8)) & _buf) );
	
		}_End(_End_Do_SendKeepXY_POS);
}
//fill the screen______________________________________________________________________________________________________________________
#define	Columns_Size 128//1024 Bytes size of display data RAM
#define Rows_Size	8
/*PROBLEM*/
volatile uint16_t	Pos,Rows;

void ScreenFill_ks0108(uint8_t	Color){
	_Do(_DoDATA_Write  |_DoKeepRX_Tmps ){		
				Data_PORT=Color;
				for(Rows=0;Rows<Rows_Size;Rows++){
									for (Pos=0;Pos<Columns_Size;Pos++){
															if(Pos==0 || Pos==64)	{ XYSet_ks0108(Pos,Rows);	}
															_Do_Send(_Do_SendBusy_wait);
									}
				}
	}_End( _EndDont_EnTrigger  );
}
volatile uint8_t	tst=36;
//used pgmspace
//bitmap_set______________________________________________________________________________________________________________________________________________________-

void 	BitmapSet_ks0108(const uint8_t	*BMP	\
						, char	xPos, char	yPos	\
						, uint8_t	width, uint8_t	 height	\
						,uint8_t (*BitmapFuncAddrss)(unsigned int )\
						, uint8_t		_mode){

/*
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

uint8_t	_buffer[(2*width)];
uint16_t	_Counter=0,buff_Cntr=0;
uint8_t dx,dy ;/*object Corner*/		
//PRE REQUESTICS
struct BS_Strct{
uint8_t	Width;	
uint8_t	Height;

unsigned ShiftValue:5;
unsigned ApplyMode:1;
unsigned LocateMode0:1;//Locate On Sram or flash.
unsigned LocateMode1:1;//relate to	BitmapMode_FunctionAddress

}BsFlags;//bitmapset Flags

	BsFlags.ShiftValue=yPos%8;
	BsFlags.Height=(height%8==0)?((int)height/8 ):((int)height/8 +1);
	height=BsFlags.Height*width;//size of out
	yPos/=8;
	BsFlags.Height+=yPos;	
	
	BsFlags.Width= width+xPos;
	BsFlags.ApplyMode=(_mode>>1)&1;//bitmap merge and set
	//in Screen_BackgroundColorDark mode set and merge methods Performance reverses to normal
	if(Screen.background_color==Screen_BackgroundColorDark)	BsFlags.ApplyMode=	!BsFlags.ApplyMode; 
	BsFlags.LocateMode0=(_mode>>2)&1;	//bitmap read FA and sram
	BsFlags.LocateMode1=(_mode>>4)&1;
	
	for(buff_Cntr=0; buff_Cntr < (2*width); buff_Cntr++) _buffer[buff_Cntr]=0;//free buffer
	buff_Cntr=0;	
//__________________________DO BLOCK
					_Do(_DoDATA_Write   ){
						
//------_______________-----------__________________------____________						
						if(BsFlags.ShiftValue){//y_pos%8!=0	

							for(dy=yPos;dy<BsFlags.Height+1;dy++){
								for(dx=xPos; dx< BsFlags.Width /*&& (BsFlags.Width)<=128*/ ;dx++){
									/*if(dx>127)	{	//object corners check
										_Counter+=dx1-128;
										break;
									}*/
									//_delay_ms(50);
//SET MODE*******************************************************************************************************									
									if(BsFlags.ApplyMode){//check bitmap set mode
										if(dx==xPos || dx==64 ) XYSet_ks0108(dx,dy);//set current cursor position																	
													_buffer[buff_Cntr]=( BsFlags.LocateMode0?/*READ SRAM*/ BMP[_Counter] :	\
																				 ( (!BsFlags.LocateMode1)?/*READ From FA*/BitmapFuncAddrss(_Counter):	\
																										/*READ From FLASH*/pgm_read_byte(&BMP[_Counter]))	\
																		);					
												 if(dy==yPos){//row **10**
																_mode=Read_ks0108(Read_DisplayData,dx,dy);	
																XYSet_ks0108(dx,XYSet_DontMatterPos);	
																_mode=( (1<<BsFlags.ShiftValue)-1 ) & _mode; 

												 }
												 else	_mode=0;
												 
												// _delay_ms(1000);
												 
												if(dx>126 || dy>7)	Data_PORT=0;//Flush data port after overflow on corner										 
												else if(_Counter<height/*6 size of data*/){	
													Data_PORT=(_buffer[buff_Cntr]<<BsFlags.ShiftValue) | ( (buff_Cntr>(width-1)) ?	/*check buffer filled*/	\
															  (	_buffer[buff_Cntr-width]>>(8-BsFlags.ShiftValue)) :		 \
															  (_buffer[buff_Cntr+width]>>(8-BsFlags.ShiftValue))  ) |_mode;
													_Do_Send(_Do_SendBusy_wait);										  
												}//else if
												else{
														_mode=Read_ks0108(Read_DisplayData,dx,dy);/*10*/
														XYSet_ks0108(dx,XYSet_DontMatterPos);
														tst=_mode=( ( (1<<(8-BsFlags.ShiftValue))-1 )<<BsFlags.ShiftValue) & _mode ;
														
														Data_PORT=   _mode | (		(buff_Cntr>(width-1)) ?			\
																					(_buffer[buff_Cntr-width]>>(8-BsFlags.ShiftValue)) :	 \
																					(_buffer[buff_Cntr+width]>>(8-BsFlags.ShiftValue))		 \
																			 );  
																		
																							
														_Do_Send(_Do_SendBusy_wait);					
												}//else
													/*Data port= ( Data on current row)<<(ypos%8) | ( current Data number on pre row)>>(8-ypos%8) */
													buff_Cntr++;_Counter++;
											
									}//if BsFlags.ApplyMode
//MERGE MODE*******************************************************************************************************											
									else{//bitmap merge
										_mode=Read_ks0108(Read_DisplayData,dx,dy);
										XYSet_ks0108(dx,XYSet_DontMatterPos);

												_buffer[buff_Cntr]=BsFlags.LocateMode0?/*READ SRAM*/ BMP[_Counter] :	\
																			(!BsFlags.LocateMode1)?/*READ From FA*/BitmapFuncAddrss(_Counter):	\
																					  /*READ From FLASH*/pgm_read_byte(&BMP[_Counter]);
												if(dx>126 || dy>7)	Data_PORT=0;//Flush data port after overflow on corner 										  
												else if(_Counter<height/*6*/){	
														Data_PORT=(_buffer[buff_Cntr]<<BsFlags.ShiftValue) | _mode | ( (buff_Cntr>(width-1)) ?		\
																  (	_buffer[buff_Cntr-width]>>(8-BsFlags.ShiftValue)) :		 \
																  (_buffer[buff_Cntr+width]>>(8-BsFlags.ShiftValue))  ) ;
														_Do_Send(_Do_SendBusy_wait);			
												}//else if
												else{	
													Data_PORT= _mode|( (buff_Cntr>(width-1)) ?				\
												   					 (	_buffer[buff_Cntr-width]>>(8-BsFlags.ShiftValue)) :	 \
																	 (_buffer[buff_Cntr+width]>>(8-BsFlags.ShiftValue))  ) ;
													_Do_Send(_Do_SendBusy_wait);				 
												}
											
												/*Data port= ( Data on current row)<<(ypos%8) | ( current Data number on pre row)>>(8-ypos%8) */
												buff_Cntr++;_Counter++;  
									};//else
									
									//_delay_ms(100);
								}//inner for
								//if(!BsFlags.ScreenMode)  _Counter+=xPos+(128-BsFlags.Width);//*****
								if(buff_Cntr==2*width)	buff_Cntr=0;
								
							}//outer for
						}//if	y_pos%8
//------_______________-----------__________________------____________								
						else{//ypos%8==0
							
							for(dy=yPos;dy<BsFlags.Height ;dy++){
								for(dx=xPos; dx<BsFlags.Width /*&& (BsFlags.Width)<=128*/ ;dx++){
									/*if(dx>127)	{	//object corners check
										_Counter+=dx1-128;
										break;
									}*/
									if(BsFlags.ApplyMode){//bitmap set 
												
												if(dx==xPos || dx==64)/*set cursor position*/XYSet_ks0108(dx,dy);
												
												/*if(dy==yPos){//row **10**
													_mode=Read_ks0108(Read_DisplayData,dx,dy);
													XYSet_ks0108(dx,XYSet_DontMatterPos);
													_mode=(int)pow(2,BsFlags.ShiftValue);

												}
												else	_mode=0;*/
												
												if(dx>127 || dy>7)	Data_PORT=0;//Flush data port after overflow on corner 
												else{
													Data_PORT=/*_mode|*/( BsFlags.LocateMode0?/*ReadSram*/												\
																(BMP[_Counter]):													\
																(!BsFlags.LocateMode1)?/*READ From FA*/BitmapFuncAddrss(_Counter):		\
																				/*READ From FLASH*/pgm_read_byte(&BMP[_Counter])	\
															);  
												_Do_Send(_Do_SendBusy_wait);			
												}//else
												_Counter++;
										}//if	
												
									else{//bitmap merge
								/*bitmap merge*/_mode=Read_ks0108(Read_DisplayData,dx,dy);	
												XYSet_ks0108(dx,XYSet_DontMatterPos);
												if(dx>127 || dy>7)	Data_PORT=0;//Flush data port after overflow on corner 
												else{
													Data_PORT=( BsFlags.LocateMode0?/*ReadSram*/												\
																( BMP[_Counter] | _mode ) :													\
																( (!BsFlags.LocateMode1)?/*READ From FA*/BitmapFuncAddrss(_Counter):	\
																			/*READ From FLASH*/pgm_read_byte(&BMP[_Counter]) | _mode )		\
															);//?
													_Do_Send(_Do_SendBusy_wait);
												}
												_Counter++;
										};//else
									
									//_delay_ms(100);
								}//inner for
							}//outer for
						}//else
					}_End( _EndDont_EnTrigger  );		
}
//____________________________________________________________________________________________________
/*
modes of operation:

#define	BitmapReadSram	4
#define	BitmapMode_FunctionAddress	0
#define BitmapMode_ConstFlash	16
***** Nippy_Mop(&mycar);
*/
void 	BitmapClear_ks0108(const uint8_t	*BMP	
						, char	xPos, char	yPos	
						, uint16_t	width, uint16_t	 height	
						,uint8_t (*BitmapFuncAddrss)(/*unsigned char suggested*/unsigned int)	
						, uint8_t		_mode){
							
uint8_t	_buffer[(2*width)],Data_Buffer;
uint16_t	_Counter=0,buff_Cntr=0;
uint8_t dx,dy/*object Corner*/ ;



//PRE REQUESTICS
struct BS_Strct{
	uint16_t	Width;
	uint16_t	Height;

	unsigned ShiftValue:5;
	unsigned LocateMode0:1;//Locate On Sram or flash.
	unsigned LocateMode1:1;//relate to	BitmapMode_FunctionAddress

}BsFlags;//bitmapset Flags

BsFlags.ShiftValue=yPos%8;
BsFlags.ShiftValue=yPos%8;
BsFlags.Height=(height%8==0)?((int)height/8 ):((int)height/8 +1);
height=BsFlags.Height*width;//size of out
yPos/=8;
BsFlags.Height+=yPos;

BsFlags.Width= width+xPos;
BsFlags.LocateMode0=(_mode>>2)&1;	//bitmap read FA and sram
BsFlags.LocateMode1=(_mode>>4)&1;

for(buff_Cntr=0; buff_Cntr < (2*width); buff_Cntr++) _buffer[buff_Cntr]=0;//free buffer
buff_Cntr=0;

					_Do(_DoDATA_Write   ){
						
//------_______________-----------__________________------____________						
						if(BsFlags.ShiftValue){//y_pos%8!=0	
							
							for(dy=yPos;dy<BsFlags.Height+1;dy++){
								for(dx=xPos; dx< BsFlags.Width /*&& (BsFlags.Width)<=128*/ ;dx++){
									
									
										_mode=Read_ks0108(Read_DisplayData,dx,dy);
										XYSet_ks0108(dx,XYSet_DontMatterPos);
										if(dy==yPos)	Data_Buffer=( (1<<BsFlags.ShiftValue)-1 ) & _mode;
										else	Data_Buffer=0;
										
										
												_buffer[buff_Cntr]=BsFlags.LocateMode0?/*READ SRAM*/ BMP[_Counter] :	\
																			(!BsFlags.LocateMode1)?/*READ From FA*/BitmapFuncAddrss(_Counter):	\
																					  /*READ From FLASH*/pgm_read_byte(&BMP[_Counter]);
												if(dx>126 || dy>7)	Data_PORT=0;//Flush data port after overflow on corner 										  
												else if(_Counter<height/*6*/){	
														Data_PORT=Data_Buffer|(~(_buffer[buff_Cntr]<<BsFlags.ShiftValue) & _mode & ~( (buff_Cntr>(width-1)) ?		\
																  (	_buffer[buff_Cntr-width]>>(8-BsFlags.ShiftValue)) :		 \
																  (_buffer[buff_Cntr+width]>>(8-BsFlags.ShiftValue))  )/*Outer or*/) ;
														_Do_Send(_Do_SendBusy_wait);			
												}//else if
												else{	
													Data_Buffer=( ( (1<<(8-BsFlags.ShiftValue))-1 )<<BsFlags.ShiftValue) & _mode;
													Data_PORT= Data_Buffer|(_mode& ~( (buff_Cntr>(width-1)) ?				\
												   					 (	_buffer[buff_Cntr-width]>>(8-BsFlags.ShiftValue)) :	 \
																	 (_buffer[buff_Cntr+width]>>(8-BsFlags.ShiftValue))  )/*Outer or*/) ;
													_Do_Send(_Do_SendBusy_wait);				 
												}
											
												/*Data port= ( Data on current row)<<(ypos%8) | ( current Data number on pre row)>>(8-ypos%8) */
												buff_Cntr++;_Counter++;  
									
									
									//_delay_ms(100);
								}//inner for
								if(buff_Cntr==2*width)	buff_Cntr=0;
								
							}//outer for
						}//if	y_pos%8
//------_______________-----------__________________------____________								
						else{//ypos%8==0
							
// 							Uart_sendstring( IntToStr(BsFlags.Height));
// 							Uart_sendchar(' ');
// 							Uart_sendstring(IntToStr(BsFlags.Width));
// 							Uart_sendchar(' ');
							
							for(dy=yPos;dy<BsFlags.Height ;dy++){
								for(dx=xPos; dx<BsFlags.Width /*&& (BsFlags.Width)<=128*/ ;dx++){
// 									Uart_sendstring(" dy: ");
// 									Uart_sendstring( IntToStr(dy));
// 									Uart_sendstring(" dx: ");
// 									Uart_sendstring(IntToStr(dx));
// 									Uart_sendchar(' ');
								/*bitmap merge*/_mode=Read_ks0108(Read_DisplayData,dx,dy);
												
								//Uart_sendchar(_mode);

												XYSet_ks0108(dx,XYSet_DontMatterPos);
												if(dx>127 || dy>7)	Data_PORT=0;//Flush data port after overflow on corner 
												else{
													Data_PORT=( BsFlags.LocateMode0?/*ReadSram*/												\
																( ~BMP[_Counter] & _mode ) :													\
																(  ~((!BsFlags.LocateMode1)?/*READ From FA*/BitmapFuncAddrss(_Counter):	\
																			/*READ From FLASH*/pgm_read_byte(&BMP[_Counter])) & _mode )		\
															);//?
													Uart_sendchar(Data_PORT);		
													_Do_Send(_Do_SendBusy_wait);
												
												_Counter++;
												}
									
									
								}//inner for
							}//outer for
						}//else
					}_End( _EndDont_EnTrigger  );		
	
}//end of func
//______________________________________________________________________________________________
