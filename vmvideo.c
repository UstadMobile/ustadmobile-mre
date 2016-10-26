#include "vmswitch.h"
#include "mmi_features.h"
#include "vmsys.h"
#ifdef __MRE_SAL_VIDEO__
#include <string.h>
#include "vmlog.h"
#include "vmmod.h"
#include "vmmm.h"
#include "vmmem.h"
#include "vmgraph.h"
#include "vmstdlib.h"
#include "vmsalvideo.h"
#include "vmresmng.h"
#include "vmpromng.h"
#include "vmpromnginner.h"
#include "vmvideo.h"
#include "MMI_trc.h"
#include "vmmacrostub.h"
#include "mmi_rp_file_type_def.h"
#include "FileMgrSrvGProt.h"

#if (defined __COSMOS_MMI_PACKAGE__ && defined __MMI_URI_AGENT__)
#include "UriAgentSrvGprot.h"
#endif

#include "mdi_video.h"

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

extern VM_GDI_RESULT  vm_graphic_delete_layer_internal(VMINT handle);
extern VMUINT vm_graphic_get_trans_handle(VMINT handle);
extern    med_type_enum mdi_video_ply_get_media_type(const S8 *filename);

#if (defined __MMI_VIDEO_STREAM__ && defined __MMI_VIDEO_PLAYER__)
extern void mmi_vdoply_entry_player_screen_from_sdp_file(PS8 sdp_filename, BOOL is_short);
#endif
#ifdef __MMI_MEDIA_PLAYER_STREAM__
extern void mmi_medply_single_play_stream_from_sdp_file(UI_string_type sdp_file, MMI_BOOL is_short);
#endif
 
#if (defined __MMI_VIDEO_STREAM__ && defined __MMI_VIDEO_PLAYER__)
extern void mmi_vdoply_entry_player_screen_from_rtsp_link(PS8 rtsp_url);
#endif
#ifdef __MMI_MEDIA_PLAYER_STREAM__
extern void mmi_medply_single_play_stream_from_rtsp_link(UI_string_type rtsp_url);
#endif

static video_context_t *s_video_struct=NULL;
static vm_video_info_struct vm_video_info;

static void vm_video_finalize_preprocess(VMINT if_need_close);
static void vm_video_reset_context(video_context_t* video_struct);
static VMINT vm_video_mod_evt_proc(MRE_MOD_LIFECIRCLE_EVT event);
static VMINT vm_video_interface_preprocess(VMINT check_if_foregroud);
static VMINT vm_video_init(void);
static void vm_video_finalize(void);
static void vm_video_sal_open_result_callback( MDI_RESULT ret, mdi_video_info_struct *video_info, void *user_data);
static void vm_video_get_info_values(vm_video_info_struct *toinfo,const mdi_video_info_struct *frominfo);
static VMINT vm_video_get_layer_flag(VMUINT blthanlde,VMUINT playhandle,VMUINT * gdi_play_layer_handle,VMUINT *gdi_base_layer_handle, VMUINT *bltflag,VMUINT *playflag);

extern VMINT vm_video_sal_open_buffer( const VMUSTR buffer, const VMUINT buffer_len, vm_video_info_struct *info);
extern VMINT vm_video_sal_play( VMUINT player_layer_handle,VMUINT base_layer_handle,VMUINT16 repeat_count,  VMINT is_visual_update,
				   VMINT is_play_audio, VMINT audio_path, VMUINT16 rotate, VMUINT16 play_speed,mdi_play_finish_callback play_finish_callback);
extern VMINT vm_video_sal_seek_and_getframe(VMUINT64 time, VMUINT player_layer_handle);
extern VMINT vm_video_sal_play( VMUINT player_layer_handle,VMUINT base_layer_handle,VMUINT16 repeat_count,  VMINT is_visual_update,
				   VMINT is_play_audio, VMINT audio_path, VMUINT16 rotate, VMUINT16 play_speed,mdi_play_finish_callback play_finish_callback);

VMINT _vm_reg_video_modual(void)
{
	int res_code = REG_MRE_MODULE_SUCCESS;
	
	if ((res_code = _vm_reg_module("VIDEO MODULE", (MOD_EVT_PROCESS)vm_video_mod_evt_proc)) != REG_MRE_MODULE_SUCCESS)
	{
		MMI_TRACE(MMI_MRE_TRC_MOD_VMVIDEO,TRC_MRE_VMVIDEO_01,res_code);
	}

	return res_code;
}

static VMINT vm_video_mod_evt_proc(MRE_MOD_LIFECIRCLE_EVT event)
{
	switch(event)
	{
	case EVT_MOD_INIT:
		vm_video_init();
		break;
	case EVT_MOD_RELEASE:
		vm_video_finalize();
		break;
	default:
		break;
	}
	return REG_MRE_MODULE_SUCCESS;
}

static void vm_video_play_finish_callback(MDI_RESULT ret, void *user_data)
{
	if (mre_running == FALSE)
	   return;
	
	if (ret>=0)
	{
		s_video_struct->vm_video_status=VM_VIDEO_IDLE;
		s_video_struct->vm_video_cur_time=0;
	}

	if (s_video_struct && (s_video_struct->vm_video_finish_callback))
	{
		if (vm_pmng_set_ctx(s_video_struct->processid) == VM_PMNG_OP_OK)
 		{
			s_video_struct->vm_video_finish_callback(ret);
			vm_pmng_reset_ctx();
		}
	}	
}


static void vm_video_reset_context(video_context_t* video_struct)
{
	if (video_struct)
	{
		memset(video_struct,0x00,sizeof(video_context_t));
		video_struct->player_layer_handle=(VMUINT)VM_INVALID_HANDLE;
		video_struct->base_layer_handle=(VMUINT)VM_INVALID_HANDLE;
		video_struct->repeat_count=VM_REPEATCOUNT;
		video_struct->is_play_audio=TRUE;
		video_struct->audio_path=VM_DEVICE_SPEAKER2;
		video_struct->rotate=VM_VIDEO_LCD_ROTATE_0;
		video_struct->vm_video_open_type=0;
		video_struct->vm_video_cur_time=0;
		video_struct->data_handle=VM_INVALID_HANDLE;
		video_struct->processid=VM_INVALID_HANDLE;
		video_struct->vm_video_status=VM_VIDEO_EXIT;
		video_struct->vm_video_open_result_callback=NULL;
		video_struct->vm_video_finish_callback=NULL;	
	}

}

void vm_video_release_callback(VM_P_HANDLE process_handle, VMINT sys_state)
{
	if (process_handle==s_video_struct->processid)
	{
		switch (sys_state)
		{
			case VM_PMNG_UNLOAD:
			case VM_PMNG_BACKGROUND:
				vm_video_finalize_preprocess(TRUE);
				break;
		}
	}
}

static VMINT vm_video_init(void)
{
	if (!s_video_struct) 
	{
		if ((s_video_struct=(video_context_t *)_vm_kernel_malloc(sizeof(video_context_t)))==NULL)
		{
			return VM_VIDEO_FAILED;
		}
	}
	vm_video_reset_context(s_video_struct);
	vm_res_type_set_notify_callback(VM_RES_TYPE_VIDEO_HANDLE,vm_video_release_callback);
	return VM_VIDEO_SUCCESS;
}

static void vm_video_finalize_preprocess(VMINT if_need_close)
{
	if (s_video_struct)
	{
		if (s_video_struct->vm_video_status!=VM_VIDEO_EXIT)
		{
			if (s_video_struct->vm_video_status==VM_VIDEO_PAUSE || s_video_struct->vm_video_status==VM_VIDEO_PLAY)
			{
				mdi_video_ply_stop();
				s_video_struct->vm_video_status=VM_VIDEO_IDLE;	
			}

			if (if_need_close)
			{
				switch (s_video_struct->vm_video_open_type)
				{
					case VM_VIDEO_OPEN_FILE:
						mdi_video_ply_close_file();
						break;
					case VM_VIDEO_OPEN_BUFFER:
						mdi_video_ply_close_clip_buffer();
						break;
				}
				s_video_struct->vm_video_status=VM_VIDEO_EXIT;
				vm_res_release_data(VM_RES_TYPE_VIDEO_HANDLE, s_video_struct->data_handle);
				vm_video_reset_context(s_video_struct);
			}
		}

		if ((s_video_struct->player_layer_handle!=(VMUINT)VM_INVALID_HANDLE) && (vm_graphic_get_trans_handle(s_video_struct->player_layer_handle)!=0))
		{
			vm_graphic_delete_layer_internal(s_video_struct->player_layer_handle);
			s_video_struct->player_layer_handle=(VMUINT)VM_INVALID_HANDLE;
		}
		
		if ((s_video_struct->base_layer_handle!=(VMUINT)VM_INVALID_HANDLE) && (vm_graphic_get_trans_handle(s_video_struct->base_layer_handle)!=0))
		{
			vm_graphic_delete_layer_internal(s_video_struct->base_layer_handle);
			s_video_struct->base_layer_handle=(VMUINT)VM_INVALID_HANDLE;
		}
	}
	vm_audio_resume_bg_play();
	
}

static void vm_video_finalize(void)
{
	vm_video_finalize_preprocess(TRUE);
	 
	if (s_video_struct)
	{
		_vm_kernel_free(s_video_struct);
		s_video_struct=NULL;
	}
}


static VMINT vm_video_interface_preprocess(VMINT check_if_foregroud)
{
	VM_P_HANDLE phandle=NULL;

	if (vm_res_get_data_list_count_by_type(VM_RES_TYPE_VIDEO_HANDLE)>0)
	{
		phandle=vm_pmng_get_current_handle();	

		if (check_if_foregroud)				
		{	
			if (!vm_pmng_is_fg(phandle))
				return VM_VIDEO_FAILED;
		}
		
		if (phandle==s_video_struct->processid)
		{
			return VM_VIDEO_SUCCESS;
		}
		
	}
	return VM_VIDEO_FAILED;

}



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

VMINT vm_video_open_file(const VMWSTR filename, vm_video_open_result_callback open_result_callback)
{
	VMINT ret=VM_VIDEO_FAILED;	

	if ((NULL==filename) || (vm_wstrlen(filename)<=0)|| (vm_video_res_preprocess()==VM_VIDEO_FAILED) )
	{	
		return VM_VIDEO_FAILED;	
	}

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
	
	} 

	return ret;
	
}

VMINT vm_video_close_file(void)
 {
	VMINT ret=VM_VIDEO_FAILED;

	if (vm_video_interface_preprocess(FALSE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;
	
	if ((s_video_struct->vm_video_status==VM_VIDEO_IDLE ) && (s_video_struct->vm_video_open_type==VM_VIDEO_OPEN_FILE) )
	{
		ret=mdi_video_ply_close_file();

		if (ret==VM_VIDEO_SUCCESS)
		{
			s_video_struct->vm_video_status=VM_VIDEO_EXIT;
			vm_res_release_data(VM_RES_TYPE_VIDEO_HANDLE, s_video_struct->data_handle);
			vm_video_reset_context(s_video_struct);
		}
	}
	return ret;
 }


VMINT vm_video_open_buffer(const VMUSTR buffer, const VMUINT buffer_len, vm_video_info_struct *info)
{
	VMINT ret=VM_VIDEO_FAILED;	

	if ((NULL==buffer)  || (NULL==info)|| (vm_video_res_preprocess()==VM_VIDEO_FAILED))
		return	VM_VIDEO_FAILED;	

	if (vm_res_get_data_list_count_by_type(VM_RES_TYPE_VIDEO_HANDLE)==0)
	{
	
		if (s_video_struct->vm_video_status==VM_VIDEO_EXIT)
		{			
			if ((ret = vm_video_sal_open_buffer(buffer, buffer_len,info))==VM_VIDEO_SUCCESS)
			{
				if (s_video_struct->data_handle<0) 
				{
					s_video_struct->processid=vm_pmng_get_current_handle();
					s_video_struct->data_handle=vm_res_save_data(VM_RES_TYPE_VIDEO_HANDLE, (void *)s_video_struct,sizeof(video_context_t), NULL,s_video_struct->processid);
				}
				
				s_video_struct->vm_video_open_type=VM_VIDEO_OPEN_BUFFER;
				s_video_struct->vm_video_status=VM_VIDEO_IDLE;
				return VM_VIDEO_SUCCESS;	
				
			}
		}
	}
	return ret;
}
                    

VMINT vm_video_close_buffer(void)
{
 	VMINT ret=VM_VIDEO_FAILED;

	if (vm_video_interface_preprocess(FALSE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;

	if ((s_video_struct->vm_video_status==VM_VIDEO_IDLE ) && (s_video_struct->vm_video_open_type==VM_VIDEO_OPEN_BUFFER) )
	{
		ret=mdi_video_ply_close_clip_buffer();

		if (ret==VM_VIDEO_SUCCESS)
		{
			s_video_struct->vm_video_status=VM_VIDEO_EXIT;
			vm_res_release_data(VM_RES_TYPE_VIDEO_HANDLE, s_video_struct->data_handle);
			vm_video_reset_context(s_video_struct);
		}
	}
	return ret;
}

VMINT vm_video_play(VMUINT player_layer_handle,VMUINT base_layer_handle,
                       VMUINT16 repeat_count, VMINT is_play_audio, VMINT audio_path, 
                       vm_video_finish_callback play_finish_callback)
{
	VMINT ret=VM_VIDEO_ERR_PLAY_FAILED;	

	if (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED)
		return VM_VIDEO_ERR_PLAY_FAILED;

	if (s_video_struct->vm_video_status==VM_VIDEO_IDLE)
	{
 		ret=vm_video_sal_play(player_layer_handle,base_layer_handle,repeat_count,
			                  TRUE,is_play_audio,audio_path,VM_VIDEO_LCD_ROTATE_0,
			                  VM_VIDEO_SPEED_DEFAULT, vm_video_play_finish_callback);

		if (ret==VM_VIDEO_SUCCESS)
		{
			s_video_struct->player_layer_handle=player_layer_handle;
			s_video_struct->base_layer_handle=base_layer_handle;
			s_video_struct->repeat_count=repeat_count;
			s_video_struct->is_play_audio=is_play_audio;
			s_video_struct->audio_path=audio_path;
			s_video_struct->vm_video_finish_callback=play_finish_callback;
			s_video_struct->vm_video_status=VM_VIDEO_PLAY;			
		}		
	}

	return ret;
 }

VMINT vm_video_stop(void)
{
 	VMINT ret=VM_VIDEO_FAILED;	

	if (vm_video_interface_preprocess(FALSE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;

	if (s_video_struct->vm_video_status==VM_VIDEO_PLAY || s_video_struct->vm_video_status==VM_VIDEO_PAUSE)
	{
		ret=mdi_video_ply_stop();
		if (ret==VM_VIDEO_SUCCESS)
		{
			s_video_struct->vm_video_status=VM_VIDEO_IDLE;	
		}
	}
	return ret;
}

VMINT vm_video_pause(void)
{
	VMINT ret=VM_VIDEO_FAILED;

	if (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;

	if (s_video_struct->vm_video_status==VM_VIDEO_PLAY)
	{
		ret=mdi_video_ply_stop();
		if (ret==VM_VIDEO_SUCCESS)
		{
			mdi_video_ply_get_cur_play_time(&(s_video_struct->vm_video_cur_time));
			s_video_struct->vm_video_status=VM_VIDEO_PAUSE;
		}
		else
		{
			ret=VM_VIDEO_FAILED; 
	    }
	}

	return ret;
 }

VMINT vm_video_resume(void)
{
	VMINT ret=VM_VIDEO_FAILED;

	if (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;

	if (s_video_struct->vm_video_status==VM_VIDEO_PAUSE)
	{
		if (vm_video_sal_seek_and_getframe(s_video_struct->vm_video_cur_time,s_video_struct->player_layer_handle)==VM_VIDEO_SUCCESS)
		{
			ret=vm_video_sal_play(s_video_struct->player_layer_handle,
				s_video_struct->base_layer_handle,s_video_struct->repeat_count,TRUE,
				s_video_struct->is_play_audio,s_video_struct->audio_path,s_video_struct->rotate,
				VM_VIDEO_SPEED_DEFAULT, vm_video_play_finish_callback);

			if (ret==VM_VIDEO_SUCCESS)
			{
				s_video_struct->vm_video_status=VM_VIDEO_PLAY;			
			}
		}		
	}
	return ret;
 }
 
VMINT vm_video_seek_and_getframe(VMUINT64 time, VMUINT player_layer_handle)
{
 	VMINT ret=VM_VIDEO_FAILED;

	if (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;
	
	ret=vm_video_sal_seek_and_getframe(time,player_layer_handle);
	if (ret==VM_VIDEO_SUCCESS)
		s_video_struct->vm_video_cur_time=time;

	return ret;	
}

VMINT vm_video_snapshot(VMUINT player_layer_handle, VMWSTR file_name)
{   
 	VMINT ret=VM_VIDEO_ERR_SNAPSHOT_FAILED;
	VMBOOL is_pause = FALSE;
	VMUINT gdi_layer_handle=0;

	if ((!file_name) || (vm_wstrlen(file_name)<=0) ||  (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED))
		return VM_VIDEO_ERR_SNAPSHOT_FAILED;

	if (s_video_struct->vm_video_status != VM_VIDEO_PAUSE)
	{
		vm_video_pause();
	    is_pause = TRUE;
	}
	
#ifdef __VDOPLY_FEATURE_SNAPSHOT__
		gdi_layer_handle=vm_graphic_get_trans_handle(player_layer_handle);
	
		if (gdi_layer_handle!=0)
			ret=mdi_video_ply_snapshot(gdi_layer_handle,(PS8)file_name);
		else
			ret = VM_VIDEO_ERR_SNAPSHOT_FAILED;
		
		switch (ret)
		{
			case MDI_RES_VDOPLY_SUCCEED:
				ret=VM_VIDEO_SUCCESS;
				break;
			case MDI_RES_VDOPLY_ERR_SNAPSHOT_DISK_FULL: 
				ret=VM_VIDEO_ERR_SNAPSHOT_DISK_FULL;
				break;
				case MDI_RES_VDOPLY_ERR_SNAPSHOT_WRITE_PROTECTION:
				ret=VM_VIDEO_ERR_SNAPSHOT_WRITE_PROTECTION;
				break;
			default:
				ret=VM_VIDEO_ERR_SNAPSHOT_FAILED;
			}
#endif
		return	ret;  

	if(is_pause)
	{
		vm_video_resume();
	}
	
	return ret;
}


VMINT vm_video_set_brightness(VMUINT16 brightness)
{
	if (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;

	if ((s_video_struct->vm_video_status==VM_VIDEO_IDLE) || (s_video_struct->vm_video_status==VM_VIDEO_PAUSE))
	{
		VMINT ret=VM_VIDEO_FAILED;
	
#ifdef __VDOPLY_FEATURE_VIDEO_ADJUSTMENT__
		VMUINT16 brightvalue;
		switch (brightness)
		{
			case VM_VIDEO_BRIGHTNESS_0:
				brightvalue=MDI_VIDEO_BRIGHTNESS_0;
				break;
			case VM_VIDEO_BRIGHTNESS_N1:
				brightvalue=MDI_VIDEO_BRIGHTNESS_N1;
				break;
			case VM_VIDEO_BRIGHTNESS_N2:
				brightvalue=MDI_VIDEO_BRIGHTNESS_N2;			
				break;
			case VM_VIDEO_BRIGHTNESS_N3:
				brightvalue=MDI_VIDEO_BRIGHTNESS_N3;		
				break;
			case VM_VIDEO_BRIGHTNESS_N4:
				brightvalue=MDI_VIDEO_BRIGHTNESS_N4;
				break;
			case VM_VIDEO_BRIGHTNESS_N5:
				brightvalue=MDI_VIDEO_BRIGHTNESS_N5;
				break;
			case VM_VIDEO_BRIGHTNESS_P1:
				brightvalue=MDI_VIDEO_BRIGHTNESS_P1;
				break;
			case VM_VIDEO_BRIGHTNESS_P2:
				brightvalue=MDI_VIDEO_BRIGHTNESS_P2;			
				break;
			case VM_VIDEO_BRIGHTNESS_P3:
				brightvalue=MDI_VIDEO_BRIGHTNESS_P3;		
				break;
			case VM_VIDEO_BRIGHTNESS_P4:
				brightvalue=MDI_VIDEO_BRIGHTNESS_P4;
				break;
			case VM_VIDEO_BRIGHTNESS_P5:
				brightvalue=MDI_VIDEO_BRIGHTNESS_P5;
				break;
			default:
				brightvalue=MDI_VIDEO_BRIGHTNESS_0;
		}
			
		if (mdi_video_ply_set_brightness(brightvalue)==MDI_RES_VDOPLY_SUCCEED)
			ret=VM_VIDEO_SUCCESS;
		else
			ret=VM_VIDEO_FAILED;
#endif
	
		return ret;
	 }
	else
		return VM_VIDEO_FAILED;
}

VMINT vm_video_set_contrast(VMUINT16 contrast)
{
	VMINT ret=VM_VIDEO_FAILED;

	if (vm_video_interface_preprocess(TRUE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;
	if ((s_video_struct->vm_video_status==VM_VIDEO_IDLE) || (s_video_struct->vm_video_status==VM_VIDEO_PAUSE))
	{
#ifdef __VDOPLY_FEATURE_VIDEO_ADJUSTMENT__
	VMUINT16 contrastvalue;
	switch (contrast)
	{
		case VM_VIDEO_CONTRAST_0:
			contrastvalue=MDI_VIDEO_CONTRAST_0;
			break;
		case VM_VIDEO_CONTRAST_N1:
			contrastvalue=MDI_VIDEO_CONTRAST_N1;
			break;
		case VM_VIDEO_CONTRAST_N2:
			contrastvalue=MDI_VIDEO_CONTRAST_N2;			
			break;
		case VM_VIDEO_CONTRAST_N3:
			contrastvalue=MDI_VIDEO_CONTRAST_N3;		
			break;
		case VM_VIDEO_CONTRAST_N4:
			contrastvalue=MDI_VIDEO_CONTRAST_N4;
			break;
		case VM_VIDEO_CONTRAST_N5:
			contrastvalue=MDI_VIDEO_CONTRAST_N5;
			break;
		case VM_VIDEO_CONTRAST_P1:
			contrastvalue=MDI_VIDEO_CONTRAST_P1;
			break;
		case VM_VIDEO_CONTRAST_P2:
			contrastvalue=MDI_VIDEO_CONTRAST_P2;			
			break;
		case VM_VIDEO_CONTRAST_P3:
			contrastvalue=MDI_VIDEO_CONTRAST_P3;		
			break;
		case VM_VIDEO_CONTRAST_P4:
			contrastvalue=MDI_VIDEO_CONTRAST_P4;
			break;
		case VM_VIDEO_CONTRAST_P5:
			contrastvalue=MDI_VIDEO_CONTRAST_P5;
			break;
		default:
			contrastvalue=MDI_VIDEO_CONTRAST_0;
	}
		
	if (mdi_video_ply_set_contrast(contrastvalue)==MDI_RES_VDOPLY_SUCCEED)
		ret=VM_VIDEO_SUCCESS;
	else
		ret=VM_VIDEO_FAILED;
#endif

	return ret;
	}
	else
		return VM_VIDEO_FAILED;
 }

VMINT vm_video_get_cur_play_time(VMUINT64 *cur_play_time)
{
	if (cur_play_time == NULL || vm_video_interface_preprocess(FALSE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;
	
 	mdi_video_ply_get_cur_play_time(cur_play_time);
	return VM_VIDEO_SUCCESS;
}
 

VMINT vm_video_play_stream_from_rtsp_link(VMWSTR url)
{
	 char g_pUrlUTF8[260] = {0};

	if (url == NULL || wstrlen(url) < 1 || vm_video_interface_preprocess(FALSE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;

#if (defined __COSMOS_MMI_PACKAGE__ && defined __MMI_URI_AGENT__)
	memset(g_pUrlUTF8,0,sizeof(g_pUrlUTF8));
    mmi_chset_convert(MMI_CHSET_UCS2, MMI_CHSET_UTF8, (char*)url, (char*)g_pUrlUTF8, sizeof(g_pUrlUTF8));
	srv_uriagent_dispatch_uri_request(SRV_URIAGENT_APPID_STREAMING,g_pUrlUTF8,SRV_URIAGENT_OPTION_UNSPECIFIED,NULL);
    return VM_VIDEO_SUCCESS;
#endif
	  
#if (defined __MMI_VIDEO_STREAM__ && defined __MMI_VIDEO_PLAYER__)
	mmi_vdoply_entry_player_screen_from_rtsp_link((PS8)url);
	return VM_VIDEO_SUCCESS;
#elif defined(__MMI_MEDIA_PLAYER_STREAM__)
	mmi_medply_single_play_stream_from_rtsp_link((UI_string_type)url);
	return VM_VIDEO_SUCCESS;
#else
	return VM_VIDEO_FAILED;
#endif
}


VMINT vm_video_play_stream_from_sdp_file(VMWSTR filename, VMUCHAR is_short)
{
	if (filename == NULL || wstrlen(filename) < 1 || vm_video_interface_preprocess(FALSE)==VM_VIDEO_FAILED)
		return VM_VIDEO_FAILED;
	
#if (defined __MMI_VIDEO_STREAM__ && defined __MMI_VIDEO_PLAYER__)
	mmi_vdoply_entry_player_screen_from_sdp_file((PS8)filename, (BOOL)is_short);
	return VM_VIDEO_SUCCESS;
#elif defined(__MMI_MEDIA_PLAYER_STREAM__)
	mmi_medply_single_play_stream_from_sdp_file((UI_string_type)filename, (MMI_BOOL)is_short);
	return VM_VIDEO_SUCCESS;
#else
	return VM_VIDEO_FAILED;
#endif
}

VMINT vm_video_play_by_filepath(VMWSTR filename)
{
    filetypes_file_type_enum filetype;
    S32 opt_menu = 0;
    S32 opt_menu_count = 0;
    S32 fs_ret = 0;
    S32 child_id = 0;

    MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_S, 1, __LINE__);
    if (NULL == filename)
    {
        MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_E1, 1, __LINE__);
        return -2;
    }

    filetype = (filetypes_file_type_enum)srv_fmgr_types_find_type_by_filepath((const WCHAR *)filename);
    if (0 > filetype)
    {
        MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_E2, 1, __LINE__);
        return -3;
    }
    
    opt_menu = srv_fmgr_types_get_option_menu(filetype, NULL, 0);
    if (0 >= opt_menu)
    {
        MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_E3, 1, opt_menu);
        return -4;
    }

    opt_menu_count = GetNumOfChild(opt_menu);
    if (0 == opt_menu_count)
    {
        MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_E4, 1, opt_menu_count);
        return -5;
    }
    
    child_id = GetSeqItemId(opt_menu, 0);
    fs_ret = srv_fmgr_types_launch_option(filename, child_id);
    if (0 >= fs_ret)
    {
        MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_E5, 1, fs_ret);
        return -6;
    }
    

    MMI_TRACE(TRACE_GROUP_7, TRC_MRE_VPF_E, 1, __LINE__);
    return 0; 
}

static VMINT vm_video_get_layer_flag(VMUINT blthanlde,VMUINT playhandle,VMUINT * gdi_play_layer_handle,VMUINT *gdi_base_layer_handle, VMUINT *bltflag,VMUINT *playflag)
{

	VMUINT i=0;
	VMUINT gdi_handle[4]={0};
	VMUINT flag[4]={GDI_LAYER_ENABLE_LAYER_0,GDI_LAYER_ENABLE_LAYER_1,GDI_LAYER_ENABLE_LAYER_2,GDI_LAYER_ENABLE_LAYER_3};

	if (!gdi_play_layer_handle || !gdi_base_layer_handle || !bltflag || !playflag)
		return FALSE;

	*gdi_play_layer_handle=vm_graphic_get_trans_handle(playhandle);
	*gdi_base_layer_handle=vm_graphic_get_trans_handle(blthanlde);

	if (!(*gdi_play_layer_handle) ||  !(*gdi_base_layer_handle))
		return FALSE;

	gdi_layer_get_blt_layer(&gdi_handle[0],&gdi_handle[1],&gdi_handle[2],&gdi_handle[3]);
	for(i=0;i<4;i++)
	{
		if(gdi_handle[i]==(*gdi_play_layer_handle)) 
		{
			(*bltflag)|=flag[i];
			(*playflag)|=flag[i];
			break;
		}
		if(gdi_handle[i]==(*gdi_base_layer_handle))
			(*bltflag)|=flag[i];
	}

	if (*playflag == 0)
	{
		for(i=0;i<4;i++)
		{
			if(gdi_handle[i] == 0)
			{
				gdi_handle[i] = *gdi_play_layer_handle;
				gdi_layer_set_blt_layer(gdi_handle[0],gdi_handle[1],gdi_handle[2],gdi_handle[3]);
				(*bltflag)|=flag[i];
				(*playflag)|=flag[i];
				break;
			}
		}
	}

	return TRUE;
}


static void vm_video_get_info_values(vm_video_info_struct *toinfo,const mdi_video_info_struct *frominfo)
{
	if (toinfo && frominfo)
	{
		toinfo->width=frominfo->width;
		toinfo->height=frominfo->height;
		toinfo->total_frame_count=frominfo->total_frame_count;
		toinfo->total_time_duration=frominfo->total_time_duration;
		toinfo->track=(vm_video_track_enum)frominfo->track;
		toinfo->is_seekable=frominfo->is_seekable;
		toinfo->aud_channel_no=frominfo->aud_channel_no;
		toinfo->aud_sample_rate=frominfo->aud_sample_rate;
		toinfo->is_drm_streaming=frominfo->is_drm_streaming;
		toinfo->drm_handle=frominfo->drm_handle;
		
		vm_wstrncpy((VMWSTR)toinfo->title_desc, (VMWSTR)frominfo->title_desc,VM_VIDEO_MAX_CONTENT_INFO_LEN);
		vm_wstrncpy((VMWSTR)toinfo->artist_desc, (VMWSTR)frominfo->artist_desc,VM_VIDEO_MAX_CONTENT_INFO_LEN);
		vm_wstrncpy((VMWSTR)toinfo->album_desc, (VMWSTR)frominfo->album_desc,VM_VIDEO_MAX_CONTENT_INFO_LEN);
		vm_wstrncpy((VMWSTR)toinfo->author_desc, (VMWSTR)frominfo->author_desc,VM_VIDEO_MAX_CONTENT_INFO_LEN);
		vm_wstrncpy((VMWSTR)toinfo->copyright_desc, (VMWSTR)frominfo->copyright_desc,VM_VIDEO_MAX_CONTENT_INFO_LEN);
		vm_wstrncpy((VMWSTR)toinfo->date_desc, (VMWSTR)frominfo->date_desc,VM_VIDEO_MAX_CONTENT_INFO_LEN);

	}

}


static void vm_video_sal_open_result_callback( MDI_RESULT ret, mdi_video_info_struct *video_info, void *user_data) 
{

	if (mre_running == FALSE)
	   return;

	if (ret>=0)
	{
        memset(&vm_video_info,0x00,sizeof(vm_video_info_struct));
		vm_video_get_info_values(&vm_video_info,video_info);
	}

	if (s_video_struct && (s_video_struct->vm_video_open_result_callback))
	{
		if (vm_pmng_set_ctx(s_video_struct->processid) == VM_PMNG_OP_OK)
		{
			s_video_struct->vm_video_open_result_callback(ret,&vm_video_info);
			vm_pmng_reset_ctx();
		}
	}
}

VMINT vm_video_sal_open_buffer( const VMUSTR buffer, const VMUINT buffer_len, vm_video_info_struct *info)
 {
 	
    VMINT ret=VM_VIDEO_FAILED;
	mdi_video_info_struct video_info;

	if ((NULL==info) || (NULL==buffer) )
		return VM_VIDEO_FAILED;

	memset(&video_info,0x00,sizeof(mdi_video_info_struct));
	
	ret=mdi_video_ply_open_clip_buffer(ASM_ANONYMOUS_ID, buffer, buffer_len, &video_info);
	switch (ret)
	{
		case MDI_RES_VDOPLY_SUCCEED:
			ret=VM_VIDEO_SUCCESS;
			vm_video_get_info_values(&vm_video_info,&video_info);
			break;
		case MDI_RES_VDOPLY_ERR_MEMORY_INSUFFICIENT:	
			ret=VM_VIDEO_ERR_MEMORY_INSUFFICIENT;
			break;
        	case MDI_RES_VDOPLY_ERR_FRAMERATE_TOO_HIGH:
			ret=VM_VIDEO_ERR_FRAMERATE_TOO_HIGH;
			break;
		default:
			ret=VM_VIDEO_FAILED;
        }
	return	ret; 
 }
                    

 VMINT vm_video_sal_play( VMUINT player_layer_handle,VMUINT base_layer_handle,VMUINT16 repeat_count,  VMINT is_visual_update,
                    VMINT is_play_audio, VMINT audio_path, VMUINT16 rotate, VMUINT16 play_speed,mdi_play_finish_callback play_finish_callback)
{
 	VMINT ret=VM_VIDEO_ERR_PLAY_FAILED;
	VMUINT blt_layer_flag=0,play_layer_flag=0,gdi_base_layer_handle=0,gdi_play_layer_handle=0; 
	VMINT mdi_audio_path=MDI_DEVICE_SPEAKER2;

	if (!vm_video_get_layer_flag(base_layer_handle,player_layer_handle,&gdi_play_layer_handle,&gdi_base_layer_handle,&blt_layer_flag,&play_layer_flag))
		return VM_VIDEO_ERR_PLAY_FAILED;

	switch (audio_path)
	{
		case VM_DEVICE_SPEAKER:
			mdi_audio_path=MDI_DEVICE_SPEAKER;
			break;
		case VM_DEVICE_MICROPHONE:
			mdi_audio_path=MDI_DEVICE_MICROPHONE;
			break;
		case VM_DEVICE_SPEAKER2:
			mdi_audio_path=MDI_DEVICE_SPEAKER2;
			break;
		case VM_DEVICE_LOUDSPEAKER:
			mdi_audio_path=MDI_DEVICE_LOUDSPEAKER;
			break;
		case VM_DEVICE_SPEAKER_BOTH:
			mdi_audio_path=MDI_DEVICE_SPEAKER_BOTH;
			break;
		case VM_DEVICE_BT_HEADSET:
			mdi_audio_path=MDI_DEVICE_BT_HEADSET;
			break;	
		default:
			mdi_audio_path=MDI_DEVICE_SPEAKER2;
	}
	
	ret=mdi_video_ply_play((gdi_handle)gdi_play_layer_handle,blt_layer_flag,play_layer_flag,
		repeat_count,TRUE,(BOOL)is_play_audio,(U8)mdi_audio_path,MDI_LCD_ROTATE_0, VM_VIDEO_SPEED_DEFAULT, play_finish_callback, 0);
	
	switch (ret)
	{
		case MDI_RES_VDOPLY_SUCCEED:
			ret=VM_VIDEO_SUCCESS;
			break;
		case MDI_RES_VDOPLY_ERR_DRM_PROHIBITED:	
			ret=VM_VIDEO_ERR_DRM_PROHIBITED;
			break;
        	case MDI_RES_VDOPLY_PROGRESSIVE_FILE_NOT_ENOUGH:
			ret=VM_VIDEO_PROGRESSIVE_FILE_NOT_ENOUGH;
			break;
        	case MDI_RES_VDOPLY_ERR_INVALID_RESOULTION:
			ret=VM_VIDEO_ERR_INVALID_RESOULTION;
			break;
		default:
			ret=VM_VIDEO_ERR_PLAY_FAILED;		
    }
	return	ret;     
 }


 VMINT vm_video_sal_seek_and_getframe(VMUINT64 time, VMUINT player_layer_handle)
 {
	VMINT ret=VM_VIDEO_FAILED;
	VMUINT gdi_player_layer_handle=0;
	gdi_player_layer_handle=vm_graphic_get_trans_handle(player_layer_handle);
	 if ((gdi_player_layer_handle!=0) && (mdi_video_ply_seek_and_get_frame(time,gdi_player_layer_handle)>=MDI_RES_VDOPLY_SUCCEED))
		ret=VM_VIDEO_SUCCESS;
	else
		ret=VM_VIDEO_FAILED;

	return ret;
 }

#endif

VMBOOL vm_video_is_support_rtsp_link_play(void)
{
#ifdef __MRE_SAL_VIDEO__

#if (defined __COSMOS_MMI_PACKAGE__ && defined __MMI_URI_AGENT__)
    return TRUE;
#endif
  
#if (defined __MMI_VIDEO_STREAM__ && defined __MMI_VIDEO_PLAYER__)
    return TRUE;
#elif defined(__MMI_MEDIA_PLAYER_STREAM__)
    return TRUE;
#else
    return FALSE;
#endif

#else
	return FALSE;
#endif

}

VMBOOL vm_video_is_support_sdp_file_play(void)
{
#ifdef __MRE_SAL_VIDEO__

#if (defined __MMI_VIDEO_STREAM__ && defined __MMI_VIDEO_PLAYER__)
	return TRUE;
#elif defined(__MMI_MEDIA_PLAYER_STREAM__)
	return TRUE;
#else
	return FALSE;
#endif

#else
	return FALSE;
#endif
 }
 
