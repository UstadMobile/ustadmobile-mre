/* 
	MRE Base: mre_base64.c - base64 stuff. Work In Progress.
*/

#include "share.h"
#include "mre_base64.h"


VMBOOL isbase64(char c)
{
   return c && strchr(table, c) != NULL;
}

char value(char c)
{
   const char *p = strchr(table, c);
   if(p) {
      return p-table;
   } else {
      return 0;
   }
}

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen)
{
   
   unsigned char *p = dest;
   *dest = 0;
   if(*src == 0) 
   {
      return 0;
   }
   do
   {

      char a = value(src[0]);
      char b = value(src[1]);
      char c = value(src[2]);
      char d = value(src[3]);
      *p++ = (a << 2) | (b >> 4);
      *p++ = (b << 4) | (c >> 2);
      *p++ = (c << 6) | d;
      if(!isbase64(src[1])) 
      {
         p -= 2;
         break;
      } 
      else if(!isbase64(src[2])) 
      {
         p -= 2;
         break;
      } 
      else if(!isbase64(src[3])) 
      {
         p--;
         break;
      }
      src += 4;
      while(*src && (*src == 13 || *src == 10)) src++;
   }
   while(srclen-= 4);
   *p = 0;
   return p-dest;
}