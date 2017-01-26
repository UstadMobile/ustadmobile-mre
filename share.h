/*
share.h Shared global variables
*/

/////////////////////////////IO and App Definitions///////////////////////////
#define MRE_LOG_ON
#define MRE_SPACE_BTW_LN            (10)
#define MRE_STR_SIZE_MAX            (100)
#define MRE_SET_X                   (0)
#define MRE_SET_Y                   (0)
#define MRE_SET_SRC_LEFT_TOP_X      (0)
#define MRE_SET_SRC_LEFT_TOP_Y      (0)
#define MRE_SET_VAL                 (0)
#define MRE_SET_MARGIN              (10)
#define MRE_FILE_CREATE_FAILED  	(-5)
#define MRE_TEXT_BOX_FAILED	        (-2)
#define MRE_FILE_OPEN_FAILED        (-3)
#define MRE_FILE_NAME_SIZE			(50)
#define MRE_FILE_NOT_WRITTEN        (-4)
#define MRE_TEXTBOX_CANCEL          (2)
#define MRE_ERROR                   (-1)
#define MRE_SCILENT                 (100)
#define MRE_GET_FILE_TIME_FAILED    (-6)
#define	SUPPORT_BG
#define MRE_DEF_STRING_SIZE	        (30)

#define	SUPPORT_BG
//#define MRE_LOG_ON
/*Constant value for displaying at screen */
#define      BOTTOM_COLOUMN_HEIGHT 20
/*Constant value for displaying at screen */
#define      BOTTOM_ROW_BOUNDARY_VALUE 3
/*Constant value for ZERO*/
//#define MRE_SCREEN_START_X         0
//#define MRE_SCREEN_START_Y         0
#define MRE_MAX_SIZE_STRING        150
#define MRE_DEFAULT_STRING_SIZE    150
#define MRE_BOTH_LAYERS            2
#define MRE_NOT_LAYER_CREATED     -1
#define MRE_TCP_CONNECTION_PORT    80
//#define MRE_STRING_MAX_SIZE        100
#define MRE_STRING_INTIAL_VALUE    0
#define MRE_TCP_BUFFER_MAX_SIZE    500
#define MRE_FILE_NAME_MAX_SIZE     30
#define MRE_TWO_TIMES              2
#define MRE_HALF_FACTOR            2
#define MRE_X_BOUNDARY_WIDTH       20
#define MRE_DISPLAY_STRING_ROW_HEIGHT      20
//#define MRE_NO_TRANS_COLOR        -1
#define MRE_SINGLE_LAYER            1

#define     MRE_SCREEN_START_X    (0)
#define     MRE_SCREEN_START_Y    (0)
//#define		SUPPORT_BG
#define     MRE_INTIAL_VALUE      (0)
#define     MRE_FIRST_LAYER       (1)
#define     MRE_HTTP_URL          "http://www.google.com"
#define     MRE_NO_LAYER          (-1)
#define     MRE_NO_TRANS_COLOR    (-1)
#define     MRE_FILE_NAME_SIZE    (50)
#define     MRE_STRING_MAX_SIZE   (50)
#define		MRE_MARGIN			  (20)

#define VM_NO_TRANSPARENT_COLOR (-1)

/* Image related */
#define MRE_GRAPHIC_IMAGE_EXPAND (1)
#define MRE_GRAPHIC_IMAGE_SHRINK (2)
#define MRE_GRAPHIC_IMAGE_CURRENT (3)
#define MRE_GRAPHIC_IMAGE_ROTATE_CW (4)
#define MRE_GRAPHIC_IMAGE_ROTATE_AW (5)
#define MRE_GRAPHIC_IMAGE_MIRROR (6)
#define MRE_GRAPHIC_IMG_MIR_DIR_X (0)
#define MRE_GRAPHIC_IMG_MIR_DIR_Y (1)
#define MRE_IMG_JPG (0)
#define MRE_IMG_GIF (1)
#define MRE_SET_SRC_LEFT_TOP_X (0)
#define MRE_SET_SRC_LEFT_TOP_Y (0)
#define MRE_IMG_MIN_SIZE (0)
#define MRE_IMG_MAX_SIZE (300)
#define MRE_IMG_ANI_RATE (100)
#define MRE_IMG_EXP_RATE (5) 
#define MRE_BG_TRANS_COLOR (-1)
#define MRE_POS_CEN_X (100)
#define MRE_POS_CEN_Y (150)
#define MRE_IMG_ANI_FIRST_FRAME (1)
#define MRE_SET_IMG_ROT (90)
#define MRE_WHOLE_ROT (360)
//#define MRE_SPACE_BTW_LN (6) //Already defined above

/////////////////////////////NUKLEAR SPECIFIC STUFF///////////////////////////
/* Preprocessor macros */
#define WINDOW_WIDTH 240
#define WINDOW_HEIGHT 320
#define NK_PRIVATE	//defines all functions as static 
					//so they only are accessed by the 
					//file that creates the implementation
#define NK_ASSERT //If not set, will use assert.h which is Standard Library - MRE doesn't like.
//So we don't want it, so we defined it to nothing. 
//#define NK_INCLUDE_FIXED_TYPES - not gonna work-it will look and use stdint.h - not present in MRE
//#define NK_INCLUDE_DEFAULT_ALLOCATOR //able to call nk_init_default(&mre.ctx, &font); BUT it will use STL -Nope
//#define NK_INCLUDE_STANDARD_IO - not gonna work-looks at stdio.h - not present in MRE-actually it is-BUT uses STL
#define NK_UINT32
/* Latest way: */
#define NK_IMPLEMENTATION
#define NK_MRE_IMPLEMENTATION
/////////////////////////////END NUKLEAR SPECIFIC///////////////////////////

/* included files */
#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "string.h"
#include "stdio.h"
#include "vmlog.h"
#include "vmmullang.h"
#include "vmsock.h"
#include "vmhttp.h" 
//NOTE WE CHANGED vmhttp.h file's http_head_t struct method to make 
//name more than 20 characters (to 100). This wont change device
//as vmhttp.h is part of the MRE OS/SDK. We will need to figure a 
//workaround at either server side or app side.
#include "vmlog.h"
#include "vmvideo.h"
#include "vmres.h"
#include "vmtimer.h"
#include "vmxml.h"

/* Global Variables */
//I think its a better idea to make a seperate header file for global variables..

VMINT       connect_status; //Don't give it a value over here..
VMINT		layer_handle[1];				////layer handle array.
VMINT g_mre_httpcontext; //Http Menu Context
VMINT g_mre_socketcontext; //Socket Menu Context

VMINT	layer_hdl[1];			//layer handle array.
VMINT	g_mre_curr_x;
VMINT	g_mre_curr_y;
VMWSTR	g_mre_textbox_text;
VMINT	g_mre_textbox_state;
VMINT	g_mre_subcontext;
VMINT	g_mre_drv;
VMINT	g_mre_fileiocontext;	//In file io context

/* image related */
VMINT g_mre_img_y_pos;
VMINT g_mre_img_x_pos;
VMINT g_mre_img_size_cng; 
VMINT g_mre_img_clockwise_cnt;
VMINT g_mre_img_dir;
VMINT g_mre_curr_x;
VMINT g_mre_curr_y;

/* Declaration */
void mre_tcp_callback_application(VMINT handle, VMINT event);
/*****************************************************************************
 * FUNCTION
 *  mre_start_menu
 * DESCRIPTION
 *  This function displays menu list on the screen for selection 
 * PARAMETERS
 *  none
 * RETURNS
 *	none
 TODO: Move to MRE_API_Tests.h 
*****************************************************************************/
void mre_start_menu(void);

