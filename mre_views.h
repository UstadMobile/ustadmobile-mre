/*
mre_views.h The logic for the views related to MRE using Nuklear
*/


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
*****************************************************************************/
void mre_set_graphic_global_data(void);

/* Initiates Nuklear GUI tests */
void initiate_nk_gui();

/* Create component */ //TODO: Check this.
struct mre_nk_c * mre_nk_c_create(char *type, char *title, int len, int hovering, char *url, char *thumb);

/* Updates, refreshes view. Clears screen, initialises view components and calls 
for actual drawing */ 
void update_gui();

/* Initialises view components for next refresh */ 
void declare_view(struct mre_nk_c **ptr_cmpts); //TODO: Changed from **ptr_cmpts to *ptr_cmpts. Check this.

/* Testing: Set View to Test1 */
void set_test_view();

/* Testing: Set View to Test1 from nuklear_mre file*/
void set_test_view_here();

/* Testing: Set View to Test2 */ 
void set_test2_view();

/* Go to next page */
void goto_next_page();

/* Go to previous page */
void goto_previous_page();

/* MRE Show Image */
void mre_show_image(VMINT size_ctrl, VMWSTR f_wname, VMSTR file_name, VMINT *mre_layer_hdl, short x, short y);

/* Testing: Set View to Test1 */
void set_custom_view();
	
/* Sets Login View Components */
void set_login_view();

/* Adds View to History */
void add_view_to_history();