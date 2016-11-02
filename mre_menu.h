/* 
mre_menu.h Menu specific functions go here. 
	Are these views ? In a testing sense, yes. 
	But are they nuklear GUI interactive views? No. 
	Which is why I've made this for now..
*/

//#include "share.h"

/* Input Types */ 
typedef enum mre_iput_type_textbox
{
    INPUT_TYPE1, 
    INPUT_TYPE2, 
    INPUT_TYPE3,
    TOTAL_INPUT_TYPE 
}mre_iput_type_textbox;

///////////////////////////MENU CONTEXT FLAGS/////////////////////////////////

/*****************************************************************************
 * FUNCTION
 *  mre_set_httpcontext
 * DESCRIPTION
 *  This function sets flag for context for http test
 * PARAMETERS
 *  val			         [IN]      it is true or false
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_httpcontext (VMINT val)
{
    g_mre_httpcontext = val;
}

void mre_set_socketcontext (VMINT val)
{
	g_mre_socketcontext = val;
}

/*****************************************************************************
 * FUNCTION
 *  mre_get_httpcontext
 * DESCRIPTION
 *  This function gets flag for context for http test
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_httpcontext    [OUT]      it is true or false
*****************************************************************************/

VMINT mre_get_httpcontext (void)
{
    return g_mre_httpcontext;
}

/*****************************************************************************
 * FUNCTION
 *  mre_get_socketcontext
 * DESCRIPTION
 *  This function gets flag for context for socket test
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_httpcontext    [OUT]      it is true or false
*****************************************************************************/
VMINT mre_get_socketcontext (void)
{
	return g_mre_socketcontext;
}

/*****************************************************************************
 * FUNCTION
 *  mre_set_fileiocontext
 * DESCRIPTION
 *  This function sets flag for context 
 * PARAMETERS
 *  val			         [IN]      it is true or false
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_fileiocontext (VMINT val)
{
    g_mre_fileiocontext = val;
}
/*****************************************************************************
 * FUNCTION
 *  mre_get_fileiocontext
 * DESCRIPTION
 *  This function gets flag for context 
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_subcontext    [OUT]      it is true or false
*****************************************************************************/

VMINT mre_get_fileiocontext (void)
{
    return g_mre_fileiocontext;
}

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
void mre_set_subcontext (VMINT val)
{
    g_mre_subcontext = val;
}

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

VMINT mre_get_subcontext (void)
{
    return g_mre_subcontext;
}

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
*****************************************************************************/
void mre_textbox_handle (VMINT state, VMWSTR text)
{
    VMINT file_result;
    vm_log_debug ("Entering mre_textbox_handle function ");
    mre_set_textbox_state (state);
	if (state == VM_INPUT_OK)
    {
        mre_set_textbox_text (text);
        file_result = mre_write_file ("file1.txt");
        mre_file_result (file_result);
    }
    else if (state == VM_INPUT_CANCEL)
    {
		mre_start_menu ();
        mre_file_result (MRE_TEXTBOX_CANCEL); 
    }
    else
    {
        mre_file_result (MRE_TEXT_BOX_FAILED);
    }
    vm_log_debug ("Exiting mre_textbox_handle function ");
}

//////////////////////////////MENU VIEWS//////////////////////////////////////

/*****************************************************************************
 * FUNCTION
 *  mre_start_menu
 * DESCRIPTION
 *  This function displays menu list on the screen for Tests selection 
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void mre_start_menu (void)
{
    /* it displays menu item list on the scrren */
	VMWCHAR s[MRE_STR_SIZE_MAX];    
	VMUINT8 *buf;	
	int w;

	/* log information*/
	vm_log_debug ("Entering mre_start_menu function");

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (layer_hdl[0]);
	
	/* fill the screen*/
	vm_graphic_fill_rect (buf, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
		                 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* converts string into usc2 format to display on the screen */
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, "Test Menu:");
	w = vm_graphic_get_string_width (s);

	/* intialize location of string menu item*/
    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);

	/* draw text */
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 1 key */
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "1: File I/O Tests..");
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 2 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "2: HTTP Tests");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 3 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "3: Play a Video	");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 4 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "4: Test POST");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);
	
	/* NUM 5 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "5: Socket Tests");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 6 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "6: Play Video (Native)");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 7 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "7: Nuklear Test");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

    
	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer (layer_hdl, 1);
	vm_log_debug ("Exiting mre_start_menu function");
}

/*****************************************************************************
 * FUNCTION
 *  mre_start_http_menu
 * DESCRIPTION
 *  This function displayes menu having http connection option
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void mre_start_http_menu(void)
{

	/* it displays menu item list on the scrren */
	VMWCHAR s[MRE_STR_SIZE_MAX];    
	VMUINT8 *buf;	
	int w;

	/* log information*/
	vm_log_debug ("Entering mre_start_io_menu function");

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (layer_hdl[0]);
	
	/* fill the screen*/
	vm_graphic_fill_rect (buf, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
		                 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* converts string into usc2 format to display on the screen */
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, "Menu:");
	w = vm_graphic_get_string_width (s);

	/* intialize location of string menu item*/
    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);

	/* draw text */
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 1 key */
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "1: Start HTTP Tests");
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 0 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "0: Back To Main Menu");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);
	
	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer (layer_hdl, 1);
	vm_log_debug ("Exiting mre_start_menu function");

}

/*****************************************************************************
 * FUNCTION
 *  mre_start_socket_menu
 * DESCRIPTION
 *  This function displayes menu having socket connection option
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void mre_start_socket_menu(void){
	//"Select connection types" ,"1.VM_TCP_APN_CMNET","2.VM_TCP_APN_CMWAP"
	/* it displays menu item list on the scrren */
	VMWCHAR s[MRE_STR_SIZE_MAX];    
	VMUINT8 *buf;	
	int w;

	/* log information*/
	vm_log_file ("Entering mre_start_socket_menu function\n");

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (layer_hdl[0]);
	
	/* fill the screen*/
	vm_graphic_fill_rect (buf, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
		                 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* converts string into usc2 format to display on the screen */
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, "Select Connection types:");
	w = vm_graphic_get_string_width (s);

	/* intialize location of string menu item*/
    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);

	/* draw text */
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 1 key */
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "1: VM_TCP_APN_CMNET");
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 2 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "2: VM_TCP_APN_CMWAP");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);


	/* NUM 0 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "0: Back To Main Menu");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);
	
	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer (layer_hdl, 1);
	vm_log_debug ("Exiting mre_start_menu function");

}

/*****************************************************************************
 * FUNCTION
 *  mre_start_io_menu
 * DESCRIPTION
 *  This function displays menu list on the screen for selection 
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void mre_start_io_menu (void)
{
    /* it displays menu item list on the scrren */
	VMWCHAR s[MRE_STR_SIZE_MAX];    
	VMUINT8 *buf;	
	int w;

	/* log information*/
	vm_log_debug ("Entering mre_start_io_menu function");

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (layer_hdl[0]);
	
	/* fill the screen*/
	vm_graphic_fill_rect (buf, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
		                 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* converts string into usc2 format to display on the screen */
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, "Menu:");
	w = vm_graphic_get_string_width (s);

	/* intialize location of string menu item*/
    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);

	/* draw text */
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 1 key */
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "1: Create a file");
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 2 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "2: Write in File");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

    /* NUM 3 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "3: Read content of file");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

    /* NUM 4 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);

	/* NUM 0 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "0: Back To Main Menu");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);
	
	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer (layer_hdl, 1);
	vm_log_debug ("Exiting mre_start_menu function");
}


/*****************************************************************************
 * FUNCTION
 *  mre_io_submenu
 * DESCRIPTION
 *  This function displays sub menu list on the screen for selection for File 
 *   IO Tests
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/

void mre_io_submenu (void)
{
    /* it displays menu item list on the scrren */
	VMWCHAR s[MRE_STR_SIZE_MAX];    
	VMUINT8 *buf;	
	int w; 

	/* log information*/
	vm_log_debug ("Entering mre_start_menu function");

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (layer_hdl[0]);
	
	/* fill the screen*/
	vm_graphic_fill_rect (buf, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
		                 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* converts string into usc2 format to display on the screen */
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, "Menu:");
	w = vm_graphic_get_string_width (s);

	/* intialize location of string menu item*/
    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);

	/* draw text */
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 1 key */
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "1: Input textbox type1");
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* NUM 2 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "2: Input textbox type2");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

    /* NUM 3 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "3: Input textbox type3");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	
	/* NUM 0 key..*/
	memset (s, MRE_SET_VAL, MRE_STR_SIZE_MAX);
	mre_set_curr_y (mre_get_curr_y () + vm_graphic_get_character_height () + MRE_SPACE_BTW_LN);
	/* converts string into usc2 format to display on the screen */
	vm_ascii_to_ucs2 (s, MRE_STR_SIZE_MAX, "0: Back To File IO Test Menu");	
	vm_graphic_textout (buf, mre_get_curr_x (), mre_get_curr_y (), s, wstrlen (s), VM_COLOR_WHITE);

	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer (layer_hdl, 1);
	vm_log_debug ("Exiting mre_start_menu function");
}

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
VMINT mre_show_input_textbox(VMINT input_type)
{
    VMINT textbox_result;
	VMWCHAR default_ucs[MRE_DEF_STRING_SIZE];

	vm_ascii_to_ucs2(default_ucs, (MRE_DEF_STRING_SIZE)*2 , "Write some text"); 
    switch (input_type)
    {
        case INPUT_TYPE1:
            vm_log_debug("vm_input_text function with type1 begins");
            textbox_result = vm_input_text(mre_textbox_handle);
            vm_log_debug("vm_input_text function with type1 ends textbox result : %d", textbox_result);
            break;

        case INPUT_TYPE2:
            vm_log_debug("vm_input_text function with type2 begins");
            textbox_result = vm_input_text2(default_ucs, VM_INPUT_METHOD_TEXT ,
                                            mre_textbox_handle);
            vm_log_debug("vm_input_text2 function with type1 ends textbox result : %d", textbox_result);
            break;

        case INPUT_TYPE3:
            vm_log_debug("vm_input_text function with type3 begins");
            textbox_result = vm_input_text3(default_ucs, MRE_STR_SIZE_MAX, 
                                              VM_INPUT_METHOD_ALPHABETIC , mre_textbox_handle);
            vm_log_debug("vm_input_text3 function with type1 ends textbox result : %d", textbox_result);
            break;

        default:
            return MRE_ERROR;   
    }
    return MRE_SCILENT;

}

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
 TODO: Maybe this goes in io.h ?
*****************************************************************************/
void mre_set_global_data (void)
{
    vm_log_debug  ("mre_set_global_data function starts ");


    mre_set_curr_x (MRE_SET_X);
    mre_set_curr_y (MRE_SET_Y);
    mre_set_subcontext (FALSE);
    mre_set_textbox_state (MRE_SET_VAL);
    mre_set_textbox_text (NULL);
    mre_set_drv ();

    vm_log_debug  ("mre_set_global_data function exits ");
}

