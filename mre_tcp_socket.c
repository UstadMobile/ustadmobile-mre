/*
mre_tcp_socket.c TCP Socket stuff code
*/

#include "share.h"
#include "mre_tcp_socket.h"

/*****************************************************************************
 * FUNCTION
 * mre_tcp_connection Definition
 *
 * DESCRIPTION
 * Establishes connection.
 *     
 * RETURNS
 * VOID
*****************************************************************************/
//void mre_tcp_connection_url(VMINT  apn, VMSTR url, void (*http_hook)(VMINT, void*))
//void http_request_hook_cb(VMINT bResponse, void* pSession)
//void mre_tcp_callback_application(VMINT handle, VMINT event)
//, void (*tcp_callback)(VMINT handle, VMINT event)
void mre_tcp_connection(VMINT  apn, VMSTR url, void (*tcp_callback)(VMINT handle, VMINT event))
{
	//VMINT connect_status = -1; //Temporary. TODO: Remove this line
	char   *host;
	VMINT  port = MRE_TCP_CONNECTION_PORT;
	
	host = vm_malloc(MRE_STRING_MAX_SIZE);
	memset(host, MRE_STRING_INTIAL_VALUE, MRE_STRING_MAX_SIZE);
	strcpy(host, url);
	
	connect_status = -1;
	//connect_status = vm_tcp_connect(host,port,apn,mre_tcp_callback_application); 
	connect_status = vm_tcp_connect(host,port,apn,tcp_callback);

	vm_log_debug("Entering %s","mre_tcp_connect");

}


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
void mre_tcp_callback_application(VMINT handle, VMINT event)
{
	/* local variables */
	VMINT length;
	VMCHAR *  layer_buffer;

	VMCHAR string[]="GET http://www.google.com HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
	VMINT count_bytes;
	layer_buffer= vm_malloc(MRE_TCP_BUFFER_MAX_SIZE);

	/* function body */
	vm_log_debug("Entering %s", "mre_tcp_callback_application");
     switch (event)
	{
		case VM_TCP_EVT_HOST_NOT_FOUND : 
			{
                vm_log_debug("Entering %s","VM_TCP_EVT_HOST_NOT_FOUND");
				//mre_display_home_top_screen_ascii("Host not found");
				mre_display_home_top_screen_ascii("Host not found", layer_hdl);
				break;
			}
		case VM_TCP_EVT_CONNECTED : 
            { 
                vm_log_debug("Entering %s","VM_TCP_EVT_CONNECTED");
                length=strlen(string);
				count_bytes = vm_tcp_write(handle,string,length);
				//mre_display_home_top_screen_ascii("Connected");
				mre_display_home_top_screen_ascii("Connected", layer_hdl);
				mre_create_file_for_write("TCP.txt", layer_hdl);
				break;
			}
		case VM_TCP_EVT_CAN_WRITE :
			{ 
                vm_log_debug("Entering %s","VM_TCP_EVT_CAN_WRITE");
			    //vm_tcp_write(handle, layer_buffer,length);
				break;
			}
		case VM_TCP_EVT_CAN_READ :
			{
				vm_log_debug("Entering %s","VM_TCP_EVT_CAN_READ");
				count_bytes = vm_tcp_read (handle, layer_buffer, sizeof(layer_buffer) - 1);
				if (count_bytes > 0)
				{
					//mre_dump_to_file(layer_buffer, count_bytes);
					mre_dump_to_file(layer_buffer, count_bytes, layer_hdl);
					//mre_display_home_top_screen_ascii("Response dumped in file");
					mre_display_home_top_screen_ascii("Response dumped in file", layer_hdl);
					memset(layer_buffer, 0, MRE_TCP_BUFFER_MAX_SIZE);
				}
				while (count_bytes > 0)
				{
					count_bytes = vm_tcp_read (handle, layer_buffer, sizeof(layer_buffer) - 1);
					//mre_dump_to_file(layer_buffer, count_bytes);
					mre_dump_to_file(layer_buffer, count_bytes, layer_hdl);
					//mre_display_home_top_screen_ascii("Response dumped in file");
					mre_display_home_top_screen_ascii("Response dumped in file", layer_hdl);
					memset(layer_buffer, 0, MRE_TCP_BUFFER_MAX_SIZE);
				}
				//mre_display_home_top_screen_ascii("Response Read");
				mre_display_home_top_screen_ascii("Response Read",  layer_hdl);
				break;
			}
		case VM_TCP_EVT_PIPE_BROKEN :
			{
                vm_log_debug("Entering %s","VM_TCP_EVT_PIPE_BROKEN");
				break;
			}
		case VM_TCP_EVT_PIPE_CLOSED :
			{
				vm_tcp_close(connect_status);
				vm_log_debug("Entering %s","VM_TCP_EVT_PIPE_CLOSED");
				break;
			}
		default:
			{
				vm_tcp_close(connect_status);
                vm_log_debug("Entering %s","default");

			}
	}
	vm_free(layer_buffer);
	layer_buffer = NULL;
}