/*
 * Img_Process.h
 *
 * Created: 6/22/2019 10:44:54 PM
 *  Author: ZahakI
 */ 
#ifndef IMG_PROCESS_H_
#define IMG_PROCESS_H_

#include "Nippy.h"


/*
#define	BitmapReadSram	4//OBJ_mode
#define	BitmapMode_FunctionAddress	0//OBJ_mode
#define BitmapMode_ConstFlash	16//OBJ_mode
*/

/*#define Scale_Mode_SendToScreen	0//default_mode
#define Scale_Mode_SetOnFather	1*/

#define	Scale_Mode_Normal	0
#define	Scale_Mode_XCoordinate	2
#define	Scale_Mode_YCoordinate	4


Nippy_Obj *Magnify( Child _OBj, uint8_t Magnify_Value,	uint8_t	Scale_Mode);




#endif /* IMG_PROCESS_H_ */