/*
	mre_tcp_socket.h Tcp socket stuff's Header file
*/


/*****************************************************************************
 * FUNCTION
 *  mre_tcp_connection Declaration
 *
 * DESCRIPTION
 *  Connect with specified URL through TCP
 *
 * PARAMETERS
 *  APN                     [IN]   specify wheather APN is CMNET or CMWAP    
 *  URL						[IN]	url to connect to
 * 
 * RETURNS
 *  VOID
*****************************************************************************/
void mre_tcp_connection(VMINT apn, VMSTR url, void (*tcp_callback)(VMINT handle, VMINT event));

/*****************************************************************************
 * FUNCTION
 * mre_tcp_callback_application
 *
 * DESCRIPTION
 * Displays tcp callback application
 *
 * PARAMETERS
 * handle             [IN]     
 * event              [IN]  connection handle returned by vm_tcp_connect()    
 * 
 * RETURNS
 * VOID
*****************************************************************************/
void mre_tcp_callback_application(VMINT handle, VMINT event);