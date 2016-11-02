/**
*
*
* Video stuff
*
**/

/** 
* Header Files
**/
//#include "share.h"

/**
* Global Variables
**/

VMINT video_playing; //Is Video currently playing ?

/*
 * prototype of user defined functions
 */
/* -------------------------------------------------------------------------*/

/*****************************************************************************
 * FUNCTION
 *  play_video
 * DESCRIPTION
 *  This function starts the stuff needed to play the video
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
void play_video(void);

/*****************************************************************************
 * FUNCTION
 *  play_video_native_player
 * DESCRIPTION
 *  This function starts the stuff needed to play the video in the native 
 *  player
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
void play_video_native_player(VMSTR filename);

void video_open_callback(VMINT ret, vm_video_info_struct *video_info);

vm_video_open_result_callback video_open_callback2( VMINT ret, vm_video_info_struct *video_info);

vm_video_open_result_callback open_video_callback(void);

vm_video_finish_callback play_video_callback(void);
