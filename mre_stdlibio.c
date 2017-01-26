/*
Standard Library stdlib stdio MRE implementation / associations
*/

#include "share.h"
#include "mre_stdlibio.h"

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////// MRE IMPLEMENTATION OF FILE FUNCTIONS /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////

void *malloc_mre(size_t size){
	/* 
	MSC:
	void* malloc(size_t Size);

	MRE:
	void *vm_malloc(int size);
	*/
	
	vm_malloc((int)size);

}

void free_mre(void *memory){
	/* 
	MSC:
	void free(void *Memory);

	MRE:
	void vm_free(void* ptr);
	*/

	vm_free(memory);

}

void *realloc_mre(void *memory, size_t new_size){
	/*
	MSC:
	void* realloc(void *Memory, size_t NewSize);

	MRE:
	void *vm_realloc(void* p, int size);
	*/
	/* Update: This will fail if memory is 0x00000000. You'd except it to but in C, it realises 
	that it is nothing and allocates new memory. The difference between realloc and free then malloc
	is that realloc preserves the contents. If memory variable is none, then we don't care because
	there is no array or information in this. So I think we should force this implementation to 
	check and manually call malloc */

	if (memory == NULL){
		return vm_malloc((int)new_size);
	}
	vm_realloc(memory, (int)new_size);
}

	

/*
	Some cases since file handle is a pointer as required and in MRE its just an integer, we 
	dereference it in the implementation.
*/

MZ_FILE *mz_fopen_mre(const char *pFilename, const char *pMode)
{ 
  VMFILE file_handle;
  MZ_FILE *pHandle;
  MZ_FILE *npHandle = (VMFILE *)vm_malloc(1*sizeof(VMFILE));
  VMUINT mode;
  VMUINT binary;

  if (pMode == "wb"){
	  mode = MODE_CREATE_ALWAYS_WRITE;
	  binary = TRUE;
  }
  if (pMode == "w"){
	  mode = MODE_WRITE;
	  binary = FALSE;
  }
  if (pMode == "r"){
	  mode = MODE_READ;
	  binary = FALSE;
  }
  if (pMode == "r+b"){
	  mode = MODE_WRITE;
	  binary = TRUE;
  }
  if (pMode == "rb"){
	  mode = MODE_READ;
	  //mode = MODE_WRITE; //"rb" somewhere but expects to be written. 
	  //mode = MODE_APPEND;
	  //mode = MODE_CREATE_ALWAYS_WRITE;
	  binary = TRUE;
  }
  if(pMode == "a"){
	  mode = MODE_APPEND;
	  binary = FALSE;
  }
  if(pMode == "ab"){
	  mode = MODE_APPEND;
	  binary = TRUE;
  }

	/*
	  VMSTR vfile_path;
	  //same as
	  VMINT8 *vmfile_path;
	  //same as
	  char *cfile_path;
	*/

  file_handle = mre_open_file((VMSTR)pFilename, mode, binary);
  //pHandle = (VMFILE *)vm_malloc(1*sizeof(VMFILE));
  if (file_handle < 0){
	  return NULL;
  }
  pHandle = &file_handle;
  *npHandle = file_handle;
  //return &file_handle;
  return npHandle;
}

// Freopen method : The effect of freopen is to reassign a handle to another source. ^VS
MZ_FILE *mz_freopen_mre(const char *pPath, const char *pMode, MZ_FILE *old_file_handle)
{
	/*Do you think we should do something with the old_file_handle ? 
	Maybe close or flush etc ? ^VS */ 
	//closing the old handle
	vm_file_close(*old_file_handle);

	/* Testing : If we can open a zip file with read write when already opened, etc */

	return mz_fopen_mre(pPath, pMode);
}

void close_mre(MZ_FILE *file_handle){ //Maybe you have to remove the pointer
	vm_file_close(*file_handle);
}

/* fread to mre's implementation of file read*/
size_t fread_mre(void *pBuf, int elementSize, size_t n, MZ_FILE *pFile){
	/*
	A typical fread function is like so:
	return MZ_FREAD(pBuf, 1, n, pZip->m_pState->m_pFile);
	which is fread(data, 1 byte, size in (1 byte), file handle);
	MRE has vm_file_read
	vm_file_read(file handle, data, size in bytes, read_bytes);
	*/ 
	VMUINT read_bytes;
	VMINT file_read_result;
	file_read_result = vm_file_read(*pFile, pBuf, n, &read_bytes);
	/* Check file_read_result value */
	if (file_read_result == 0){
		printf("\nError or end of file reached.\n");
	}
	return (size_t) read_bytes; //convert to size_t which is basically unsigned int
}


size_t fwrite_mre(const void *pBuf, int element_size, size_t n, MZ_FILE *pFile){
	/* fwrite in MSC is:
	static size_t mz_zip_file_write_callback(void *pOpaque, mz_uint64 ofs, const void *pBuf, size_t n)
	{
		(void)ofs; return MZ_FWRITE(pBuf, 1, n, (MZ_FILE*)pOpaque);
	}
	MZ_FWRITE(pBuf, 1, n, pZip->m_pState->m_pFile);
	*/

	//size_t fwrite(const void *str, size_t size, size_t count, file *file);

	/*
	MRE:
	return fwrite(data_to_write, size_of_data, count, file);
	 vs
	vm_file_write(file handle, data, length, &written);
	*/
	
	VMINT written_result;
	VMUINT written_bytes;
	VMUINT length;
	VMINT tell_position, tell_end_position, tell_origi_position;
	VMINT commit_result;
	length = (VMUINT)n;

	/*
	// Testing offset is set or not 
	tell_position = vm_file_tell(*pFile);

	// Testing: Check whats the end of the file 
	vm_file_seek(*pFile, 0, BASE_END);
	tell_end_position = vm_file_tell(*pFile);

	// seek back to tell_position 
	vm_file_seek(*pFile, tell_position, BASE_BEGIN);
	tell_origi_position = vm_file_tell(*pFile);
	*/ 

	written_result = vm_file_write(*pFile, (void *)pBuf, n, &written_bytes);
	//qn: close the file handle ? ^VS
	//Maybe we commit it ?
	commit_result = vm_file_commit(*pFile);
	if (commit_result != 0 ){
		printf("\nCouldn't commit write to file. Please check.\n");
	}
	return (unsigned int) written_result;
		//which one is correct ?
	return (unsigned int) written_bytes;
	//return written_result;

}

__int64  _ftelli64_mre(MZ_FILE *pFile){
	/* MSC be like:
		MZ_FTELL64(pZip->m_pState->m_pFile);
	*/
	VMINT pointer_position;
	VMFILE file_handle;

	/* testing: */
	/* end of testing 
		Result: You DONT need to close the file that you just created
		and open it again before you do any sort of tell-ing / seek-ing.
	*/

	//pointer_position = vm_file_tell(*pFile);
	file_handle = *pFile;
	pointer_position = vm_file_tell(file_handle);
	return (__int64) pointer_position;
}

int _fseeki64_mre(MZ_FILE *file_handle, long offset, int origin){
	/* MSC be like:
	fseek(FILE* _File, long _Offset, int _Origin);
	MZ_FSEEK64(pFile, 0, SEEK_END);
	MRE:
	VMINT vm_file_seek(VMFILE handle, VMINT offset, VMINT base);
	Origin according to MRE (vmio.h):
		#define BASE_BEGIN		1
		#define BASE_CURR		2
		#define BASE_END		3
	Origin according to MSC (stdio.h):
		#define SEEK_CUR    1
		#define SEEK_END    2
		#define SEEK_SET    0
	*/

	VMINT seek_result;
	switch(origin) {
	   case 1  :
			origin = BASE_CURR;
			break; 
		
	   case 2  :
			origin = BASE_END;
			break; 
	  
	   case 0:
			origin = BASE_BEGIN;
			break;

	   default : 
		   origin = BASE_BEGIN;
			break;
	}

	seek_result = vm_file_seek(*file_handle, (int) offset, origin); //cant cast int to long I think ^VS
	return seek_result;
}

int _stat_mre(const char* file_path, void *stat_object){
	/* If file found, return 0: 
	if (MZ_FILE_STAT(pZip_filename, &file_stat) != 0)
	{
		// Create a new archive.
	}
	*/
	VMBOOL file_exists;
	int stat_result = -1; //Default -1.

	/* Does this file exist?
		VMBOOL does_this_file_exist(VMSTR filename);
	*/
	file_exists = does_this_file_exist((VMSTR)file_path);
	if (file_exists == TRUE){
		stat_result = 0;
	}else{
		stat_result = -1;
	}

	return stat_result;
}

int fflush_mre(MZ_FILE *file_handle){
	/*MSC: 
	int fflush(FILE *file);
	*/
	VMINT fflush_result;
	VM_FS_HANDLE fs_hdl;


	fflush_result = vm_fs_async_commit(*file_handle);
	//fflush_result = vm_fs_async_commit(fs_hdl);
	//or
	//fflush_result = vm_fs_async_close(fs_hdl);

	return fflush_result;
	
}

int remove_mre(const char *file_path){ 
	/* 
	MSC:
	int remove(const char *file_path);

	MRE:
	VMINT vm_file_delete(const VMWSTR filename);	
	*/

	int remove_result;
	VMWCHAR wfile_path[MRE_STR_SIZE_MAX + 1];

	/* VMSTR to VMWSTR string format conversion */
	vm_ascii_to_ucs2 (wfile_path, MRE_STR_SIZE_MAX, file_path);
	remove_result = vm_file_delete(wfile_path);

	return remove_result;
}

int fclose_mre(MZ_FILE *file_handle){
	/*
	MSC:
	int fclose(FILE *file);
	*/ 
	VMINT close_result;
	vm_file_close(*file_handle); //Doesn't return anything (void)
	close_result = 0; //Going to have to override it as always close success.
	return close_result;
}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////// MRE IMPLEMENTATION //////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////