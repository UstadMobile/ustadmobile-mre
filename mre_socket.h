#ifndef _MRE_APP_WIZARDTEMPLATE_
#define	_MRE_APP_WIZARDTEMPLATE_

/*****************************************************************************
	mre_socket.h
 *****************************************************************************/

/* ---------------------------------------------------------------------------
 * GLOBAL VARIABLES
 * ------------------------------------------------------------------------ */
VMINT       connect_status  = -1;

/* ---------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------ */
//mre_display_home_top_screen_ascii(VMCHAR *text);
//old mre_tcp_connection() declaration
//mre_dump_to_file

/*****************************************************************************
 * FUNCTION
 *  mre_tcp_callback_application Declaration
 *
 * DESCRIPTION
 *  Receives various state responses of TCP connection
 *
 * PARAMETERS
 *  handle                  [IN]   Handle of TCP connection               
 *  event                   [IN}   Particular state code that has been received by callback
 *
 * RETURNS
 *  VOID
*****************************************************************************/
//void mre_tcp_callback_application(VMINT handle, VMINT event);

#endif