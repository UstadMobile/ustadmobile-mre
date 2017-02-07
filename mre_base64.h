/* 
	MRE Base: mre_base64.h Base64 related method header file. Work In Progress.
*/


//base64
#define table "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_INPUT_SIZE 57

VMBOOL isbase64(char c);

char value(char c);

int UnBase64(unsigned char *dest, const unsigned char *src, int srclen);

