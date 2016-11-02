/*
mre_display.c Display stuff, popups, alerts, updates etc Source
*/ 

#include "share.h"
#include "mre_display.h"

/*****************************************************************************
 * FUNCTION
 * mre_display_home_top_screen_ascii Definition
 *
 * DESCRIPTION
 * Displays home screen,here string passed is in ascii format.
 *
 * PARAMETERS
 * text             [IN] string to be displayed in on screen
 * layer_hdl		The layer? (VMINT	layer_hdl[2])
 * 
 * RETURNS
 * VOID
*****************************************************************************/
void mre_display_home_top_screen_ascii(VMCHAR *text, VMINT *layer_hdl)
{
	VMWCHAR string[MRE_DEFAULT_STRING_SIZE];
	VMUINT8* buffer;	
	int x_coordinate, y_coordinate, width;

	/* function body */
	vm_gb2312_to_ucs2(string, MRE_DEFAULT_STRING_SIZE, text);
	/*Width of character string, in elemental area*/
	width = vm_graphic_get_string_width(string);
	/*x coordinate of screen where string is to be displayed*/
	x_coordinate = (vm_graphic_get_screen_width() - width) / 2;
	/*y coordinate of screen where string is to be displayed*/
	y_coordinate = (vm_graphic_get_screen_height() - vm_graphic_get_character_height()) / 2;
		vm_graphic_clear_layer_bg(layer_hdl[0]);
	/* get the target buffer*/
	buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);		
	/* fill the screen*/
	vm_graphic_fill_rect(buffer, MRE_SCREEN_START_X, MRE_SCREEN_START_Y, vm_graphic_get_screen_width(), 
		                 vm_graphic_get_screen_height(), VM_COLOR_BLACK, VM_COLOR_BLACK);
	/* draw text */	
	vm_graphic_textout(buffer,x_coordinate, y_coordinate, string, wstrlen(string), VM_COLOR_WHITE);
	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer(layer_hdl, MRE_SINGLE_LAYER);
}

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
*****************************************************************************/
void mre_text_box_display_result (VMINT state, VMWSTR text)
{
    /* it is used to display content of a file */
}

/*****************************************************************************
 * FUNCTION
 *  mre_display_file_contents
 * DESCRIPTION
 *  This function displays file's content on display text box 
 *  which is actually input textbox 
 * PARAMETERS
 *  file name				[IN]      name of file
 * RETURNS
 *	result                  [OUT]     some code for error and success 
 TODO: Modulate this accordingly 
*****************************************************************************/
VMINT mre_display_file_contents (VMSTR file_name)
{
    VMCHAR  data[MRE_STR_SIZE_MAX + 1];
	VMCHAR  show_data[MRE_STR_SIZE_MAX + 1 + 10];
	VMWCHAR default_ucs[MRE_STR_SIZE_MAX + 1];
    VMCHAR show_text[MRE_STR_SIZE_MAX + 1];
	VMINT read_file_result;

	read_file_result = mre_read_file(file_name, data);
	if (read_file_result < 0 ){
		return read_file_result;
	}
  
	/* Displaying data in textbox editor*/
	strcpy(show_data, "File Content:");
	strcat(show_data, data);
	vm_ascii_to_ucs2(default_ucs, (MRE_STR_SIZE_MAX + 1)*2 , show_data); 

    vm_input_text3 (default_ucs, MRE_STR_SIZE_MAX, VM_INPUT_METHOD_ALPHABETIC,
                                     mre_text_box_display_result);
    vm_log_debug ("Entered  mre_display_file_contents and  vm_input_text3 function ends ");
    
	sprintf (show_text, "file content displayed : %s", file_name);
    mre_show_text (show_text);
    vm_log_debug ("Exiting mre_display_file_contents function ");
    return MRE_SCILENT;
        
}


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
void mre_file_result (VMINT result)
{
    vm_log_debug ("Entering mre_file_result function");
    switch (result)
    {
 
		case MRE_FILE_CREATE_FAILED:
			mre_show_text ("file is not created");
			break;

		case MRE_FILE_OPEN_FAILED:
			mre_show_text ("file can not be opened");
			break;

		case MRE_TEXT_BOX_FAILED:
			mre_show_text ("input textbox can not be displayed");
			break;

		case MRE_FILE_NOT_WRITTEN:
			mre_show_text ("file is not written");
			break;

		case MRE_ERROR:
			mre_show_text ("some fatal error");
			break;
		case MRE_TEXTBOX_CANCEL:
			mre_show_text ("text box canceled");
			break;

		case MRE_GET_FILE_TIME_FAILED:
			mre_show_text ("can not get time for file");

		case MRE_SCILENT:
			break;

		default:
			mre_show_text ("unknown error");
			break;
		}
    vm_log_debug ("Exiting mre_file_result function");
}

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
void mre_show_text_layer (VMSTR text, VMINT *handle_layer)
{
	
	VMWCHAR s[MRE_STR_SIZE_MAX];
	VMUINT8 *buf;

	/*log information*/
	vm_log_debug ("Entering mre_show_text_layer function ,input parameter is :%s",text);

    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, text);

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (handle_layer[0]);

	/* fill the screen*/
	vm_graphic_fill_rect (buf, 0, vm_graphic_get_screen_height () / 2, vm_graphic_get_screen_width (),
        (vm_graphic_get_screen_height () / 2) - MRE_SET_MARGIN, VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* draw text */
  	vm_graphic_textout (buf, (MRE_SET_MARGIN * 2), (vm_graphic_get_screen_height () / 2) + MRE_SET_MARGIN, s, wstrlen (s), VM_COLOR_WHITE);
	vm_graphic_flush_layer (handle_layer, 1);
	vm_log_debug ("Exiting mre_show_text_layer function");
}

/*****************************************************************************
 * FUNCTION DEFINITION
 *  mre_show_text
 * DESCRIPTION
 *  This function takes text string and displays it on the screen 
 * PARAMETERS
 *  text				[IN]      Text string in ASCII or GB2312 format
 * RETURNS
 *	none
*****************************************************************************/

void mre_show_text (VMSTR text)
{
	
	VMWCHAR s[MRE_STR_SIZE_MAX];
	VMUINT8 *buf;

	/*log information*/
	vm_log_debug ("Entering mre_show_text function ,input parameter is :%s",text);

    mre_set_curr_x (MRE_SET_X + MRE_SET_MARGIN);
    mre_set_curr_y (MRE_SET_Y + MRE_SET_MARGIN);
	vm_gb2312_to_ucs2 (s, MRE_STR_SIZE_MAX, text);

	/* get the target buffer*/
	buf = vm_graphic_get_layer_buffer (layer_hdl[0]);

	/* fill the screen*/
	vm_graphic_fill_rect (buf, 0, vm_graphic_get_screen_height () / 2, vm_graphic_get_screen_width (),
        (vm_graphic_get_screen_height () / 2) - MRE_SET_MARGIN, VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* draw text */
  	vm_graphic_textout (buf, (MRE_SET_MARGIN * 2), (vm_graphic_get_screen_height () / 2) + MRE_SET_MARGIN, s, wstrlen (s), VM_COLOR_WHITE);
	vm_graphic_flush_layer (layer_hdl, 1);
	vm_log_debug ("Exiting mre_show_text function");
}

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
void vertical_scrolling_text(VMSTR ascii_string)
{
	/* local variables */
	int max_height; 
	unsigned int i =  MRE_INTIAL_VALUE;
	static int last_y = MRE_INTIAL_VALUE;

	max_height = vm_graphic_get_screen_height();
	for (i = MRE_INTIAL_VALUE; i < strlen (ascii_string); i++)
	{
		int width, height;
		vm_graphic_measure_character (ascii_string[i], &width, &height);
		if(height > max_height)
		{
			max_height = height;
		}
	}

	if ((last_y + max_height) > vm_graphic_get_screen_height() || last_y == MRE_INTIAL_VALUE)
	{
		last_y = MRE_INTIAL_VALUE;
		vm_graphic_fill_rect(vm_graphic_get_layer_buffer(layer_handle[0]), MRE_SCREEN_START_X, MRE_SCREEN_START_Y, 
			vm_graphic_get_screen_width(), vm_graphic_get_screen_height(), VM_COLOR_BLACK, VM_COLOR_BLACK);
	}
	mre_show_text_coordinates(MRE_SCREEN_START_X, last_y,  ascii_string);
	last_y += max_height;
}

/*****************************************************************************
 * FUNCTION DEFINITION
 *  mre_show_text_coordinates
 * DESCRIPTION
 *  This function displayes ascii string on screen with x, y coordinates
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
static void mre_show_text_coordinates(int display_string_x_pos, 
									int display_string_y_pos, VMSTR ascii_string)
{
	/* local variables */
	VMWSTR display_string, exit_str;
	VMINT size;

	/* function body */
	size = (strlen(ascii_string) + 1) * 2;
	display_string = vm_malloc(size);
    vm_graphic_clear_layer_bg (layer_handle[0]);
	vm_ascii_to_ucs2(display_string, size,  ascii_string);
	/* draw text */
	vm_graphic_textout(vm_graphic_get_layer_buffer(layer_handle[0]), display_string_x_pos, display_string_y_pos, display_string, wstrlen(display_string), VM_COLOR_WHITE);
	/* display exit */
	display_string_x_pos = vm_graphic_get_screen_width() - 3 * MRE_MARGIN; 
	display_string_y_pos = vm_graphic_get_screen_height() - MRE_MARGIN; 
	size = (strlen("Exit") + 1) * 2;
	exit_str = vm_calloc(size);
	vm_ascii_to_ucs2(exit_str, size,  "Exit");
	vm_graphic_textout(vm_graphic_get_layer_buffer(layer_handle[0]), display_string_x_pos, display_string_y_pos, exit_str, wstrlen(exit_str), VM_COLOR_WHITE);

	/* flush the screen with data in the buffer*/
	vm_graphic_flush_layer(layer_handle, MRE_FIRST_LAYER);
	vm_free(display_string);
}

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
void mre_set_textbox_text (VMWSTR text)
{
    if (text != NULL)
    {
        g_mre_textbox_text = (VMWSTR)vm_malloc (sizeof (VMWCHAR) * (vm_wstrlen (text)+1));
        vm_wstrcpy (g_mre_textbox_text, text);
    }
    else
    {
        g_mre_textbox_text = NULL;
    }

}
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

VMWSTR mre_get_textbox_text (void)
{
    return g_mre_textbox_text;
}


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
void mre_set_textbox_state (VMINT state)
{
    g_mre_textbox_state = state;
}

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
VMINT mre_get_textbox_state (void)
{
    return g_mre_textbox_state;
}


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
void mre_set_curr_x (VMINT x)
{
    g_mre_curr_x = x;
}

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
VMINT mre_get_curr_x (void)
{
    return g_mre_curr_x;
}

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
void mre_set_curr_y (VMINT y)
{
    g_mre_curr_y = y;
}

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
VMINT mre_get_curr_y (void)
{
    return g_mre_curr_y;
}