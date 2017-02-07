/*
	MRE File: mre_file.c - File realted source code

	Author: Varuna Singh, UstadMobile
*/


#include "share.h"
#include "mre_file.h"
#include "mre_display.h"

/*****************************************************************************
 * FUNCTION
 *  mre_create_file
 * DESCRIPTION
 *  This function creates a file with given name in a drive set earlier 
 * PARAMETERS
 *  file name				[IN]      name of file
 * RETURNS
 *	result                  [OUT]     some code for error and success   
*****************************************************************************/
VMINT mre_create_file (VMSTR file_name)
{
    VMFILE fc;
    VMCHAR f_name[MRE_STR_SIZE_MAX + 1];
    VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    VMCHAR show_text[MRE_STR_SIZE_MAX + 1];
    VMUINT written;
	void *test_ptr;
	int ptr_size;
    vm_log_debug("mre_create_file function starts with file name : %s ", file_name);

	//Testing only. Result: works
	/*
	ptr_size = 1024;
	test_ptr = vm_malloc(ptr_size);
	*/

	sprintf (f_name, "%c:\\%s", mre_get_drv(), file_name);
    vm_log_file("file name\n");
	vm_log_file(f_name);
	vm_log_file("\n");
	

	/* string format conversion */
	vm_ascii_to_ucs2 (f_wname, MRE_STR_SIZE_MAX, f_name);

	/* file is created here */
	fc = vm_file_open (f_wname, MODE_CREATE_ALWAYS_WRITE, FALSE);
	vm_log_debug("file open hdl : %d", fc);
    

    if (fc < 0)
	{
		vm_log_file ("mre_create_file function exits\n");
        return MRE_FILE_CREATE_FAILED;
	}
	else 
	{
		vm_file_write (fc, "", 1,  & written);
        vm_file_close (fc);
		sprintf (show_text, "file created:%s", f_name);
        mre_show_text (show_text);
        vm_log_debug (" %s mre_create_file function exits", show_text);
		return MRE_SCILENT;
	}
    
}

/*****************************************************************************
 * FUNCTION
 *  mre_write_file
 * DESCRIPTION
 *  This function writes on the file given 
 * PARAMETERS
 *  file name				[IN]      name of file
 * RETURNS
 *	result                  [OUT]     some code for error and success   
*****************************************************************************/
VMINT mre_write_file (VMSTR file_name)
{
	VMFILE f_write; 
	VMCHAR f_name[MRE_STR_SIZE_MAX + 1] ;
    VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    VMINT written_result;
    VMUINT written;
    VMCHAR show_text[MRE_STR_SIZE_MAX + 1];
    vm_log_debug ("mre_write_file function starts with file name : %s ", file_name);
    sprintf (f_name, "%c:\\%s", mre_get_drv(), file_name);
    vm_log_debug("file name : %s", f_name);

	/* string format conversion */
	vm_ascii_to_ucs2 (f_wname, MRE_STR_SIZE_MAX, f_name);

	/* file is opened here */
	f_write = vm_file_open (f_wname, MODE_WRITE, FALSE);
    vm_log_debug("file open hdl : %d", f_write);

	if (f_write < 0)
	{
        return MRE_FILE_OPEN_FAILED;
	}

        written_result = vm_file_write (f_write, 
							//vm_gb2312_string (mre_get_textbox_text ()), 
							mre_get_textbox_text (), 
								sizeof (VMWCHAR) * (vm_wstrlen (mre_get_textbox_text ())+1),  
									& written);
        vm_log_debug("file write result : %d", written_result);
        vm_free (mre_get_textbox_text ());
        if (written_result == 0)
        {
            vm_file_close (f_write);
            return MRE_FILE_NOT_WRITTEN;
        }

	    else 
	    {
		    vm_file_close (f_write);
			sprintf (show_text, "Written in file: success");
            mre_show_text (show_text);
		    return MRE_SCILENT;
	    }
   
    //vm_log_debug ("mre_write_file function exits");
    
}

/*****************************************************************************
 * FUNCTION
 *  does_this_file_exist
 * DESCRIPTION
 *  Checks if the givenn filename exists 
 * PARAMETERS
 *  VMSTR filename			  [IN]      filename in VMSTR
 * RETURNS
 *	VMBOOL TRUE if file exists or FALSE if it doesnt
*****************************************************************************/
VMBOOL does_this_file_exist(VMSTR filename){
	VMWSTR wfilename;
	VMINT wfilename_size;
	VMFILE test_handle;

	wfilename_size = (strlen(filename) + 1) * 2;
	wfilename = vm_malloc(wfilename_size);

	vm_ascii_to_ucs2 (wfilename, MRE_STR_SIZE_MAX, filename);
	return does_this_wfile_exist(wfilename);
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*	mre_write_line_to_file
* DESCRIPTION
*	Writes line to file 
* PARAMETERS
*	file_name just the name as string in root directory, line as string
* RETURNS
*	MRE_FILE_NOT_WRITTEN if fail, MRE_SCILENT for success
*****************************************************************************/
VMINT mre_write_line_to_file (VMSTR file_name, VMSTR line)
{
	/* local variables */
	VMFILE  file_handle;
	VMCHAR  file_path[MRE_FILE_NAME_SIZE];
	VMWCHAR wfile_name[MRE_FILE_NAME_SIZE];
	VMWCHAR wline[MRE_STR_SIZE_MAX + 1];
    VMUINT written;
	VMUINT  bytes_written;
	VMINT line_bytes;
	
	//const VMINT line_size = sizeof (VMCHAR) * (vm_wstrlen (line)+1); //+2 for new line
	//VMINT line_bytes = sizeof(VMCHAR) * (vm_strlen(line) + 1 )
	//VMSTR line_new[1000];
	//sprintf(line_new,"%s\n", line);

	/* Open file */
	sprintf(file_path,"%c:\\%s",mre_get_drv(),file_name);
	vm_ascii_to_ucs2(wfile_name, MRE_STR_SIZE_MAX, file_path);

	vm_ascii_to_ucs2(wline, MRE_STR_SIZE_MAX, line);
	line_bytes = sizeof(VMWCHAR) *(vm_wstrlen (wline)+1); 

	//g_mre_textbox_text =  (sizeof (VMWCHAR) * (vm_wstrlen (wline)+1));
    //vm_wstrcpy (g_mre_textbox_text, text);

	file_handle = vm_file_open(wfile_name, MODE_APPEND, FALSE);
	if (file_handle < 0){
		file_handle = vm_file_open(wfile_name, MODE_CREATE_ALWAYS_WRITE, FALSE);
	}
	if(file_handle < 0)
	{
		vertical_scrolling_text("Write to file fail....");
	}else{
		/* write to file */
		bytes_written = vm_file_write (file_handle, wline, line_bytes, &written);
		vm_log_debug("%d bytes has been written in file", bytes_written);
		
        if (bytes_written == 0)
        {
            vm_file_close (file_handle);
            return MRE_FILE_NOT_WRITTEN;
        }

	    else 
	    {
		    vm_file_close (file_handle);
			//sprintf (show_text, "Written in file: success");
            //mre_show_text (show_text);
		    return MRE_SCILENT;
	    }
   
	}
	/* Closing file */
	vm_file_close(file_handle);
	return MRE_FILE_NOT_WRITTEN;
}

/*****************************************************************************
* FUNCTION
*	mre_write_line_to_file
* DESCRIPTION
*	Writes line to file 
* PARAMETERS
*	file_name just the name as string in root directory, line as string
* RETURNS
*	MRE_FILE_NOT_WRITTEN if fail, MRE_SCILENT for success
TODO: Remove this. Already defined above.
*****************************************************************************/
VMINT mre_write_line_to_file2 (VMSTR file_name, VMSTR line)
{
	/* local variables */
	VMFILE  file_handle;
	VMCHAR  file_path[MRE_FILE_NAME_SIZE];
	VMWCHAR wfile_name[MRE_FILE_NAME_SIZE];
	VMWCHAR wline[MRE_STR_SIZE_MAX + 1];
    VMUINT written;
	VMUINT  bytes_written;
	VMINT line_bytes;
	
	//const VMINT line_size = sizeof (VMCHAR) * (vm_wstrlen (line)+1); //+2 for new line
	//VMINT line_bytes = sizeof(VMCHAR) * (vm_strlen(line) + 1 )
	//VMSTR line_new[1000];
	//sprintf(line_new,"%s\n", line);

	/* Open file */
	sprintf(file_path,"%c:\\%s",mre_get_drv(),file_name);
	vm_ascii_to_ucs2(wfile_name, MRE_STR_SIZE_MAX, file_path);

	vm_ascii_to_ucs2(wline, MRE_STR_SIZE_MAX, line);
	line_bytes = sizeof(VMWCHAR) *(vm_wstrlen (wline)+1); 

	//g_mre_textbox_text =  (sizeof (VMWCHAR) * (vm_wstrlen (wline)+1));
    //vm_wstrcpy (g_mre_textbox_text, text);

	file_handle = vm_file_open(wfile_name, MODE_APPEND, FALSE);
	if (file_handle < 0){
		file_handle = vm_file_open(wfile_name, MODE_CREATE_ALWAYS_WRITE, FALSE);
	}
	if(file_handle < 0)
	{
		vertical_scrolling_text("Write to file fail....");
	}else{
		/* write to file */
		bytes_written = vm_file_write (file_handle, wline, line_bytes, &written);
		vm_log_debug("%d bytes has been written in file", bytes_written);
		
        if (bytes_written == 0)
        {
            vm_file_close (file_handle);
            return MRE_FILE_NOT_WRITTEN;
        }

	    else 
	    {
		    vm_file_close (file_handle);
			//sprintf (show_text, "Written in file: success");
            //mre_show_text (show_text);
		    return MRE_SCILENT;
	    }
   
	}
	/* Closing file */
	vm_file_close(file_handle);
	return MRE_FILE_NOT_WRITTEN;
}

/*****************************************************************************
 * FUNCTION
 *  does_this_wfile_exist
 * DESCRIPTION
 *  Checks if the given filename(in VMWSTR) exists 
 * PARAMETERS
 *  VMWSTR wfilename			  [IN]      filename in VMWSTR
 * RETURNS
 *	VMBOOL TRUE if file exists or FALSE if it doesnt
*****************************************************************************/
VMBOOL does_this_wfile_exist(VMWSTR wfilename){
	VMFILE test_handle;
	
	/* Checking: Test that the file exists, we have access to it */
	test_handle = vm_file_open(wfilename, MODE_READ, FALSE);
	
	if(test_handle < 0)
	{
		// -2 is : VM_FILE_OPEN_ERROR
		printf("File doesnt exist - file handle FAIL!\n");
		return FALSE;
	}else{
		printf("File exists- file handle SUCCESS \n");
		vm_file_close(test_handle); //MUST CLOSE FILE> MUST CLOSE FILE <MUST CLOSE FILE
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  vm_log_file
 * DESCRIPTION
 *  Logs message to pre set log file 
 * PARAMETERS
 *  VMSTR	message		[IN]      log line message in VMSTR
 * RETURNS
 *	VMINT MRE_FILE_NOT_WRITTEN if fail, MRE_SCILENT for success
*****************************************************************************/
VMINT vm_log_file(VMSTR message){
	vm_log_debug(message);
	return mre_write_line_to_file("log.txt", message);
}

/*****************************************************************************
 * FUNCTION
 *  mre_create_file_for_write
 *
 * DESCRIPTION
 *  it create file in which response data has been written 
 *
 * PARAMETERS
 *  VMSTR	filename
 *  VMINT *layer_hdl
 * RETURNS
 *  VOID
*****************************************************************************/
void mre_create_file_for_write(VMSTR filename, VMINT *layer_hdl)
{
	VMFILE  handle;
	VMWCHAR wfile_name[MRE_FILE_NAME_MAX_SIZE ];
	VMCHAR  file_name[2 * MRE_FILE_NAME_MAX_SIZE];

   	sprintf(file_name,"%c:\\%s",mre_get_drv(),filename);
	vm_ascii_to_ucs2(wfile_name, 100, file_name);
	handle = vm_file_open(wfile_name, MODE_CREATE_ALWAYS_WRITE, TRUE);
	if(handle < 0)
	{
		//mre_display_home_top_screen_ascii("File Handle Fail");
		mre_display_home_top_screen_ascii("File Handle Fail", layer_hdl);
	}
	vm_file_close(handle);
}

/*****************************************************************************
 * FUNCTION
 *  mre_dump_to_file
 *
 * DESCRIPTION
 *  Write response data to specified file. Show alert if failed.
 *
 * PARAMETERS
 *  tcp_buffer              [IN]   buffer pointer containing address of received buffer
 *  size                    [IN]   size of received data
 *  VMINT *layer_hdl		[IN]
 *
 * RETURNS
 *  VOID
*****************************************************************************/
void mre_dump_to_file(VMCHAR *tcp_buffer, VMINT size, VMINT *layer_hdl)
{
    VMFILE  handle;
	VMWCHAR wfile_name[MRE_FILE_NAME_MAX_SIZE ];
	VMCHAR  file_name[2 * MRE_FILE_NAME_MAX_SIZE];
	VMUINT length;
	VMINT write_status;

	sprintf(file_name,"%c:\\%s",mre_get_drv(),"TCP.txt");
	vm_ascii_to_ucs2(wfile_name, 100, file_name);
	handle = vm_file_open(wfile_name, MODE_APPEND, TRUE);
	if(handle < 0)
	{
		//mre_display_home_top_screen_ascii("File Handle Fail");
		mre_display_home_top_screen_ascii("File Handle Fail", layer_hdl);
		//vertical_scrolling_text("File Handle Fail....");
	}

	if (handle)
    {
   
	/* log information */
	vm_log_debug("File handle is %d.", handle);
	/* write response to tcp_buffer.txt file */
	write_status =  vm_file_write(handle, tcp_buffer, size, &length);

	if (write_status < 0)
	{
		 vm_log_debug("unable to write");
	}
	/* log information */
    vm_log_debug("%d bytes has been written to file.", length);
    }

	vm_file_close(handle);
}

/*****************************************************************************
*
* #define MODE_READ					1
* #define MODE_WRITE					2
* #define MODE_CREATE_ALWAYS_WRITE	4
* #define MODE_APPEND					8
*****************************************************************************/
VMFILE mre_open_file(VMSTR file_path, VMUINT mode, VMUINT binary){
	VMFILE file_handle;
	VMWSTR wfile_path;
	VMINT wfile_size;

	wfile_size = (strlen(file_path) + 1) * 2;
	wfile_path = vm_malloc(wfile_size);

	vm_ascii_to_ucs2 (wfile_path, MRE_STR_SIZE_MAX, file_path);

	/* Opening the file */
	file_handle = vm_file_open (wfile_path, mode, binary);
    vm_log_debug("file open handle : %d", file_handle);

	return file_handle;
}

/*****************************************************************************
 * FUNCTION DEFINITION
 *  mre_read_file
 * DESCRIPTION
 *  This function reads the file's contents and returns it
 * PARAMETERS
 *  file name				[IN]      name of file
 *	
 * RETURNS
 *	result                  [OUT]     some code for error and success
*****************************************************************************/ 
VMINT mre_read_file(VMSTR file_name, VMCHAR  *data){
	VMFILE  f_read; 
	VMCHAR  f_name[MRE_STR_SIZE_MAX + 1] ;
    VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    //VMCHAR  data[MRE_STR_SIZE_MAX + 1];
	VMCHAR  show_data[MRE_STR_SIZE_MAX + 1 + 10];
	VMWCHAR default_ucs[MRE_STR_SIZE_MAX + 1];
    VMCHAR show_text[MRE_STR_SIZE_MAX + 1];
    VMUINT nread;
    vm_log_debug ("Entering mre_read_file function ");
	sprintf (f_name, "%c:\\%s", mre_get_drv(), file_name);
    vm_log_debug("file name : %s", f_name);

	/* string format conversion */
	vm_ascii_to_ucs2 (f_wname, MRE_STR_SIZE_MAX, f_name);

    /* reading from file */
	f_read = vm_file_open (f_wname, MODE_READ, FALSE);
    vm_log_debug("file read hdl : %d", f_read);
	vm_file_read (f_read, data, MRE_STR_SIZE_MAX,  & nread); 
	data[nread] = '\0';
	vm_file_close (f_read);
    
	if (f_read < 0)
	{
		return MRE_FILE_OPEN_FAILED;
	}
    return MRE_SCILENT;
}

/*****************************************************************************
 * FUNCTION
 *  get_file_size
 * DESCRIPTION
 *  Gets the file specified size. 
 * PARAMETERS
 *	f_name			[IN]	VMSTR		Complete file name of the file	
 * RETURNS
 *	file_size		[OUT]	VMINT		The file's size in int
*****************************************************************************/
VMINT get_file_size(VMSTR f_name){
		VMFILE  file_handle; 
		//VMCHAR  f_name[MRE_STR_SIZE_MAX + 1] ;
		VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
		VMCHAR  show_data[MRE_STR_SIZE_MAX + 1 + 10];
		VMWCHAR default_ucs[MRE_STR_SIZE_MAX + 1];
		VMCHAR show_text[MRE_STR_SIZE_MAX + 1];
		VMUINT nread;
		VMINT file_size;
		vm_log_debug ("Entering get_file_size function ");
		//sprintf (f_name, "%c:\\%s", mre_get_drv(), file_name);
		vm_log_debug("file name : %s", f_name);

		/* string format conversion */
		vm_ascii_to_ucs2 (f_wname, MRE_STR_SIZE_MAX, f_name);

		file_handle = vm_file_open(f_wname, MODE_WRITE, TRUE); //Mode as Binary cuz image, right?
		vm_file_getfilesize(file_handle, &file_size);
		vm_file_close(file_handle); //Don't need the file no more. Closing it.
		return file_size;
}