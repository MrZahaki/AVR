
#ifndef MAIN_H_
#define MAIN_H_

#ifdef	F_CPU
#undef F_CPU
#define F_CPU 4000000UL//mega1284
#else
#define F_CPU 4000000UL//mega1284
#endif

#include <math.h>
#include <avr/interrupt.h>
#include "Nippy.h"

#include "Img_Process.h"







#endif /* MAIN_H_ */