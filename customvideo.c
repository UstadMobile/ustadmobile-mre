#include "vmsys.h"
#include "vmmm.h"
#include "vmvideo.h"
#include "vmstdlib.h"

#define VM_VIDEO_EXIT		0
#define VM_VIDEO_IDLE		1
#define VM_VIDEO_PLAY		2
#define VM_VIDEO_PAUSE		3

#define VM_VIDEO_SPEED_DEFAULT		100

#define VM_VIDEO_OPEN_FILE 	(-1)
#define VM_VIDEO_OPEN_BUFFER 	(-2)

#define VM_INVALID_HANDLE  (-1)

#define VM_REPEATCOUNT  (1)

typedef struct {
	VMUINT player_layer_handle;
	VMUINT base_layer_handle;
	VMUINT16 repeat_count;
	VMINT is_play_audio;
	VMINT audio_path;
	VMUINT16 rotate;
	VMINT vm_video_open_type;	
	VMUINT64 vm_video_cur_time;
	VMINT vm_video_status;
	vm_video_open_result_callback vm_video_open_result_callback;
	vm_video_finish_callback vm_video_finish_callback; 
	VMINT data_handle;
	VMINT processid;
} video_context_t;


extern VMINT mre_running;

static VMINT vm_video_res_preprocess(void)
{
	if (vm_res_get_data_list_count_by_type(VM_RES_TYPE_AUDIO_BIT_HANDLE)>0
		|| vm_res_get_data_list_count_by_type(VM_RES_TYPE_VIDEO_HANDLE)>0
		|| vm_res_get_data_list_count_by_type(VM_RES_TYPE_RECORD_HANDLE)>0)  
	{
		return VM_VIDEO_FAILED;	
	}

	if (vm_res_get_data_list_count_by_type(VM_RES_TYPE_MIDI_HANDLE)>0)
	{
		vm_midi_stop_all();	
	}
	
	if (vm_res_get_data_list_count_by_type(VM_RES_TYPE_AUDIO_HANDLE)>0)
	{
		return (vm_audio_stop(NULL)==VM_AUDIO_SUCCEED)?VM_VIDEO_SUCCESS:VM_VIDEO_FAILED;
	}		

	vm_audio_suspend_bg_play();

	return VM_VIDEO_SUCCESS;	
}



VMINT vm_video_open_file_source(const VMWSTR filename,
                    vm_video_open_result_callback open_result_callback){
	int x =0;
	VMINT y = -1;
	//return y;

	VMINT ret=VM_VIDEO_FAILED;	

	if ((NULL==filename) || (vm_wstrlen(filename)<=0)|| (vm_video_res_preprocess()==VM_VIDEO_FAILED) )
	{	
		return VM_VIDEO_FAILED;	
	}

	/*
	if (s_video_struct->vm_video_status==VM_VIDEO_EXIT)
	{
		med_type_enum type;
		
		type = mdi_video_ply_get_media_type((S8*)filename);
		
#if !defined(MP4_DECODE)
		if ((type == MED_TYPE_MP4) || (type == MED_TYPE_3GP))
		{
			return VM_VIDEO_ERR_INVALID_RESOULTION;
		}
#endif
		
#if !defined(MJPG_DECODE)		
		if (type == MED_TYPE_MJPG)
		{
			return VM_VIDEO_ERR_INVALID_RESOULTION;
		}
#endif
		ret=mdi_video_ply_open_file(ASM_ANONYMOUS_ID, (S8*)filename,vm_video_sal_open_result_callback, 0);
	
		switch (ret)
		{
			
			case MDI_RES_VDOPLY_SUCCEED:
				{
					ret=VM_VIDEO_SUCCESS;
					s_video_struct->processid=vm_pmng_get_current_handle();
					s_video_struct->data_handle=vm_res_save_data(VM_RES_TYPE_VIDEO_HANDLE, (void *)s_video_struct,sizeof(video_context_t), NULL,s_video_struct->processid );
					s_video_struct->vm_video_open_type=VM_VIDEO_OPEN_FILE;
					s_video_struct->vm_video_status=VM_VIDEO_IDLE;
					s_video_struct->vm_video_open_result_callback=open_result_callback;
				}
				break;
			case MDI_RES_VDOPLY_ERR_MEMORY_INSUFFICIENT:	
				ret=VM_VIDEO_ERR_MEMORY_INSUFFICIENT;
				break;
				case MDI_RES_VDOPLY_ERR_FILE_TOO_LARGE:
				ret=VM_VIDEO_ERR_FILE_TOO_LARGE;
				break;
				case MDI_RES_VDOPLY_ERR_FRAMERATE_TOO_HIGH:
				ret=VM_VIDEO_ERR_FRAMERATE_TOO_HIGH;
				break;
				case MDI_RES_VDOPLY_ERR_INVALID_RESOULTION:
				ret=VM_VIDEO_ERR_INVALID_RESOULTION;
				break;
			
			default:
				ret=VM_VIDEO_ERR_OPEN_FILE_FAILED;
		}
	
	} */

	return ret;
	
}