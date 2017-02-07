/*****************************************************************************
 *
 * Filename:
 * ---------
 *  mre_io.h
 * 
 * Project:
 * --------
 *  UstadMobile
 * 
 * Description:
 * ------------
 *  IO (Filesystem, etc)
 * 
 * Author:
 * -------
 *  Varuna Singh
 * 
 ****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  mre_start_io_menu
 * DESCRIPTION
 *  This function displays menu list on the screen for selection for All tests
 * PARAMETERS
 *  none
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h 
*****************************************************************************/
void mre_start_io_menu(void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_fileiocontext
 * DESCRIPTION
 *  This function sets flag for context of fileio test
 * PARAMETERS
 *  val			         [IN]      it is true or false
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h 
*****************************************************************************/
void mre_set_fileiocontext(VMINT val);

/*****************************************************************************
 * FUNCTION
 *  mre_get_fileiocontext
 * DESCRIPTION
 *  This function gets flag for context of fileio test
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_subcontext    [OUT]      it is true or false
 TODO: Move to MRE_API_Tests.h 
*****************************************************************************/
VMINT mre_get_fileiosubcontext(void);

/*****************************************************************************
 * FUNCTION
 *  mre_textbox_handle
 * DESCRIPTION
 *  This function takes text string and saves it global variable 
 * PARAMETERS
 *  state               [IN]      state of input text box
 *  text				[IN]      Text string in ASCII or GB2312 format
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
void mre_textbox_handle(VMINT state, VMWSTR text);

/*****************************************************************************
 * FUNCTION
 *  mre_io_submenu
 * DESCRIPTION
 *  This function displays sub menu list on the screen for selection 
 * PARAMETERS
 *  none
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
void mre_io_submenu(void);

/*****************************************************************************
 * FUNCTION
 *  mre_file_result
 * DESCRIPTION
 *  This function decides if error found then displays error 
 *  otherwise remain scilent 
 * PARAMETERS
 *  result				[IN]      result
 * RETURNS
 *	none
 DISPlAY 
*****************************************************************************/
void mre_file_result(VMINT result);

/*****************************************************************************
 * FUNCTION
 *  mre_set_subcontext
 * DESCRIPTION
 *  This function sets flag for context 
 * PARAMETERS
 *  val			         [IN]      it is true or false
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_subcontext(VMINT val);

/*****************************************************************************
 * FUNCTION
 *  mre_get_subcontext
 * DESCRIPTION
 *  This function gets flag for context 
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_subcontext    [OUT]      it is true or false
*****************************************************************************/
VMINT mre_get_subcontext(void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_textbox_text
 * DESCRIPTION
 *  This function saves the text of input textbox in a global variable 
 * PARAMETERS
 *  text			         [IN]      text from input textbox
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
void mre_set_textbox_text(VMWSTR text);

/*****************************************************************************
 * FUNCTION
 *  mre_get_textbox_text
 * DESCRIPTION
 *  This function returns pointer to text of input textbox saved in a global 
 *  variable 
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_textbox_text    [OUT]      it is pointer to text stored in global 
 *                                   vaiable
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
VMWSTR mre_get_textbox_text(void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_textbox_state
 * DESCRIPTION
 *  This function saves the state of input textbox in a global variable 
 * PARAMETERS
 *  state			         [IN]      it is ok or cancel
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
void mre_set_textbox_state(VMINT state);

/*****************************************************************************
 * FUNCTION
 *  mre_get_textbox_state
 * DESCRIPTION
 *  This function returns the state of input textbox saved in a global variable 
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_textbox_state    [OUT]      it is ok or cancel
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
VMINT mre_get_textbox_state(void);

/*****************************************************************************
 * FUNCTION
 *  mre_get_drv
 * DESCRIPTION
 *  This function returns a no corresponding to a drive found
 * PARAMETERS
 *  none
 * RETURNS
 *  g_mre_drv            [OUT]      drive found on device
*****************************************************************************/
VMINT mre_get_drv(void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_drv
 * DESCRIPTION
 *  This function stores a no corresponding to a drive found
 * PARAMETERS
 *  none
 * RETURNS
 *  none
*****************************************************************************/
void mre_set_drv(void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_global_data
 * DESCRIPTION
 *  This function sets global data variables
 *  handling actions
 * PARAMETERS
 *  none
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h
*****************************************************************************/
void mre_set_global_data (void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_curr_x
 * DESCRIPTION
 *  This function sets current x cordinate to write strings on screen
 * PARAMETERS
 *  x			         [IN]      current x cordinate
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_curr_x (VMINT x);

/*****************************************************************************
 * FUNCTION
 *  mre_get_curr_x
 * DESCRIPTION
 *  This function gets current x cordinate to write strings on screen
 * PARAMETERS
 *  none
 * RETURNS
 *  g_mre_curr_x        [OUT]      current x cordinate
*****************************************************************************/
VMINT mre_get_curr_x (void);

/*****************************************************************************
 * FUNCTION
 *  mre_set_curr_y
 * DESCRIPTION
 *  This function sets current y cordinate to write strings on screen
 * PARAMETERS
 *  y			         [IN]      current y cordinate
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_curr_y (VMINT y);

/*****************************************************************************
 * FUNCTION
 *  mre_get_curr_y
 * DESCRIPTION
 *  This function gets current y cordinate to write strings on screen
 * PARAMETERS
 *  none
 * RETURNS
 *  g_mre_curr_y         [OUT]      current y cordinate
*****************************************************************************/
VMINT mre_get_curr_y (void);

/*****************************************************************************
 * FUNCTION
 *  mre_show_input_textbox
 * DESCRIPTION
 *  This function displays input textbox
 * PARAMETERS
 *  input_type			  [IN]      type of input textbox
 * RETURNS
 *	none
*****************************************************************************/
VMINT mre_show_input_textbox(VMINT input_type);


