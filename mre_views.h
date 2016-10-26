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

/* Create component */ 
struct mre_nk_c * mre_nk_c_create(char *type, char *title, int len, int hovering);

/* Updates, refreshes view. Clears screen, initialises view components and calls 
for actual drawing */ 
void update_gui();

/* Initialises view components for next refresh */ 
void declare_view(struct mre_nk_c **ptr_cmpts);

/* Testing: Set View to Test1 */
void set_test_view();

/* Testing: Set View to Test2 */ 
void set_test2_view();

/* Go to next page */
void goto_next_page();

/* Go to previous page */
void goto_previous_page();

/* Mre show image */
void mre_show_image();