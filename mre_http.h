/*

	MRE HTTP : Header File - mre_http.h
	The MRE Http file does HTTP network stuff.

	Author: Varuna Singh, UstadMobile

*/ 

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
 *  session        [IN]       structure variable containing http_session 
							  other parameters
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
 *  session        [IN]       structure variable containing http_session 
 *							  other parameters
 * RETURNS
 *	none This HAS TO BE VOID for it to work and get the session properly!
*****************************************************************************/
static void http_hook_cb(VMINT bResponse, void* pSession);

/*****************************************************************************
 * FUNCTION
 *  http_request_hook_cb
 * DESCRIPTION
 *  This function will receive response of make_http_request connection as a 
 *	structure
 * PARAMETERS
 *  bResponse      [IN]       to ensure response has been arrived
 *  session        [IN]       structure variable containing http_session 
 *							  other parameters
 * RETURNS
 *	none This HAS TO BE VOID for it to work and get the session properly!
*****************************************************************************/
static void http_request_hook_cb(VMINT bResponse, void* pSession);

/*****************************************************************************
 * FUNCTION
 *  make_http_request
 * DESCRIPTION
 *  This function will Make the http request
 * PARAMETERS
 *  method					VMSTR			[IN]		Http request type
 *	url						VMSTR			[IN]		url of the request
 *	headers[]				http_head_t		[IN]		http headers
 *	headers_count			int				[IN]		total number of headers
 *	post_parameters[]		http_head_t		[IN]		Post parameters
 *	post_parameters_count	int				[IN]		Post parameters count
 * RETURNS
 *	none
*****************************************************************************/
void make_http_request(VMSTR method, VMSTR url, http_head_t headers[], 
					   int headers_count, http_head_t post_parameters[], 
					   int post_parameters_count);

/*****************************************************************************
 * FUNCTION
 *  um_login_controller
 * DESCRIPTION
 *  This function is just a placeholder. Supposed to login. Should move it 
 * PARAMETERS
 *  username		[IN]		VMSTR		Username
 *  password		[IN]		VMSTR		Password
 * RETURNS
 *	none YET
*****************************************************************************/
void um_login_controller(VMSTR username, VMSTR password);