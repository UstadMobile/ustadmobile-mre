/*
mre_views.c The logic for the views related to MRE using Nuklear 
*/

#include "share.h"
#include "mre_views.h"

/*****************************************************************************
 * FUNCTION
 *  mre_set_graphic_global_data
 * DESCRIPTION
 *  This function sets global data variables
 *  handling actions
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_graphic_global_data(void)
{
	/* function body */
    vm_log_file("mre_set_graphic_global_data function starts..\n");
    mre_set_curr_x(MRE_SET_X);
    mre_set_curr_y(MRE_SET_Y);
    vm_font_set_font_size(VM_SMALL_FONT);
    /*log information*/
    vm_log_file("mre_set_global_data done.\n");
}

/* Go to next page placeholder */
void goto_next_page(){

}

/* Go to previous page placeholder */
void goto_previous_page(){

}

/* mre_show_image placeholder */
void mre_show_image(){
	//Testing
}