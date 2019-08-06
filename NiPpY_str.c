/*
											        			DESIGNED BY NiPpY
                                         MrZahaki
                                            V4
*/
#include "NiPpY_str.h"

typedef struct {
long int *Variable_BUF;//used in indexremover function
int Variable_num;
}nippy_Var;
nippy_Var VariableCTL={.Variable_BUF=0,.Variable_num=0};

typedef struct {
unsigned Insulate_Mode:1 ;//used in indexremover function(default auto)
                
}nippy_Mode;
nippy_Mode STR_Mode={.Insulate_Mode=1};
//_______________________________________________________________________________
/*
 * used in index_remover and Str_Extractor, StrCutter, etc
 */
void _MODE_(char M){
  M? (STR_Mode.Insulate_Mode=1):(STR_Mode.Insulate_Mode=0);
}
//_____________________________________________________________________________
/*
 * 'Variable control function' used for variable address identication in special functions.
 * These intrinsic functions are used in intelligent memory management and prevent the
   allocation of additional memory to the addresses used by this library. 
 * this function check variable that exist in list(1) or not(0).
 * true :1
 * false: 0
 */
int VariableCTL_Check(void *ptr){//variable address
  unsigned int _cntr=0;
  for(;_cntr<VariableCTL.Variable_num;_cntr++)
      if(*(VariableCTL.Variable_BUF+_cntr) == (long)ptr) return 1;
  if( (ptr==0) || (VariableCTL.Variable_num==0) ) return 0;
  return 0;       
}
//_______________________________________________________________________________
//Add new variable in list
void VariableCTL_Add(void *ptr){// variable address
  unsigned int _cntr=0;
  
  for(;_cntr<VariableCTL.Variable_num;_cntr++)
          if(*(VariableCTL.Variable_BUF+_cntr) == 0){
              *(VariableCTL.Variable_BUF+_cntr)=(long)ptr;
              return ; 
          }
  
  VariableCTL.Variable_BUF =realloc(VariableCTL.Variable_BUF ,(VariableCTL.Variable_num+1)*sizeof (void *));
  *(VariableCTL.Variable_BUF + VariableCTL.Variable_num++)=(long)ptr;
}
//_______________________________________________________________________________
//free variable from list
#define Free_CTL(addrss)  VariableCTL_Free(addrss); free(addrss)
void VariableCTL_Free(void *ptr){
  unsigned int _cntr=0;
  for(;_cntr<VariableCTL.Variable_num;_cntr++)
          if(*(VariableCTL.Variable_BUF+_cntr) == (long)ptr)  *(VariableCTL.Variable_BUF+_cntr)=0; 
}
//_______________________________________________________________________________
unsigned int Str_len(char *str_){//Str_len of one *str
	unsigned int counter=0;
	while(str_[counter++]);
	return counter-1;
}
//_____________________________________________________________________________________________________________________________________
int *ignore_zero_(int *input_){//ignore zero and size reduction for integer array/ capacity= Str_len_ num in array[first eleman]
#define buf_size  Search_Buffer_Size
static int result[buf_size+2];
int i_=buf_size,count_=buf_size,result_Str_len_=0;
	for(;i_>-1;) result[i_--]=0;//cleaning buffer
	for(i_=buf_size-1; i_ > -1; ) if(input_[i_--]>0){
		 result[count_--]=input_[i_+1];//caculates num of nonzero numbers
		 result_Str_len_++;
	}
result[count_]=result_Str_len_+1;
input_=result;
while(*input_ == 0)	input_++;//cleaning zero from first of array
return input_;
}
#define StrSearch_All_Occurrences   0
#define StrSearch_First_Occurrence   1
#define StrSearch_Next_Occurrence   2

//_____________________________________________________________________________________________________________________________________
void* StrSearch(char* _str,char* _search ,char _mode ){
	int i=0,*result=NULL;
	unsigned int search_cnt=0;
  static unsigned int str_cnt=0;


if(_mode == StrSearch_All_Occurrences){
    str_cnt=0;
    int _buffer_ [Search_Buffer_Size];
    for(;i<Search_Buffer_Size;) _buffer_[i++]=0;//cleaning buffer
	i=0;

	while(_str[str_cnt] && i< Search_Buffer_Size) if(_search[search_cnt]==_str[str_cnt++] ) _buffer_[i++]=str_cnt;//founding first char that be samed in both str's
	if( i>0 && Str_len(_search)>1 ){//if find any same char then ...
		for( search_cnt = 1; search_cnt < Str_len(_search); search_cnt++ ){
		    for( str_cnt=0; str_cnt<i; str_cnt++){// select and process other characters form _search.str*
					if( ( _search[search_cnt]!=_str[ _buffer_[str_cnt]+(search_cnt-1)] ) && ( _buffer_[str_cnt]!=0 ) )	_buffer_[str_cnt]=0	;
			}
		}
	}
	result=ignore_zero_(_buffer_);
	if((*result > 1))
            return (int *)result;
    else {
        return (int *)(result+1);
    }
}//first  if
else {
    unsigned int _Str_len ;
    if(_mode ==StrSearch_First_Occurrence) str_cnt=0;
    _Str_len=Str_len(_search);
    while(_str[str_cnt]) if(_search[search_cnt]==_str[str_cnt++] ){
                               while ( ++search_cnt < _Str_len )
                                        if(_search[search_cnt]!=_str[str_cnt++])  break;
                               if(search_cnt == _Str_len)  return  (unsigned int)(str_cnt-_Str_len+1);
                                search_cnt=0;

                        }//if
                        return (unsigned int)0;
}


}//func
//_____________________________________________________________________________________________________________________________________

#define StrReplace_All_Occurrences   0
#define StrReplace_First_Occurrence   1
#define StrReplace_Next_Occurrence    2

void *StrReplace(char *dest, char *Search_str,char *Replace_str, char _Mode){
unsigned int *InP,_index[]={3,0,1};//integer pointer
unsigned int _i,_j,_buf,_offset=0,ReplaceStr_len,Search_len;

ReplaceStr_len= Str_len(Replace_str);
Search_len=Str_len(Search_str);
dest=Str_IndexAdder(dest,' ', _index);        
if(_Mode == StrReplace_All_Occurrences){
                                InP=(unsigned int *)StrSearch(dest,Search_str,StrSearch_All_Occurrences);
                                for(_i=1; _i < *InP; _i++){
                                    _buf=InP[_i]+_offset-1;                                  
                                    if(ReplaceStr_len > Search_len) {
                                        _index[1]=_buf;//address
                                        _index[2]=ReplaceStr_len-Search_len;//index numbers
                                        _offset+= _index[2];
                                        dest=Str_IndexAdder (dest,' ',_index );                                                          
                                    }
                                    else if(ReplaceStr_len < Search_len){
                                      dest=Str_Extractor (dest, _buf, _buf+Search_len-ReplaceStr_len-1 );
                                      _offset-=Search_len-ReplaceStr_len;
                                    }
                                                                                                                        
                                    for(_j=0;_j<ReplaceStr_len;_j++)  *(dest+_buf+_j)=*(Replace_str+_j);                                                                         
                                }//for                                
}//if
else{
                                    _i=(unsigned int )StrSearch(dest,Search_str,_Mode);
                                    _buf=_i+_offset-1;
                                  
                                    if(ReplaceStr_len > Search_len) {
                                        _index[1]=_buf;//address
                                        _index[2]=ReplaceStr_len-Search_len;//index numbers
                                        _offset+= _index[2];
                                        dest=Str_IndexAdder (dest,' ',_index );                                                          
                                    }
                                    else if(ReplaceStr_len < Search_len){
                                      dest=Str_Extractor (dest, _buf, _buf+Search_len-ReplaceStr_len-1 );
                                      _offset-=Search_len-ReplaceStr_len;
                                    }
                                                                                                                        
                                    for(_j=0;_j<ReplaceStr_len;_j++)  *(dest+_buf+_j)=*(Replace_str+_j);     
  }
return dest;
}
//_____________________________________________________________________________________________________________________________________
#define Str_IndexCreator_END    0
#define Str_IndexCreator_ADD    1
#define Str_IndexCreator_Reset  2//for free index buffer

void *Str_IndexCreator(char *_inp,char Fill,void *_index, unsigned char _Mode){
static char *OuT;
static unsigned int  *index_Buffer, Out_size, Buffer_size=0;
unsigned int _i, _j;

/**/if( (_Mode>>1)&1 ) Buffer_size=0; //Str_IndexCreator_Reset
else if(Buffer_size==0) Out_size=Str_len(_inp);


/**/if(!(_Mode&1) && (Buffer_size>1) ){//Str_IndexCreator_END

        if(Fill == 0) return -1;
        OuT=realloc(OuT,Out_size);
        //printf("Out_size= %d, buffer size= %d\n",Out_size,Buffer_size);

        for(_i=0;_i<Out_size;) *(OuT+_i++)=0;

        for(_i=0;_i<Buffer_size;_i+=2){//base
                for(_j=0;_j<Buffer_size;_j+=2){//in progress
                        if(_i==_j) continue;
                        if( *(index_Buffer+_j) > *(index_Buffer+_i) )   *(index_Buffer+_j) += *(index_Buffer+_i+1);//see nippy Str_IndexCreator algorithm

                }//inner for
        }//for

        for(_i=0;_i<Buffer_size;_i+=2){
            for(_j=0; _j < *(index_Buffer+_i+1);_j++ )
                *(OuT + *(index_Buffer+_i) + _j/*see algorithm*/)=Fill;
        }

        _i=0;
        while(*_inp){
                while( _i < Out_size ){
                                if(*(OuT+_i) == Fill){
                                        //printf("Fill");
                                        _i++;
                                        continue;
                                }
                                *(OuT+_i++) = *_inp;
                                break;
                }//while
                _inp++;
        }//while
         *(OuT+ Out_size) = *_inp;
        //for(_j=0;_j<Out_size;_j++) printf("%c ",*(OuT+_j));
       // printf("\nout size=%d\n\n",Out_size);

        return OuT;
}//if
/**/else if((_Mode&1)){//Str_IndexCreator_ADD

        if( (*((unsigned int*)_index+1) == 0) || (*(unsigned int*)_index >Str_len(_inp)) ) return  -1;//check input;

        index_Buffer= realloc(index_Buffer,Buffer_size+2);
        *(index_Buffer+Buffer_size++)=*(unsigned int*)_index;//Save index Address
        *(index_Buffer+Buffer_size)=*((unsigned int*)_index+1);//Save index Numbers
        Out_size+=*(index_Buffer+Buffer_size++);

        //for(_i=0;_i<Buffer_size;_i++)   printf("%d ",*((unsigned int*)_index+_i));
        return (int *)1;
}//else if
return (int *)1;
}
/*
index frame: [size of frame,index_address0,index Numbers0,index_address1,index Numbers1,...]
fp: pointer that point to address that.
*/
//________________________________________________________________________________________________________________________________
void *Str_IndexAdder(char *_inp,char Fill,unsigned int *_index){
char    *OuT,char_size,memFlag=0;
unsigned int _i, _j, Out_size=0;


Out_size=Str_len(_inp);
char_size=sizeof(char); 
_j=Str_len(_inp);
//printf("\n str=%s \n",_inp);
//printf("Out_size=%d \n",Out_size);

for(_i=2;_i<*_index;_i+=2){
       //printf("%d len=%d ",*(_index+_i-1));
        if(*(_index+_i-1) > _j) return (int *)-1;
        Out_size+= *(_index+_i);
}

/**/if(*_index > 1 ){
        if(Fill == 0) return (int *)-1;
        if(STR_Mode.Insulate_Mode && VariableCTL_Check (_inp)){
            OuT=realloc(_inp,(Out_size+1)*char_size);
              _inp=calloc(Out_size,char_size);
              StrCpy(_inp,OuT);
              memFlag=1;
        }
        else{
            OuT=calloc(Out_size+1,char_size);
             if(STR_Mode.Insulate_Mode) VariableCTL_Add (OuT);
        }    
        for(_i=0;_i<Out_size;) *((char*)OuT+_i++)=0;
        for(_i=1;_i<*_index;_i+=2){//base
                for(_j=1;_j<*_index;_j+=2){//in progress
                        if(_i==_j) continue;
                        if( *(_index+_j) > *(_index+_i) )   *(_index+_j) += *(_index+_i+1);//see nippy Str_IndexCreator algorithm

                }//inner for
        }//for
         for(_i=1;_i<*_index;_i+=2){
            for(_j=0; _j < *(_index+_i+1);_j++ )
                *((char*)OuT + *(_index+_i) + _j/*see algorithm*/)=Fill;
        }
/*printf("\n");
        for(_i=0;_i<Out_size;_i++)printf("%c ",*((char *)OuT+_i));
printf("\n");*/

//printf("\n str=%s \n",_inp);
            
  
        _i=0;
        _j=0;
        while( *(_inp+_j) ){
                while( _i < Out_size ){
                                if(*((char*)OuT+_i) == Fill){
                                        _i++;
                                        continue;
                                }
                                *((char*)OuT+_i++) = *(_inp+_j);
                                break;
                }//while
                _j++;
        }//while
         *((char*)OuT+ Out_size) = *(_inp+_j);
        //printf("%s\n",_inp ); 
        if(memFlag) free(_inp);
        return OuT;
}//if
return (int *)-1;
}
/*#define Str_IndexRemover_AutoMode 1//atuomatic mode
#define Str_IndexRemover_InsulateMode 0//insulate input and output*/
//__________________________________________________________________________________________________________________________________
void *Str_IndexRemover(char *_inp, unsigned int _index){
char    *OuT,memFlag=0,chr_size;
unsigned int Out_size=0,_i =0,_j=0;
  
//printf("addrssIn:%x  ", _inp);
Out_size=Str_len(_inp);
chr_size=sizeof (char);
if(_index > Out_size)   return 0;
  
  if(STR_Mode.Insulate_Mode && VariableCTL_Check (_inp)){
      OuT=(char *)realloc(_inp,(Out_size+1)*chr_size);

       _inp=(char *)calloc(Out_size+1,chr_size);//insulate input
       StrCpy(_inp,OuT);
      memFlag=1;   
  }
  else{
      OuT=(char *)calloc(Out_size,chr_size);
      if(STR_Mode.Insulate_Mode) VariableCTL_Add (OuT);
  }     
                                                         
//printf("%s",_inp);

Out_size=0;
while( *(_inp+_j) ){
                if(_i++ == _index){
                        //printf("\n_inp= %c\n",*_inp);
                        _j++;
                        continue;
                }
                *((char*)OuT + Out_size++) = *(_inp+_j);
                _j++;
        }//while
        *((char*)OuT+ Out_size) = *(_inp+_j);
        if(memFlag) free(_inp);
        //else _MODE_ (_MODE_Auto);
  
//printf("addrssOut:%x\n", OuT);  
return OuT;
}
//_____________________________________________________________________________________________________________________________________
int* FStrSearch(FILE *fp,char* _search){
/*
    1-File must come in read & write format(append mode)

*/
    static int *result=NULL;
    int str_cnt=0,search_cnt=0,i=0,j,_buffer_ [Search_Buffer_Size];//,k=0;

    rewind(fp);
	for(;i<Search_Buffer_Size;) _buffer_[i++]=0;//cleaning buffer
	i=0;
    while(( !feof(fp)) && (i< Search_Buffer_Size)) {
        str_cnt++;
        if(_search[search_cnt]==getc(fp) ) _buffer_[i++]=str_cnt; //founding first char that be samed in both str's
    }


    if( i>0 && Str_len(_search)>1 ){//if find any same char then ...
		for( search_cnt = 1; search_cnt < Str_len(_search); search_cnt++ ){
		    for( str_cnt=0; str_cnt<i; str_cnt++){// select and process other characters form _search.str*
                    rewind(fp);
                    for(j=0;j<(_buffer_[str_cnt]+(search_cnt-1) );j++) getc(fp);

					if( ( _search[search_cnt]!= getc(fp) ) && ( _buffer_[str_cnt]!=0 ) )	_buffer_[str_cnt]=0	;
			}
		}
	}

        result=ignore_zero_(_buffer_);

        return result;
 }
//_______________________________________________________________________________________________________
long int StrNumToInt(char *inp_,char state_){/*0-convert str numbers to equal integer numberinput "23",0(string decimal number) output 23 input "23",
1(string Hex number) output 35*/
	int count=0,pow;
	long int result_=0;
	while(inp_[count]!=0X20 && inp_[count]!=0X00) count++;

	if (state_==0)//decimal numbers
	{
		pow=1;
		for(;count>0;count--){
			result_+=((int)inp_[count-1]-48)*pow;
			pow*=10;
		}
	}
	else{
		pow=0;
		long int buffer_=0;
		for(;count>0;count--,pow+=4){
			if(inp_[count-1] > 47 && inp_[count-1] < 58){

				buffer_=((int)inp_[count-1]-48);
			}

			else if(inp_[count-1] > 64 && inp_[count-1] < 71){//abcdef   a=65  f=70
				buffer_=((int)inp_[count-1]-55);
			}

			else if(inp_[count-1] > 96 && inp_[count-1] < 103){//ABCDEF   A=97  F=102
				buffer_=((int)inp_[count-1]-87);
			}
			result_= result_ | (buffer_ << pow);
		}
	}
	return result_;
}
//_________________________________________________________________________________________________________________________
#define ToEnd 0
void *Str_Extractor(char *_inp,unsigned int FIndex,unsigned int LIndex){
  
unsigned int Out_size=0,_i =0,_j=0;  
unsigned int _len,*OuT,memFlag=0,chr_size;

_len=Str_len (_inp);
Out_size=_len-(LIndex-FIndex);
chr_size=sizeof (char);  
  
if(LIndex==0) LIndex=_len-1;  
if( (LIndex > Str_len (_inp)-1) || (FIndex > LIndex ) ) return 0;
                                                                  
  
//printf("addrssIn:%x  ", _inp);  
  if(STR_Mode.Insulate_Mode && VariableCTL_Check (_inp)){
      OuT=(char *)realloc(_inp,(Out_size)*chr_size);

       _inp=(char *)calloc(Out_size,chr_size);//insulate input
       StrCpy(_inp,OuT);
      memFlag=1;   
  }
  else{
      OuT=(char *)calloc(Out_size,chr_size);
      if(STR_Mode.Insulate_Mode) VariableCTL_Add (OuT);
  }     
                                                         
//printf("%s",_inp);

Out_size=0;
while( *(_inp+_j) ){
                if(_i >= FIndex && _i <=LIndex){
                        //printf("\n_inp= %c\n",*_inp);
                        _j++;_i++;
                        continue;
                }            
                *((char*)OuT + Out_size++) = *(_inp+_j);
                _i++;
                _j++;
        }//while
        *((char*)OuT+ Out_size) = *(_inp+_j);
        if(memFlag) free(_inp);
        //else _MODE_ (_MODE_Auto);
  
//printf("addrssOut:%x\n", OuT);  
return OuT;    
                                                                   
//printf("\n\n" );             
                                                                         
return  _inp;
}
//_________________________________________________________________________________________________________________________
void *StrCutter( char *_inp,int FIndex,int LIndex){//cutting string from first index to last index[include first and last indexes]
	char *_res=0,FLG=0;//index is start from zero
  unsigned int _len=Str_len (_inp);
  
  if(LIndex > (_len-1)) return 0;
  
  if(!STR_Mode.Insulate_Mode){
    _MODE_ (_MODE_Auto);
    FLG=1;
    VariableCTL_Free (_inp);  
  }
                                                    
  if(LIndex < (_len-1) ){
    _res= Str_Extractor (_inp,LIndex+1,0 );
    if(FIndex > 1) _res=Str_Extractor (_res,0 ,FIndex-1 );
    else if(FIndex==1) _res=Str_IndexRemover (_res, 0);
  }//if
  
  else if(FIndex > 1) _res=Str_Extractor (_inp,0 ,FIndex-1 );
  else if(FIndex==1) _res=Str_IndexRemover (_inp, 0);
  
  else if(STR_Mode.Insulate_Mode && VariableCTL_Check (_inp)){ //if(LIndex == _len && FIndex==0)
    _res=_inp;
  }
  else{
     _res=(char *)calloc(_len+1,sizeof (char));//insulate input
    StrCpy(_res,_inp);
  }                                              
  
  if(FLG){
    _MODE_ (_MODE_Insulate);
    VariableCTL_Free (_inp);
  } 
  //printf("\nmode:%d\n", STR_Mode.Insulate_Mode);
	return _res;
}
//_________________________________________________________________________________________________________________________
char *IntToStr(uint32_t inp){
 
signed char *req,i=1;


	while((inp/(int)pow(10,i++))!=0);//set Str_len of array
	req=calloc(i--,sizeof(char));
	while(i-->=0) {
	req[i]=(inp%10 + 48);
	inp/=10;
	}
return req;
}
//___________________0.0______________________________________________________________________________________________________
char *FloatToStr(long double Fl_Part,unsigned char p){
char *ret,*st0,neg_flag=0;
long double _j;

	if (Fl_Part<0) {
			Fl_Part*=-1;
			neg_flag=1;
			}
	int Int_Part = (int) Fl_Part, buff=0,_i;

	while((Int_Part/(long int)pow(10,buff))!=0) buff++;

	Fl_Part=(Fl_Part-Int_Part)*pow(10,p);
	p=0;
	do{
		_j=Fl_Part/(long int)pow(10,p++);

	}while(_j > 1);

	p+=buff;
	ret=calloc(p+3,sizeof(char));
	if(neg_flag){
			 _i=1;
			 ret[0]='-';
	}
	else _i=0;

	st0=IntToStr(Int_Part);
	for(;(neg_flag? _i<=buff : _i<buff); _i++){
				if(neg_flag) ret[_i]=st0[_i-1];
				else ret[_i]=st0[_i];
	}//for

	ret[_i++]='.';
	st0=IntToStr((unsigned long int)Fl_Part);
	for(buff=0;_i<=p; _i++,buff++){
				ret[_i]=st0[buff];
	}//for

	return 	ret;

}
//___________________0.0______________________________________________________________________________________________________
unsigned char IsAlphaStr(char *c){//This function checks whether the passed character or string is alphabetic.return 1 for true and 0 for false
int _i=0;
	while(*c){
			 if(  ( ((*c<97) && (*c>90))  || (*c>122) || (*c<65) ) && (*c != 32) ) return 0;
			 *c++;_i++;
			 }
	if(_i==0) return 0;
	return 1;
}
//_________________________________________________________________________________________________________________________
unsigned char IsAlpha(int c){//This function checks whether the passed single character is alphabetic.return 1 for true and 0 for false
			 if( (c<97 && c>90)  || c>122 || c<65 ) return 0;
	return 1;
}
//_____________________________________________________________________________________________________________
char IsNumber(char c){
  if(c<48 || c>57)  return 0;
 return 1; 
}
//__________________________________________________________________________________________________________________________
void StrCpy(char *_dest,const char *_st0){//COPY STR0 TO DEST
while(*_dest++=*_st0++);
}
//________________________________________________________________________________________________________________________
unsigned char StrComp(char *_st1, char *_st2){//compare str1(string format) with str2(string format) (true:return 1 False:return 0)
	while(*_st1 || *_st2) if(*_st1++ != *_st2++) return 0;
	return 1;
}
//________________________________________________________________________________________________________________________
/*
 * 
 * parsing string into Related components
 *
 * Ex:"  hello world ?+123[] -63/2 h _lio"
 * Mode_Math: 
 * error out: return 0;
 * 
 */
#define Str_Parse_MathMode 2
#define Str_Parse_WordMode 1
#define Str_Parse_DefaultMode 0

#define IsMath_Operator(x)  ( (x)== '+' || (x)== '*' || (x)== '-' ||  \
                              (x)== '/' || (x)== '^' || (x)== '%'  )?  \
                              (1):(0)

#define IsRanege_Specifier(x)  (  (x)== '(' || (x)== ')' || (x)== '['  ||  \
                                  (x)== ']' || (x)== '{' || (x)== '}'    )?  \
                                  (1):(0)
#define SIndex(z) *(_st+NIndex+z)

void **Str_Parse(char *_st,char Mode,char *Math_validate_FLG/*1 true | 0 false*/){
  unsigned int Size=0,_Count=0;//last index and first index and current index
  unsigned int FIndex=0,NIndex=0;
  unsigned int **Out=NULL;
  char _buf,Err=-1;
  
  if(Str_len(_st)<2) return 0;
  while(*_st == 32) _st++;
  
  Size=sizeof( int*);
  Out=calloc (1,Size);
  *Out=calloc (1,sizeof(int));//used for out[0][0]
  _MODE_ (_MODE_Insulate);
  
  if((Mode&1) && !(Mode>>1) ){//WordMode 1
    
    while( SIndex (0) ){
      //terms
        if( IsAlpha (SIndex (0)) || SIndex (0)== 95 ){
            while(SIndex (0) && (IsAlpha (SIndex (0)) || IsNumber(SIndex (0)) || SIndex (0)== 95 || SIndex (0)==45) ) NIndex++;           
            _Count++; 
            Out= realloc (Out,Size*(_Count+1));
          //printf("\nloop1: **nindex= %d**", NIndex);
          *(Out+_Count)=(unsigned int *) StrCutter (_st, FIndex, NIndex-1);  
          //printf("%s\n", Out[1]);
          }//if
        else 
          {
            while(SIndex(0)!=32 && SIndex(0)) NIndex++;//word detection
            FIndex = ++NIndex;
          }//else
    }//while
  }//if wordmode
  
  else if ((Mode&2) && !(Mode>>2) ){//MathMode  2
                                    
    while( SIndex (0) ){
        
        Mode=1;             
        if( IsAlpha (SIndex (0)) || SIndex (0)== 95  ){
            FIndex=NIndex; 
            while(IsAlpha (SIndex (0)) || IsNumber(SIndex (0)) || SIndex (0)== 95 || SIndex (0)==45 ) NIndex++;                   
            if(Err)Err=1;
            
        }//if
        else if(  IsNumber(SIndex (0))  ){
             FIndex=NIndex; 
             while(IsNumber(SIndex (0))) NIndex++; 
             if(Err)Err=2;
          }//if 
        else if(  IsMath_Operator(SIndex (0))  ){  
            FIndex=NIndex++;
            if(Err)Err=3;
          
        }
        else if(  IsRanege_Specifier(SIndex (0))  ){
             FIndex=NIndex++;
             _buf=-15;
            
        }
        else 
          {
            while(SIndex(0)!=32 && SIndex(0)) NIndex++;//word detection
            FIndex = ++NIndex;
            Mode=0;
          }//else
        if(Mode){
            _Count++; 
            Out= realloc (Out,Size*(_Count+1));
            *(Out+_Count)=(unsigned int *) StrCutter (_st, FIndex, NIndex-1);
        }//if
        if(Err && Mode){
          if(_buf==Err) Err=0; 
          _buf=Err;       
        }
    }//while
    (Err)? (*Math_validate_FLG=1): (*Math_validate_FLG=0);  
  }
                                                                                   
  else{//DefaultMode 0  
    while( SIndex (1) ){
        NIndex++;
        if(  SIndex (0)== Mode ){
            while(SIndex (0) && SIndex (0)!= Mode ) NIndex++;           
            _Count++; 
            Out= realloc (Out,Size*(_Count+1));
          *(Out+_Count)=(unsigned int *) StrCutter (_st, FIndex, NIndex-1);  
          FIndex=NIndex+1;
          
        }//if
          
    }//while
    _Count++;
    Out= realloc (Out,Size*(_Count+1));
          *(Out+_Count)=(unsigned int *) StrCutter (_st, FIndex, NIndex-1);  
          FIndex=NIndex; 
  }//else
  
  **Out=(unsigned int)(_Count+1);
  _MODE_ (_MODE_Reset);
  return (void **)Out;
}
//__________________________________________________________________________________________________________________
char *StrCat(char *_dest,const char *_st0){
unsigned int _len=Str_len(_dest);
StrCpy((_dest+_len),_st0);
return _dest;
}
//__________________________________________________________________________________________________________________
char *StrnCat(char *_dest,int _n, ...){
  unsigned int _len=Str_len(_dest);
  va_list valist;//declare va_list variable
  va_start(valist,_n);//initial valist for number of arguments

  while(_n>0){
        
    StrCpy((_dest+_len),va_arg(valist,char *));
    _len=Str_len(_dest);
    _n--;
  }//while

  va_end(valist);
  return _dest;
}




