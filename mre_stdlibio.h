/* 
	Header file for Standard Library stdlib stdio and MRE implementation/association
*/

#define MZ_FILE VMFILE

void *malloc_mre(size_t size);
void free_mre(void *memory);
void *realloc_mre(void *memory, size_t new_size);
MZ_FILE *mz_fopen_mre(const char *pFilename, const char *pMode);
MZ_FILE *mz_freopen_mre(const char *pPath, const char *pMode, MZ_FILE *old_file_handle);
void close_mre(MZ_FILE *file_handle);
size_t fread_mre(void *pBuf, int elementSize, size_t n, MZ_FILE *pFile);
size_t fwrite_mre(const void *pBuf, int element_size, size_t n, MZ_FILE *pFile);
__int64  _ftelli64_mre(MZ_FILE *pFile);
int _fseeki64_mre(MZ_FILE *file_handle, long offset, int origin);
int _stat_mre(const char* file_path, void *stat_object);
int fflush_mre(MZ_FILE *file_handle);
int remove_mre(const char *file_path);
int fclose_mre(MZ_FILE *file_handle);