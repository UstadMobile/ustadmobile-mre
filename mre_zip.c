/* 
mre_zip.c: Zip related definitions
*/

#include "share.h"
#include "mre_zip.h"

void read_zip_file(VMSTR file_path){
	printf("\nRead Zip File");

}

void read_file_in_zip_file(VMSTR file_path, VMSTR filename){
	printf("\nRead a file from zip file");
}

void extract_zip_file(VMSTR file_path, VMSTR dest_folder){
	printf("\nExtract Zip File");
}

void extract_file_from_zip_file(VMSTR file_path, VMSTR filename){
	printf("\n Extract a file from zip file");
}

/* Commenting out cause we are defining it elsewhere.
	This was just a placeholder.
int test_zip(int argc, char *argv[]){

	printf("\n\nHello Everybody\n\n");
	return 1;
}
*/