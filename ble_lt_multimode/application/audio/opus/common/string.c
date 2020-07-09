
#ifndef WIN32
#include "types.h"
#include "string.h"
#include "../common/assert.h"

char* strcpy(char * dst, const char * src){
	char *s = dst;
	while ((*dst++ = *src++))
		;
	return s;
}

int mystrcpy(char * dst, const char * src) {
	char *s = dst;
	while ((*dst++ = *src++))
		;
	return (dst - s - 1);
}

char * strchr(const char *s, int c) {
	do {
		if (*s == c) {
			return (char*) s;
		}
	} while (*s++);
	return (0);
}

char* strcat ( char * dst , const char * src )  
{  
    char * cp = dst;  
    while( *cp )  
        cp++;                       /* find end of dst */  
    while( (*cp++ = *src++) ){}       /* Copy src to end of dst */  
    return( dst );                  /* return dst */  
}

int mystrcat ( char * dst , const char * src ){  
    char * cp = dst;  
    while(*cp )  
        cp++;                       /* find end of dst */  
    while( (*cp++ = *src++) ){}       /* Copy src to end of dst */  
    return (cp - dst - 1);                  /* return dst */  
}

int memcmp(const void * m1, const void *m2, unsigned int n) {

	unsigned char *s1 = (unsigned char *) m1;
	unsigned char *s2 = (unsigned char *) m2;

	while (n--) {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		s1++;
		s2++;
	}
	return 0;
}

void *
memchr(register const void * src_void, int c, unsigned int length) {
	const unsigned char *src = (const unsigned char *) src_void;

	while (length-- > 0) {
		if (*src == c)
			return (void *) src;
		src++;
	}
	return NULL;
}

void * memmove(void * out, const void * in, unsigned int len) {
	u8* dst = (u8*)out;
	u8* src = (u8*)in;
	if(dst < src){
		while (len--)
			*dst++ = *src++;
	}else{
		src = src + (len - 1);
		dst = dst + (len - 1);
		while (len--)
			*dst-- = *src--;
	}
	return out;
}

void * memset(void * dest, int val, unsigned int len) {
	register unsigned char *ptr = (unsigned char*) dest;
	while (len-- > 0)
		*ptr++ = (unsigned char)val;
	return dest;
}

void * memcpy(void * dest, const void * src, unsigned int n) {
#if(1)	
	return memmove(dest, src, n);
#else
	char * d = (char *)dest;
	char * s = (char *)src;

	while (n--)
		*d++ = *s++;

	return dest;
#endif	
}

unsigned int strlen(const char *str) {

	unsigned int len = 0;

	if (str != NULL) {
		while (*str++) {

			len++;

		}
	}

	return len;
}

int strcmp(const char* firstString, const char* secondString) {
	while (*firstString == *secondString) {
		if (*firstString == '\0') {
			return 0;
		}
		++firstString;
		++secondString;
	}
	if (((unsigned char) *firstString - (unsigned char) *secondString) < 0) {
		return -1;
	}
	return 1;
}

int strncmp(const char* firstString, const char* secondString, unsigned int n) {
	while (*firstString == *secondString && (n -- > 0)) {
		if (*firstString == '\0') {
			return 0;
		}
		++firstString;
		++secondString;
	}
	if (((unsigned char) *firstString - (unsigned char) *secondString) < 0) {
		return -1;
	}
	return 1;
}

char * strncpy(char *s, const char *t, unsigned int n) {
	char *p = s;
	unsigned int i = 0;

	if (!s)
		return s;

	while (t && i < n) {
		*s++ = *t++;
		i++;
	}

	if (!t) {
		do
			*s++ = '\0';
		while (i++ < n);
	}
	return p;
}

int ismemzero(void *data, unsigned int len){
	u8 *p = (u8*)data;
	for(int i = 0; i < len; ++i){
		if(*p){
			return 0;
		}
		++p;
	}
	return 1;
}

int ismemff(void *data, unsigned int len){
	u8 *p = (u8*)data;
	for(int i = 0; i < len; ++i){
		if(*p != 0xff){
			return 0;
		}
		++p;
	}
	return 1;
}

int isspace(int c){
 
  /* first check gives quick exit in most cases */
  return(c<=' ' && (c==' ' || (c<=13 && c>=9)));
   
}

int isdigit(int c){
  return (c<='9' && c>='0');
   
}

/*
** atoi(s) - convert s to integer.
*/
int atoi(const char *s){

	int sign, n;
	while(isspace(*s)) ++s;

	sign = 1;
	switch(*s) {
		case '-': sign = -1;
		case '+': ++s;
	}
	n = 0;
	while(isdigit(*s)) n = 10 * n + *s++ - '0';
	return (sign * n);
}

int myitoa(int num,char *buf,int radix){  
	const char index[]= "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 

	u8 str[13];
	int i = (13-1), s = 0; 
	str[i--] = '\0';
	
	if(radix == 10 && num < 0){
		num = (unsigned int)(-num); 
		s = 1;
	}
	
	do{
		str[i--] = index[num % radix]; 
		num /= radix; 
	}while(num);
	if(s){
		str[i--]='-'; 
	}
	int cnt = 11 - i;
	while((++i) < 13){
		*(buf++) = str[i];
	}
	return cnt; 
	
} 

char tohex(unsigned char c){
	if(c <= 9){
		return c + '0';
	}else if(c <= 15){
		return c - 10 + 'A';
	}
	return 0;
}

void bufToHexString(u8* buf, int len, u8* s){
	for(int i = 0; i < len; ++i){
		*s++ = tohex(buf[i] >> 4);
		*s++ = tohex(buf[i] & 0x0f);
	}
}

// 
void itohexstring(u32 c, u8 *buf){
	bufToHexString((u8*)(&c), 4, buf);
}

#if(0)
unsigned short hextoint(u8 *s){
	u8 h = *s;
	u8 l = *(s+1);
	if(h > '9'){
		h -= 'A' + 10;
	}else{
		h -= '0';
	}
	if(l > '9'){
		l -= 'A' + 10;
	}else{
		l -= '0';
	}
	return h * 16 + l;
}
#endif
void stringToMac(u8 *s, int len, u8 *mac){
	u8 h = 1;
	while(len--){
		u8 valid = 0;
		u8 c = *s;
		u8 hf;
		if(c >= 'A' && c <= 'F'){
			hf = c - 'A' + 10;
			valid = 1;
		}else if(c >= 'a' && c <= 'f'){
			hf = c - 'a' + 10;
			valid = 1;
		}else if(c >= '0' && c <= '9'){
			hf = c - '0';
			valid = 1;
		}
		if(valid){
			if(h){
				*mac = hf << 4;
				h = 0;
			}else{
				*mac |= hf;
				mac++;
				h = 1;
			}
		}
		s++;
	}
}

void macToString(u8 *mac, u8 *s){
	for(int i = 5; i >= 0; --i){
		*s++ = tohex(mac[i] >> 4);
		*s++ = tohex(mac[i] & 0x0f);
	}
}

char *strstr(const char *strLong, const char *strShort)  
{  
    char *cp = (char *)strLong;
    char *pL = NULL;  
    char *pS = NULL;  
      
    if ('\0' == *strShort)  
        return (char *)strLong;  
      
    while ('\0' != *cp)  
    {  
        pL = cp;  
        pS = (char *)strShort;  
          
        while ( ('\0' != *pL) && ('\0' != *pS) && (*pL == *pS) )  
        {  
            pL++;  
            pS++;  
        }  
          
        if ('\0' == *pS)
            return cp;  
          
        cp++;
    }  
      
    return NULL;      
}  

#endif
void * memset4(void * dest, int val, unsigned int len) {
	int *p = (int*)dest;
	len = len >> 2;
	for(int i = 0; i < len; ++i){
		*p++ = val;
	}
	return dest;
}

void * memmove4(void * out, const void * in, unsigned int len) {
	int* dst = (int*)out;
	int* src = (int*)in;
	len >>= 2;
	if(dst < src){
		while (len--)
			*dst++ = *src++;
	}else{
		src = src + (len - 1);
		dst = dst + (len - 1);
		while (len--)
			*dst-- = *src--;
	}
	return out;
}
// for performance, assume lenght % 4 == 0,  and no memory overlapped
void memcpy4(void * d, const void * s, unsigned int len){
	int* dst = (int*)d;
	int* src = (int*)s;
	len >>= 2;
	while(len --){
		*dst++ = *src++;
	}
}

