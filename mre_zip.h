/* 
mre_zip.h: To handle zip files like epub
*/

void read_zip_file(VMSTR file_path);

void read_file_in_zip_file(VMSTR file_path, VMSTR filename);

void extract_zip_file(VMSTR file_path, VMSTR dest_folder);

void extract_file_from_zip_file(VMSTR file_path, VMSTR filename);

int test_zip(int argc, char *argv[]);