//#include "share.h"

/*****************************************************************************
 * FUNCTION
 *  mre_set_httpcontext
 * DESCRIPTION
 *  This function sets flag for context of http test
 * PARAMETERS
 *  val			         [IN]      it is true or false
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_httpcontext(VMINT val);

/*****************************************************************************
 * FUNCTION
 *  mre_set_socketcontext
 * DESCRIPTION
 *  This function sets flag for context of socket test
 * PARAMETERS
 *  val			         [IN]      it is true or false
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_socketcontext(VMINT val);

/*****************************************************************************
 * FUNCTION
 *  mre_get_httpcontext
 * DESCRIPTION
 *  This function gets flag for context of http test
 * PARAMETERS
 *  none
 * RETURNS
 *	g_mre_httpsubcontext    [OUT]      it is true or false
*****************************************************************************/
VMINT mre_get_httpcontext(void);


/*****************************************************************************
 * FUNCTION
 *  handle_sysevt
 * DESCRIPTION
 *  This function handles system events
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void handle_sysevt(VMINT message, VMINT param);
/*****************************************************************************
 * FUNCTION
 *  handle_keyevt
 * DESCRIPTION
 *  This function takes control when key is pressed.
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void handle_keyevt(VMINT event, VMINT keycode);
/*****************************************************************************
 * FUNCTION
 *  handle_penevt
 * DESCRIPTION
 *  This function takes control when screen is touched against pen
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void handle_penevt(VMINT event, VMINT x, VMINT y);
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
void mre_start_http_menu(void);

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
void mre_start_socket_menu(void);

/*****************************************************************************
 * FUNCTION
 *  draw_http_result_string
 * DESCRIPTION
 *  This function displayes state of http connection and response head, body on screen
 * PARAMETERS
 *  none
 * RETURNS
 *	none

 Replaced by mre_show_text_coordinates(int x, int y, VMSTR ascii_string);
*****************************************************************************/
//static void draw_http_result_string(int x, int y, VMSTR ascii_string);

/*****************************************************************************
 * FUNCTION
 *  connect_http
 * DESCRIPTION
 *  This function connects http_connection
 * PARAMETERS
 *  none

 * RETURNS
 *	none
*****************************************************************************/
void connect_http(void);

/*****************************************************************************
 * FUNCTION
 *  http_state_notify_cb
 * DESCRIPTION
 *  This function will receive various http connection state after connection 
 *  has been established
 * PARAMETERS
 *  state          [IN]       state of http connection
 *  param          [IN]       
 *  session        [IN]       structure variable containing http_session other parameters
 * RETURNS
 *	none
*****************************************************************************/
static void http_state_notify_cb(VMINT state, VMINT param, void* session);
/*****************************************************************************
 * FUNCTION
 *  http_hook_cb
 * DESCRIPTION
 *  This function will receive response of http connection as a structure
 * PARAMETERS
 *  bResponse      [IN]       to ensure response has been arrived
 *  session        [IN]       structure variable containing http_session other parameters
 * RETURNS
 *	none This HAS TO BE VOID for it to work and get the session properly!
*****************************************************************************/
static void http_hook_cb(VMINT bResponse, void* pSession);

/*****************************************************************************
 * FUNCTION
 *  http_request_hook_cb
 * DESCRIPTION
 *  This function will receive response of make_http_request connection as a structure
 * PARAMETERS
 *  bResponse      [IN]       to ensure response has been arrived
 *  session        [IN]       structure variable containing http_session other parameters
 * RETURNS
 *	none This HAS TO BE VOID for it to work and get the session properly!
*****************************************************************************/
static void http_request_hook_cb(VMINT bResponse, void* pSession);

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
//void vertical_scrolling_text(VMSTR ascii_string);

void make_http_request(VMSTR method, VMSTR url, http_head_t headers[], int headers_count, http_head_t post_parameters[], int post_parameters_count);

void um_login_controller(VMSTR username, VMSTR password);