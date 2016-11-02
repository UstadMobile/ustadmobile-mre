/*
mre_events.c Events for application
*/

#include "share.h"
#include "mre_events.h"
#include "mre_http.h" //Need it for some menu item declarations

typedef enum mre_iput_type_textbox
{
    INPUT_TYPE1, 
    INPUT_TYPE2, 
    INPUT_TYPE3,
    TOTAL_INPUT_TYPE 
}mre_iput_type_textbox;

//////////////////////////////SYSTEM EVENTS///////////////////////////////////
/*****************************************************************************
 * FUNCTION
 *  handle_sysevt
 * DESCRIPTION
 *  This function handles system events 
 * PARAMETERS
 *  event		    	[IN]      message id
 *  param               [IN]      parameter
 * RETURNS
 *	none
*****************************************************************************/
void handle_sysevt (VMINT message, VMINT param) 
{
	vm_log_debug ("system event handle function starts with message : %d and parameter : %d", message, param);	
	/* The application updates the screen when receiving the message VM_MSG_PAINT 
	*  what is sent after the application is activated. The application can skip 
	*  the process on screen when the VM_MSG_ACTIVE or VM_MSG_INACTIVE is received.
	*/
	switch (message) 
	{
		case VM_MSG_CREATE:
			/* the GDI operation is not recommended as the response of the message*/
			break;
		case VM_MSG_ACTIVE:
			break;
		case VM_MSG_PAINT:
            vm_log_debug ("paint message, support bg");
            
            layer_hdl[0] = vm_graphic_create_layer (0, 0, vm_graphic_get_screen_width (), 
	                                        vm_graphic_get_screen_height (), -1);
            vm_graphic_active_layer (layer_hdl[0]);
			/* set clip area */
			vm_graphic_set_clip (0, 0, 
				vm_graphic_get_screen_width (), 
				vm_graphic_get_screen_height ());
           
			//Starts the main menu guts of the application..
			mre_start_menu ();
			break;

		case VM_MSG_HIDE:
		case VM_MSG_INACTIVE:
            vm_log_debug ("VM_MSG_HIDE message, support bg");
            vm_graphic_delete_layer (layer_hdl[0]);
			break;

		case VM_MSG_QUIT:
            vm_log_debug ("VM_MSG_QUIT message, support bg");
			if ( layer_hdl[0] != -1 )
			{
				vm_graphic_delete_layer (layer_hdl[0]);
				layer_hdl[0] = -1;
			}
			break;
	}
}

///////////////////////////////KEY EVENTS/////////////////////////////////////
/*****************************************************************************
 * FUNCTION
 *  handle_keyevt
 * DESCRIPTION
 *  This function handles key events 
 * PARAMETERS
 *  event		    	[IN]      event no
 *  keycode             [IN]      keycode no
 * RETURNS
 *	none
*****************************************************************************/
void handle_keyevt (VMINT event, VMINT keycode) {
	VMINT file_result;
	VMCHAR username[MRE_STR_SIZE_MAX + 1];
	VMCHAR password[MRE_STR_SIZE_MAX + 1];
	
	sprintf(username, "karmakid02");
	sprintf(password, "karmakid02");

	vm_log_debug ("key event handl function starts with event : %d and keycode : %d", event, keycode);
	
	if (event == VM_KEY_EVENT_UP)
	{
		if (mre_get_socketcontext() == TRUE && mre_get_fileiocontext() == FALSE && mre_get_httpcontext()==FALSE){
			vm_log_file("Entering socket tests\n");
			switch(keycode) //MAIN MENU
			{
				case VM_KEY_NUM1:
					vm_log_file("Entering VM_TCP_APN_CMNET\n");
					mre_tcp_connection(VM_TCP_APN_CMNET, "www.google.com",
						mre_tcp_callback_application);
					//mre_tcp_connection(VM_TCP_APN_CMNET, "www.google.com");
					break;
				case VM_KEY_NUM2:
					vm_log_file("Entering VM_TCP_APN_CMWAP\n");
					mre_tcp_connection(VM_TCP_APN_CMWAP, "www.google.com",
						mre_tcp_callback_application);
					//mre_tcp_connection(VM_TCP_APN_CMWAP, "www.google.com");
					break;
				case VM_KEY_NUM0:
						vm_log_debug("Going back to Main Menu..");
						mre_set_socketcontext (FALSE);
						mre_set_fileiocontext(FALSE);
						mre_set_httpcontext(FALSE);
						mre_start_menu();
						break;
				default:
					break;

			}

		}

		if (mre_get_fileiocontext () == FALSE && mre_get_httpcontext() == FALSE){
			//MAIN MENU
			switch (keycode)
		    {
				case VM_KEY_NUM1:
					//In File IO Test
					vm_log_debug("mre_set_fileiosubcontext in main menu in VM_KEY_NUM2");
					mre_set_fileiocontext (TRUE);
					mre_start_io_menu();
					break;

				case VM_KEY_NUM2:
					//In HTTP Tests
					vm_log_debug("HTTP Tests in main menu in VM_KEY_NUM2");
					mre_set_httpcontext(TRUE);
					mre_start_http_menu();
					break; 

				case VM_KEY_NUM3:
					//Play a video
					vm_log_debug("Play a Video in main menu in VM_KEY_NUM3");
					play_video();
					break;

				case VM_KEY_NUM4:
					//In Http Test
					vm_log_file("mre_set_httpcontext in main menu in VM_KEY_NUM4\n");
					//http_result = 
					um_login_controller(username,password);
					break;

				case VM_KEY_NUM5:
					//In Socket Test
					vm_log_file("mre socket tests from main menu key 5\n");
					mre_set_socketcontext(TRUE);
					mre_start_socket_menu();
					break;

				case VM_KEY_NUM6:
					//Play a video in the native video player
					vm_log_debug("Play a Video in main menu in VM_KEY_NUM3");
					play_video_native_player("small.mp4");
					break;

				case VM_KEY_NUM7:
					//Nuklear GUI Testing
					vm_log_file("Going to start Nuklear GUI Test\n");
					initiate_nk_gui();
					break;

				case VM_KEY_NUM0:
						vm_log_debug("Going back to Main Menu..");
						mre_set_fileiocontext (FALSE);
						mre_start_menu();
						break;

				case VM_KEY_RIGHT_SOFTKEY:
					//Right Soft Key 
					//Basically Back Key
					mre_set_fileiocontext (FALSE);
					mre_start_menu ();
					break;

				//case VM_KEY_RIGHT_SOFTKEY:
					//vm_exit_app();
					//break;

				default:
					break;
		    }
            
		}else if(mre_get_fileiocontext () == TRUE && mre_get_httpcontext() == FALSE)
		{
			//FILE IO Test:
			if(mre_get_subcontext() == FALSE){
				switch (keycode)
				{
					//Main File IO Test Menu
					case VM_KEY_NUM1:
						vm_log_debug("create file result in VM_KEY_NUM1");
						file_result = mre_create_file ("file1.txt");
						vm_log_debug("create file result %d", file_result);
						vm_log_file("Created file file1.txt\n");
						mre_file_result (file_result);
						break;

					case VM_KEY_NUM2:
						vm_log_debug("mre_set_subcontext in main menu in VM_KEY_NUM2");
						mre_set_subcontext (TRUE);
						mre_io_submenu ();
						break;

					case VM_KEY_NUM3:
						vm_log_debug("mre_display_file_contents in main menu in VM_KEY_NUM3");
						file_result = mre_display_file_contents ("file1.txt");
						vm_log_debug("mre_display_file_contents result %d",file_result);
						mre_file_result (file_result);
						break;

					case VM_KEY_NUM0:
						vm_log_debug("Going back to Main Menu..");
						mre_set_fileiocontext (FALSE);
						mre_start_menu();
						break;

					case VM_KEY_RIGHT_SOFTKEY:
						//Right Soft Key 
						//Basically Back Key
						mre_start_menu ();
						break;

					default:
						break;
				}
	            
			}
			else if (mre_get_subcontext () == TRUE)
			{
	            //Sub Menu File IO Test:
				switch (keycode)
				{
					case VM_KEY_NUM1:    
						mre_set_subcontext (FALSE);
						vm_log_debug("mre_set_subcontext in VM_KEY_NUM1 ");
						file_result = mre_show_input_textbox(INPUT_TYPE1);
						vm_log_debug("mre_set_subcontext result %d",file_result);
						mre_file_result (file_result);
						break;

					case VM_KEY_NUM2:
						mre_set_subcontext (FALSE);
						vm_log_debug("mre_set_subcontext in VM_KEY_NUM2 ");
						file_result = mre_show_input_textbox(INPUT_TYPE2);
						vm_log_debug("mre_set_subcontext result %d",file_result);
						mre_file_result (file_result);
						break;

					case VM_KEY_NUM3:
						mre_set_subcontext (FALSE);
						vm_log_debug("mre_set_subcontext in VM_KEY_NUM3");
						file_result = mre_show_input_textbox(INPUT_TYPE3);
						vm_log_debug("mre_set_subcontext result %d",file_result);
						mre_file_result (file_result);
						break;


					case VM_KEY_NUM0:
						vm_log_debug("Going back to File IO Main Menu..");
						mre_set_fileiocontext (TRUE);
						mre_set_subcontext(FALSE);
						mre_start_io_menu();
						break;

					case VM_KEY_RIGHT_SOFTKEY:
						//Right Soft Key 
						//Basically Back Key
						vm_log_debug("mre_set_subcontext in VM_KEY_RIGHT_SOFTKEY");
						mre_set_subcontext (FALSE);
						mre_start_menu ();
						break;

					default:
						break;
					}
			}
		}
		else if (mre_get_httpcontext () == TRUE && mre_get_fileiocontext() == FALSE){
			//Http Tests MENU
			switch (keycode)
		    {
				case VM_KEY_NUM1:
					//In Http Test
					vm_log_debug("mre_set_httpcontext in main menu in VM_KEY_NUM1");
					connect_http();
					break;

				case VM_KEY_NUM2:
					vm_log_debug("2nd Test in main menu in VM_KEY_NUM2");
					break;

				case VM_KEY_RIGHT_SOFTKEY:
					//Right Soft Key 
					//Basically Back Key
					mre_set_httpcontext (FALSE);
					mre_start_menu ();
					break;

				case VM_KEY_NUM0:
					vm_log_debug("Going back to Main Menu..");
					mre_set_httpcontext (FALSE);
					mre_start_menu();
					break;

				default:
					break;
		    }
            
		}
		//else if (Another Test Mode is True)
		// ..
	}
    vm_log_debug ("key event handle function exits");
}

///////////////////////////////PEN EVENTS/////////////////////////////////////
/*****************************************************************************
 * FUNCTION
 *  handle_penevt
 * DESCRIPTION
 *  This function handles touch panel events 
 * PARAMETERS
 *  event				[IN]      event no
 *  x					[IN]      x cordinate of cursor point on screen
 *  y					[IN]      y cordinate of cursor point on screen
 * RETURNS
 *	none
*****************************************************************************/
void handle_penevt (VMINT event, VMINT x, VMINT y)
{
	vm_log_debug ("touch event handle function starts with event : %d and x,y :%d, %d", event, x, y);
    vm_log_debug ("touch event handle function exits");

}