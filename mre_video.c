/* 
mre_video.c Video related 
*/ 

#include "share.h"
#include "mre_video.h"

/*****************************************************************************
 * FUNCTION
 *  play_video
 * DESCRIPTION
 *  This function starts the stuff needed to play the video
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void play_video(void)
{
	VMINT x = 0;
	VMINT y = 0;
	VMINT width = 240;
	VMINT height = 320;
	VMINT trans_color = 1;
	VM_GDI_RESULT vm_grl_result;
	VMINT count = vm_graphic_get_blt_layer_count();	
	VMUINT16 repeat_count = 2;
	VMINT is_play_audio = 1;
	VMINT audio_path = 1;
	VMINT play_result;
	VMINT file_open_result;
	VMINT test_handle;
	VMINT g_flush_result;
	VMINT v_seek_result;
	VMINT g_layer_delete_result;
	VMINT v_stop_result;
	VMINT v_close_result;
	//VMINT v_play_by_filepath_result; 
	VMSTR video_filename_string;
	VMWSTR video_filename;
	VMINT video_filename_size;
	VMCHAR video_filepath[MRE_STR_SIZE_MAX + 1];
	vm_video_open_result_callback orc = video_open_callback;	
	vm_video_finish_callback fc = NULL;
	//VMINT layer_handle[1] = {0}; //Commenting cause already defined..201020162028
	VMCHAR f_name[MRE_STR_SIZE_MAX + 1];	//Old usage for video filename string
	VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1]; //Old usage for video filename
	VMCHAR log_message[MRE_STR_SIZE_MAX + 1];

	VMSTR filename = "small.mp4";

	vm_log_file("\nCustom: Video playback\n");
	
	//Zero-ing the layer as replacement for above {0}
	//layer_handle[1] = 0; //throws error: Access violation
	
	sprintf (f_name, "%c:\\%s", mre_get_drv(), filename);

	//Notes:
	//video_filename_string = "small.mp4"; //->Works
	//sprintf(video_filename_string, "%c:\\%s", mre_get_drv(), "small.mp4"); //->Invalid
	//video_filename_string = "%c:\\%s", mre_get_drv(), "small.mp4"; //->Doesnt work

	sprintf(video_filepath, "%c:\\%s", mre_get_drv(), filename);
	//sprintf(video_filepath,filename); //just testing a thought //Update: Didnt do anything

	video_filename_string = vm_malloc(MRE_STR_SIZE_MAX + 1);
	strcpy(video_filename_string, video_filepath);
	video_filename_size = (strlen(video_filename_string) + 1) * 2;
	video_filename = vm_malloc(video_filename_size);
	vm_ascii_to_ucs2(video_filename, video_filename_size, video_filename_string);


	/* string format conversion */
	vm_ascii_to_ucs2 (f_wname, MRE_STR_SIZE_MAX, f_name);

	/* test that the file exists, we have access to it */
	test_handle = vm_file_open(f_wname, MODE_READ, FALSE);
	sprintf(log_message, "File: %s opened result: %d", 
	f_name, test_handle);
	vm_log_file(log_message);
	vm_log_file("..\n");
	if(test_handle < 0)
	{
		// -2 is : VM_FILE_OPEN_ERROR
		vm_log_file("File doesnt exist - file handle FAIL!\n");
	}else{
		vm_log_file("File exists- file handle SUCCESS \n");
	}

	/* We have to close the file opened right? */
	vm_log_file("Closing the file handle.\n");
	vm_file_close(test_handle);
	
	/*
	 * EXAMPLE
	 * VMINT layer_hdl[2] = {0};			
	 * layer_hdl[0] = vm_graphic_create_layer(0, 
	 *		0, vm_graphic_get_screen_width(), 
	 *			vm_graphic_get_screen_height(), 
	 *				VM_NO_TRANS_COLOR);
	 * //user can draw on the layer
	 *      vm_graphic_line_ex( layer_hdl[0],0,0,100,100);
	 *     ....
	 * // when need to show it to the LCD
	 *   vm_graphic_flush_layer( layer_hdl[0], 1);
	 * //when no more use the layer, delete it
	 *   vm_graphic_delete_layer(layer_hdl[0]);
	 ************************************************
	 */

	//According to the MRE Tutorial these are the steps needed to play the video:
	//1. Create a layer for playback	
	layer_handle[0] = vm_graphic_create_layer(x, y, width, height, trans_color);
	//handle = vm_graphic_create_layer(x, y, width, height, trans_color);

	//2. Specifying the size of the playback layer
	vm_grl_result = vm_graphic_resize_layer(layer_handle[0], width, height);
	//vm_grl_result = vm_graphic_resize_layer(handle, width, height);
	if (vm_grl_result == VM_GDI_SUCCEED ){
		vm_log_file("Graphic resize layer is ok\n");
	}else{
		vm_log_file("Graphic resize layer specified FAIL.\n");
	}

	
	//3. Flushing the playback layer: flatten layers to one layer and blt to LCD.
	//(*layer_handles: The pointer of the layer array)
	//count	: [IN] layer number that  to be flushed.
	g_flush_result = vm_graphic_flush_layer(layer_handle, 1);
	//g_flush_result = vm_graphic_flush_layer(&layer_handle, 1): //-> Gives error on ARM.
	//g_flush_result = vm_graphic_flush_layer(&layer_handle, 0); //->Fails with -2
	//g_flush_result = vm_graphic_flush_layer(layer_handle[0], 1); //->Throws exception: mre 'example' is wrong
	//g_flush_result = vm_graphic_flush_layer(*layer_handle, count); //-> Throws exception
	if (g_flush_result == VM_GDI_SUCCEED){
		vm_log_file("Graphic flush layer is specified ok.\n");
	}else{
		vm_log_file("Graphic flush layer NOT specified ok.\n");
	}
	
	//4. Drawing the tools layer
	//Lets skip this so that the video plays in full screen

	vm_log_file("OPENING VIDEO FILE (AS VIDEO) TO PLAY\n");

	//5. Opening a file to play 
	//file_open_result = vm_video_open_file(f_wname, orc);
	file_open_result = vm_video_open_file((const VMWSTR)video_filename, orc);
	//file_open_result = vm_video_open_file_source((const VMWSTR)video_filename, orc);
	sprintf(log_message, "Video file: %s opened result: %d", 
		video_filename_string, file_open_result);
	if (file_open_result >= 0){
		vm_log_file("Video File open ok.\n");
		vm_log_file(log_message);
		vm_log_file("..\n");
	}else{
		vm_log_file("Video file open fail!\n");
		vm_log_file(log_message);
		vm_log_file("..\n");
		//VM_VIDEO_ERR_OPEN_FILE_FAILED 
	}

 
	//6. Seek first video frame or required video frame in time
	//   parameters : VMUINT64 time, VMUINT player_layer_handle
	v_seek_result = vm_video_seek_and_getframe(0, layer_handle[0]);
	//v_seek_result = vm_video_seek_and_getframe(0, handle);

	if(v_seek_result == VM_VIDEO_SUCCESS){
		vm_log_file("Video seek SUCCESS\n");
	}else if(v_seek_result == VM_VIDEO_FAILED){
		vm_log_file("Video seek Failure!\n");
	}else{
		vm_log_file("Video seek unknown Failure!\n");
	}

	//7.a: Playing a video in the player handle created.
	play_result = vm_video_play(layer_handle[0],NULL,repeat_count, 
		is_play_audio,audio_path, fc);
	if (play_result < 0){
		vm_log_file("Video playback failed!\n");
		//vm_log_debug(play_result);
		//VM_VIDEO_ERR_PLAY_FAILED is -106
	}else{
		printf("Video playback was a success!");
		vm_log_file("Video playback SUCCESS!\n");
	}

	//8. Delete the layer
	g_layer_delete_result = vm_graphic_delete_layer(layer_handle[0]);
	//g_layer_delete_result = vm_graphic_delete_layer(handle);
	if(g_layer_delete_result == VM_GDI_SUCCEED){
		vm_log_file("Graphic layer delete success.\n");
	}else{
		vm_log_file("Graphic layer delete FAIL\n");
	}

	//9. Stop Video
	v_stop_result = vm_video_stop();
	if (v_stop_result == VM_VIDEO_SUCCESS){
		vm_log_file("Video stop ok.\n");
	}else if (v_stop_result == VM_VIDEO_FAILED){
		vm_log_file("Video stop failed!\n");
	}else{
		vm_log_file("video stop fail unknown!\n");
	}
	
	//10. Close video file
	v_close_result = vm_video_close_file();

	if (v_close_result == VM_VIDEO_SUCCESS ){
		vm_log_file("Video file closed ok\n");
	}else if (v_close_result == VM_VIDEO_FAILED){
		vm_log_file("Video file closed NOT ok\n");
	}else{
		vm_log_file("Vide file closed NOT ok unknown error!\n");
	}

	vm_log_file("END OF VIDEO TESTS.\n");
	vm_log_file("\n");
	
}

/*****************************************************************************
 * FUNCTION
 *  play_video_native_player
 * DESCRIPTION
 *  This function starts the stuff needed to play the video in the native 
 *  video player
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void play_video_native_player(VMSTR filename)
{
	VMINT x = 0;
	VMINT y = 0;
	VMINT width = 240;
	VMINT height = 320;
	VMINT trans_color = 1;
	VMINT count = vm_graphic_get_blt_layer_count();	
	VMUINT16 repeat_count = 2;
	VMINT is_play_audio = 1;
	VMINT audio_path = 1;
	VMINT test_handle;
	VMINT v_play_by_filepath_result; 
	VMSTR video_filename_string;
	VMWSTR video_filename;
	VMINT video_filename_size;
	VMCHAR video_filepath[MRE_STR_SIZE_MAX + 1];
	vm_video_open_result_callback orc = video_open_callback;	
	vm_video_finish_callback fc = NULL;
	//VMINT layer_handle[1] = {0}; //Already defined??
	VMCHAR f_name[MRE_STR_SIZE_MAX + 1];	//Old usage for video filename string
	VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1]; //Old usage for video filename
	VMCHAR log_message[MRE_STR_SIZE_MAX + 1];

	//VMSTR filename = "small.mp4";

	vm_log_file("\nNative: Video Playback\n");

	//layer_handle[1] = 0; //Zero-ing the handle..Corrupts the layer handle..
	
	sprintf (f_name, "%c:\\%s", mre_get_drv(), filename);

	sprintf(video_filepath, "%c:\\%s", mre_get_drv(), filename);

	video_filename_string = vm_malloc(MRE_STR_SIZE_MAX + 1);
	strcpy(video_filename_string, video_filepath);
	video_filename_size = (strlen(video_filename_string) + 1) * 2;
	video_filename = vm_malloc(video_filename_size);
	vm_ascii_to_ucs2(video_filename, video_filename_size, video_filename_string);


	/* string format conversion */
	vm_ascii_to_ucs2 (f_wname, MRE_STR_SIZE_MAX, f_name);

	/* test that the file exists, we have access to it */
	test_handle = vm_file_open(f_wname, MODE_READ, FALSE);
	sprintf(log_message, "File: %s opened result: %d", 
	f_name, test_handle);
	vm_log_file(log_message);
	vm_log_file("..\n");
	if(test_handle < 0)
	{
		// -2 is : VM_FILE_OPEN_ERROR
		vm_log_file("File doesnt exist - file handle FAIL!\n");
	}else{
		vm_log_file("File exists- file handle SUCCESS \n");
	}

	/* We have to close the file opened right? */
	vm_log_file("Closing the file handle.\n");
	vm_file_close(test_handle);

	//Try to play the video in phone's own player
	// this launches in the emulator but fails to play
	// Does not even launch on device (LG)
	v_play_by_filepath_result = vm_video_play_by_filepath(video_filename);
	sprintf(log_message, "VIDEO File: %s opened result: %d", 
		video_filename_string, v_play_by_filepath_result);
	vm_log_file(log_message);
	vm_log_file("..\n");
	if(v_play_by_filepath_result == VM_VIDEO_SUCCESS){
		vm_log_file("Video playback by filename (native player) SUCCESS.\n");
	}else{
		vm_log_file("Video playback by filename (native player) FAIL!\n");
	}

	vm_log_file("END OF VIDEO TESTS.\n");
	vm_log_file("\n");
	
}


/***
Get Thumbnail from Video:
Gets middle frame's frame -> pikchar
****/ 
void get_video_thumbnail(VMWSTR video_filename, VMWSTR video_thumbnail_filename){
	VMINT x = 0;
	VMINT y = 0;
	VMINT width = 240;
	VMINT height = 320;
	VMINT trans_color = 1;
	VMINT count = vm_graphic_get_blt_layer_count();	
	VMUINT16 repeat_count = 2;
	VMINT is_play_audio = 1;
	VMINT audio_path = 1;
	VMINT test_handle;
	VMINT v_play_by_filepath_result; 
	VMSTR video_filename_string;
	VMINT video_filename_size;
	VMCHAR video_filepath[MRE_STR_SIZE_MAX + 1];
	vm_video_open_result_callback orc = video_open_callback;	
	vm_video_finish_callback fc = NULL;
	VMCHAR f_name[MRE_STR_SIZE_MAX + 1];	//Old usage for video filename string
	VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1]; //Old usage for video filename
	VMCHAR log_message[MRE_STR_SIZE_MAX + 1];
	VMINT file_open_result;
	VMINT v_seek_result;
	VMINT vm_video_snapshot_result;	
	vm_video_open_result_callback orc_middle_time = video_open_callback_get_thumbnail;

	printf("\n Getting Video file's thumbnail .. ");

	//5. Opening a file to play 
	file_open_result = vm_video_open_file((const VMWSTR)video_filename, orc_middle_time);
	
	//6. Seek first video frame or required video frame in time
	//v_seek_result = vm_video_seek_and_getframe(time_middle, layer_handle[0]);
	
	vm_video_snapshot_result = vm_video_snapshot(layer_handle[0], video_thumbnail_filename);

}

//typedef void (*vm_video_open_result_callback)(VMINT ret, vm_video_info_struct *video_info);
void video_open_callback_get_thumbnail( VMINT ret, vm_video_info_struct *video_info){
	VMUINT64 time_middle;
	VMINT v_seek_result;
	VMINT layer_handle[1];

	layer_handle[0] = 0;
	printf("\nIn video open callback..");
	time_middle = (VMUINT64) video_info->total_time_duration / 2;
	printf("\nGot middle time..");

	//6. Seek first video frame or required video frame in time
	//   parameters : VMUINT64 time, VMUINT player_layer_handle
	v_seek_result = vm_video_seek_and_getframe(time_middle, layer_handle[0]);

		if(v_seek_result == VM_VIDEO_SUCCESS){
			vm_log_file("Video seek SUCCESS\n");
		}else if(v_seek_result == VM_VIDEO_FAILED){
			vm_log_file("Video seek Failure!\n");
		}else{
			vm_log_file("Video seek unknown Failure!\n");
		}

	

}



/*
static vm_video_open_result_callback open_video_callback(void){	
	vm_video_open_result_callback v;
	vm_log_debug("Video File Open Callback!");
	return v;
}

static vm_video_finish_callback play_video_callback(void){
	vm_video_finish_callback v;
	vm_log_debug("Play Video Callback!");	
	return v;
}
*/

//typedef void (*vm_video_open_result_callback)(VMINT ret, vm_video_info_struct *video_info);
void video_open_callback( VMINT ret, vm_video_info_struct *video_info){
	printf("In video open callback..");
}

/*
//Trying with type retutn
vm_video_open_result_callback video_open_callback2( VMINT ret, vm_video_info_struct *video_info){
	printf("In video open callback..");
}
*/