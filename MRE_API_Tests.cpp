/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"
/* ---------------------------------------------------------------------------
* global variables
* ------------------------------------------------------------------------ */

/* define this macro if application support background running. */
#define		SUPPORT_BG		


VMINT		layer_hdl[1];	/* layer handle array. */

/* ---------------------------------------------------------------------------
 * local variables
 * ------------------------------------------------------------------------ */
/*
* system events 
*/
void handle_sysevt(VMINT message, VMINT param);

/*
* key events 
*/
void handle_keyevt(VMINT event, VMINT keycode);

/*
* pen events
*/
void handle_penevt(VMINT event, VMINT x, VMINT y);

/*
* demo
*/
static void draw_hello(void);

/*
* entry
*/
void vm_main(void) {
	/* initialize layer handle */
	layer_hdl[0] = -1;	
	
	/* register system events handler */
	vm_reg_sysevt_callback(handle_sysevt);
	
	/* register keyboard events handler */
	vm_reg_keyboard_callback(handle_keyevt);
	
	/* register pen events handler */
	vm_reg_pen_callback(handle_penevt);

	/* Init MRE resource */
	vm_res_init();
}

void handle_sysevt(VMINT message, VMINT param) {
#ifdef		SUPPORT_BG	
/* The application updates the screen when receiving the message VM_MSG_PAINT 
*  what is sent after the application is activated. The application can skip 
*  the process on screen when the VM_MSG_ACTIVE or VM_MSG_INACTIVE is received.
	*/
	switch (message) {
	case VM_MSG_CREATE:	/* the message of creation of application */
		/* the GDI operation is not recommended as the response of the message*/
		break;
	case VM_MSG_PAINT:	/* the message of asking for application to repaint screen */
		/* cerate base layer that has same size as the screen*/
		layer_hdl[0] = vm_graphic_create_layer(0, 0, 
			vm_graphic_get_screen_width(),		/* get screen width */
			vm_graphic_get_screen_height(),		/* get screen height */
			-1);		/* -1 means layer or canvas have no transparent color */
		
		/* set clip area */
		vm_graphic_set_clip(0, 0, 
			vm_graphic_get_screen_width(), 
			vm_graphic_get_screen_height());
		
		draw_hello();	/* draw hello world! */
		break;
	case VM_MSG_HIDE:	
		/* message of application state from foreground running to background running */
		if( layer_hdl[0] != -1 )
		{
			vm_graphic_delete_layer(layer_hdl[0]);
			layer_hdl[0] = -1;
		}
		break;
	case VM_MSG_QUIT:	/* the message of quit of application */
		if( layer_hdl[0] != -1 )
		{
			vm_graphic_delete_layer(layer_hdl[0]);
			layer_hdl[0] = -1;
		}
		
		/* Release all resource */
		vm_res_deinit();

		break;
	}
#else
	switch (message) {
	case VM_MSG_CREATE:	/* the message of creation of application */
	case VM_MSG_ACTIVE: /* the message of application state from inactive to active */
		/*cerate base layer that has same size as the screen*/
		layer_hdl[0] = vm_graphic_create_layer(0, 0, 
			vm_graphic_get_screen_width(),		/* get screen width */
			vm_graphic_get_screen_height(),		/* get screen height */
			-1);		/* -1 means layer or canvas have no transparent color */
		
		/* set clip area*/
		vm_graphic_set_clip(0, 0, 
			vm_graphic_get_screen_width(), 
			vm_graphic_get_screen_height());
		break;
		
	case VM_MSG_PAINT:	/* the message of asking for application to repaint screen */
		draw_hello();	/* draw hello world! */
		break;
		
	case VM_MSG_INACTIVE:	/* the message of application state from active to inactive */
		if( layer_hdl[0] != -1 )
			vm_graphic_delete_layer(layer_hdl[0]);
		
		break;	
	case VM_MSG_QUIT:		/* the message of quit application */
		if( layer_hdl[0] != -1 )
			vm_graphic_delete_layer(layer_hdl[0]);
		
		/* Release all resource */
		vm_res_deinit();
		break;	
	}
#endif
}

void handle_keyevt(VMINT event, VMINT keycode) {
	/* press any key and return*/
	if( layer_hdl[0] != -1 )
	{
		vm_graphic_delete_layer(layer_hdl[0]);
		layer_hdl[0] = -1;
	}
	
	vm_exit_app();		/* quit application */
}

void handle_penevt(VMINT event, VMINT x, VMINT y)
{
	/* touch and return*/
	if( layer_hdl[0] != -1 )
	{
		vm_graphic_delete_layer(layer_hdl[0]);
		layer_hdl[0] = -1;
	}
	
	vm_exit_app();		/* quit application */
}

static void draw_hello(void) {

	VMWSTR s;					/* string's buffer */
	int x;						/* string's x coordinate */
	int y;						/* string's y coordinate */
	int wstr_len;				/* string's length */
	vm_graphic_color color;		/* use to set screen and text color */

	
	/* get string content from resource */
	s = (VMWSTR)vm_res_get_string(STR_ID_HELLO);
	
	/* calculate string length*/ 
	wstr_len = vm_graphic_get_string_width(s);

	/* calculate string's coordinate */
	x = (vm_graphic_get_screen_width() - wstr_len) / 2;
	y = (vm_graphic_get_screen_height() - vm_graphic_get_character_height()) / 2;
	
	/* set screen color */
	color.vm_color_565 = VM_COLOR_WHITE;
	vm_graphic_setcolor(&color);
	
	/* fill rect with screen color */
	vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
	
	/* set text color */
	color.vm_color_565 = VM_COLOR_BLUE;
	vm_graphic_setcolor(&color);
	
	/* output text to layer */
	vm_graphic_textout_to_layer(layer_hdl[0],x, y, s, wstr_len);
	
	/* flush the screen with text data */
	vm_graphic_flush_layer(layer_hdl, 1);
}

