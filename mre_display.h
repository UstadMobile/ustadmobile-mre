/* 
	mre_display.h : Display alerts, updates, popups, etc 
*/

/*****************************************************************************
 * FUNCTION
 *  mre_read_file_display
 * DESCRIPTION
 *  This function displays file's content on display text box 
 *  which is actually input textbox 
 * PARAMETERS
 *  file name				[IN]      name of file
 * RETURNS
 *	result                  [OUT]     some code for error and success 
*****************************************************************************/
VMINT mre_display_file_contents(VMSTR file_name);

/*****************************************************************************
 * FUNCTION
 *  mre_text_box_display_result
 * DESCRIPTION
 *  This function takes text string and displays it on the screen 
 * PARAMETERS
 *  state               [IN]      state of input text box
 *  text				[IN]      Text string in ASCII or GB2312 format
 * RETURNS
 *	none
 TODO: VIEWS
*****************************************************************************/
void mre_text_box_display_result(VMINT state, VMWSTR text);

/*****************************************************************************
 * FUNCTION
 *  mre_file_result
 * DESCRIPTION
 *  This function decides if file type error found then displays error 
 *  otherwise remain scilent 
 * PARAMETERS
 *  result				[IN]      result
 * RETURNS
 *	none
*****************************************************************************/
void mre_file_result (VMINT result);

/*****************************************************************************
 * FUNCTION DECLARATION
 *  mre_show_text
 * DESCRIPTION
 *  This function takes text string and displays it on the screen 
 * PARAMETERS
 *  text				[IN]      Text string in ASCII or GB2312 format
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h
 he
*****************************************************************************/
void mre_show_text(VMSTR text);

/*****************************************************************************
 * FUNCTION
 *  mre_show_text_layer
 * DESCRIPTION
 *  This function takes text string and displays it on the screen 
 * PARAMETERS
 *  text				[IN]      Text string in ASCII or GB2312 format
 * RETURNS
 *	none
*****************************************************************************/
void mre_show_text_layer (VMSTR text, VMINT *handle_layer);

/*****************************************************************************
 * FUNCTION
 *  mre_set_textbox_text
 * DESCRIPTION
 *  This function saves the text of input textbox in a global variable 
 * PARAMETERS
 *  text			         [IN]      text from input textbox
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_textbox_text (VMWSTR text);

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
*****************************************************************************/
VMWSTR mre_get_textbox_text (void);


/*****************************************************************************
 * FUNCTION
 *  mre_set_textbox_state
 * DESCRIPTION
 *  This function saves the state of input textbox in a global variable 
 * PARAMETERS
 *  state			         [IN]      it is ok or cancel
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_textbox_state (VMINT state);

/*****************************************************************************
 * FUNCTION
 *  mre_get_textbox_state
 * DESCRIPTION
 *  This function returns the state of input textbox saved in a global variable 
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_textbox_state    [OUT]      it is ok or cancel
*****************************************************************************/
VMINT mre_get_textbox_state (void);


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
 *  draw_http_result_string
 * DESCRIPTION
 *  This function displayes string as per coordinates on screen
 * PARAMETERS
 *  x
 *  y
 *	ascii_string 
 * RETURNS
 *	none
*****************************************************************************/
void mre_show_text_coordinates(int x, int y, VMSTR ascii_string);

/*****************************************************************************
 * FUNCTION
 *  vertical_scrolling_text
 * DESCRIPTION
 *  This function displays results of http conne
 * PARAMETERS
 *  ascii_string      [IN]           - contains display string
 * RETURNS
 *	none
*****************************************************************************/
void vertical_scrolling_text(VMSTR ascii_string);