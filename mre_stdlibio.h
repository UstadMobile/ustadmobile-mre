/* 
	Header file for Standard Library stdlib stdio and MRE implementation/association
	Author: Varuna Singh, UstadMobile
*/

/*
	Defining File type for MRE. This will be passed  around and should be also 
	set at library end so it can pass its version. 
*/
#define MZ_FILE VMFILE

/*****************************************************************************
 * FUNCTION
 *  malloc_mre
 * DESCRIPTION
 *	Malloc function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void *malloc_mre(size_t size);

/*****************************************************************************
 * FUNCTION
 *  free_mre
 * DESCRIPTION
 *	free function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void free_mre(void *memory);

/*****************************************************************************
 * FUNCTION
 *  realloc_mre
 * DESCRIPTION
 *  realloc function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void *realloc_mre(void *memory, size_t new_size);

/*****************************************************************************
 * FUNCTION
 *  mz_fopen_mre
 * DESCRIPTION
 *   file open fopen function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
MZ_FILE *mz_fopen_mre(const char *pFilename, const char *pMode);

/*****************************************************************************
 * FUNCTION
 *  mz_freopen_mre
 * DESCRIPTION
 *  file change freopen open function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
MZ_FILE *mz_freopen_mre(const char *pPath, const char *pMode, MZ_FILE *old_file_handle);

/*****************************************************************************
 * FUNCTION
 *  close_mre
 * DESCRIPTION
 *  file close function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void close_mre(MZ_FILE *file_handle);

/*****************************************************************************
 * FUNCTION
 *  fread_mre
 * DESCRIPTION
 *  file read f_read function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
size_t fread_mre(void *pBuf, int elementSize, size_t n, MZ_FILE *pFile);

/*****************************************************************************
 * FUNCTION
 *  fwrite_mre
 * DESCRIPTION
 *  file write fwrite function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
size_t fwrite_mre(const void *pBuf, int element_size, size_t n, MZ_FILE *pFile);

/*****************************************************************************
 * FUNCTION
 *  _ftelli64_mre
 * DESCRIPTION
 *  file tell ftell function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
__int64  _ftelli64_mre(MZ_FILE *pFile);

/*****************************************************************************
 * FUNCTION
 *  _fseeki64_mre
 * DESCRIPTION
 *  file seek fseek function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
int _fseeki64_mre(MZ_FILE *file_handle, long offset, int origin);

/*****************************************************************************
 * FUNCTION
 *  _stat_mre
 * DESCRIPTION
 *  file stat function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
int _stat_mre(const char* file_path, void *stat_object);

/*****************************************************************************
 * FUNCTION
 *  fflush_mre
 * DESCRIPTION
 *  file flush fflush function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
int fflush_mre(MZ_FILE *file_handle);

/*****************************************************************************
 * FUNCTION
 *  remove_mre
 * DESCRIPTION
 *  file remove function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
int remove_mre(const char *file_path);

/*****************************************************************************
 * FUNCTION
 *  fclose_mre
 * DESCRIPTION
 *  file close, fclose function association with MRE
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
int fclose_mre(MZ_FILE *file_handle);