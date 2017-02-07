/*
Video stuff
*/


VMINT video_playing; //Is Video currently playing ?


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

/*****************************************************************************
 * FUNCTION
 *  video_open_callback
 * DESCRIPTION
 *  This function is the callback to video file open 
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
void video_open_callback(VMINT ret, vm_video_info_struct *video_info);

/*****************************************************************************
 * FUNCTION
 *  video_open_callback_get_thumbnail
 * DESCRIPTION
 *  This function is the callback to video open get thumbnail (?)
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
void video_open_callback_get_thumbnail(VMINT ret, vm_video_info_struct *video_info);

/*****************************************************************************
 * FUNCTION
 *  video_open_callback2
 * DESCRIPTION
 *  This function is another callback
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
vm_video_open_result_callback video_open_callback2( VMINT ret, vm_video_info_struct *video_info);

/*****************************************************************************
 * FUNCTION
 *  open_video_callback
 * DESCRIPTION
 *  This function is another callback ?
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
vm_video_open_result_callback open_video_callback(void);

/*****************************************************************************
 * FUNCTION
 *  play_video_callback
 * DESCRIPTION
 *  This function is the callback for when the video has finished playing 
 * PARAMETERS
 *  void			  
 * RETURNS
 *	none
*****************************************************************************/
vm_video_finish_callback play_video_callback(void);
