/*
mre_file.h All file realted stuff's header file
*/

//#include "share.h"

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
VMINT mre_create_file(VMSTR file_name);

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
VMINT mre_write_file(VMSTR file_name);

/*****************************************************************************
 * FUNCTION
 *  mre_check_modify_time
 * DESCRIPTION
 *  This function displays file's modified time 
 * PARAMETERS
 *  file name				[IN]      name of file
 * RETURNS
 *	result                  [OUT]     some code for error and success 
*****************************************************************************/
VMINT mre_check_modify_time(VMSTR file_name);

/*****************************************************************************
 * FUNCTION
 *  mre_write_line_to_file
 * DESCRIPTION
 *  This function writes a VMSTR line to a file 
 * PARAMETERS
 *  VMSTR file_name			[IN]		the file name in VMSTR
 *	VMSTR line				[IN]		the line to be written in VMSTR
 * RETURNS
 *	VMINT	MRE_SCILENT if success; MRE_FILE_NOT_WRITTEN if  not
*****************************************************************************/
VMINT mre_write_line_to_file(VMSTR file_name, VMSTR line);

/*****************************************************************************
 * FUNCTION
 *  vm_log_file
 * DESCRIPTION
 *  This function logs a message to a file
 * PARAMETERS
 *  VMSTR message			[IN]		the message log line
 * RETURNS
 *	VMINT	MRE_SCILENT if success; MRE_FILE_NOT_WRITTEN if  not
*****************************************************************************/
VMINT vm_log_file(VMSTR message);

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
VMBOOL does_this_file_exist(VMSTR filename);

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
VMBOOL does_this_wfile_exist(VMWSTR wfilename);

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
void mre_create_file_for_write(VMSTR filename, VMINT *layer_hdl);

/*****************************************************************************
 * FUNCTION
 *  mre_dump_to_file Declaration
 *
 * DESCRIPTION
 *  Write response data to specified file
 *
 * PARAMETERS
 *  tcp_buffer              [IN]   buffer pointer containing address of received buffer
 *  size                    [IN]   size of received data
 *  layer_hdl				[IN]	VMINT *layer_hdl
 *
 * RETURNS
 *  VOID
*****************************************************************************/
void mre_dump_to_file(VMCHAR *tcp_buffer, VMINT size, VMINT *layer_hdl);

/*****************************************************************************
 * FUNCTION DECLARATION
 *  mre_read_file
 * DESCRIPTION
 *  This function reads the file's contents and returns it
 * PARAMETERS
 *  file name				[IN]      name of file
 *	
 * RETURNS
 *	result                  [OUT]     some code for error and success
*****************************************************************************/ 
VMINT mre_read_file(VMSTR file_name, VMCHAR *data);