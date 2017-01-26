/*
mre_views.c The logic for the views related to MRE using Nuklear 
*/

#include "share.h"
#include "mre_views.h"

/*****************************************************************************
 * FUNCTION
 *  mre_set_graphic_global_data
 * DESCRIPTION
 *  This function sets global data variables
 *  handling actions
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void mre_set_graphic_global_data(void)
{
	/* function body */
    vm_log_file("mre_set_graphic_global_data function starts..\n");
    mre_set_curr_x(MRE_SET_X);
    mre_set_curr_y(MRE_SET_Y);
    vm_font_set_font_size(VM_SMALL_FONT);
    /*log information*/
    vm_log_file("mre_set_global_data done.\n");
}

/* Go to next page placeholder */
void goto_next_page(){

}

/* Go to previous page placeholder */
void goto_previous_page(){

}


/*****************************************************************************
 * FUNCTION
 *  mre_show_image
 * DESCRIPTION
 *  Image is placed in BLUE layer_buffer with effective changes made in this function 
 * PARAMETERS
 *  size_ctrl				[IN]      controls size of image
 * RETURNS
 *	none
*****************************************************************************/
void mre_show_image(VMINT size_ctrl, VMWSTR f_wname, VMSTR file_name, VMINT *mre_layer_hdl, short x, short y)
{
	/* local variables */
	VMUINT8 *res=NULL;
	VMUINT8 *imgres = NULL;
	VMUINT8 *target_layer_buf;
	VMINT img; //the canvas (image)
	VMINT res_size;
	VMINT imgres_size;
	VMUINT8 *canvas_buffer = NULL;
	struct frame_prop* image_prop = NULL;
	
	/* File name related variables */
	VMWSTR wfilename;
	VMINT wfilename_size;
	VMSTR filename;
	VMSTR filename_vm;
	VMCHAR f_name[MRE_STR_SIZE_MAX + 1];	//Old usage for video filename string
	//VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1]; //Old usage for video filename Part of the argument now

	//VMSTR file_name = "tips.gif"; // Part of argument now
	VMFILE file_handle;
	VMUINT nread;
	VMINT file_read_ret; //file read return value

	/* VMCHAR is the same as VMUINT8 which is the same as unsigned char */
	VMCHAR *data;
	 
	/* Getting em strings ready */
	/*
		//Gotta allocate VMSTR before we do anything to it!
		//filename_vm = malloc(MRE_STR_SIZE_MAX); //Testing
		//filename = (VMSTR)vm_malloc(MRE_STR_SIZE_MAX); //Testing
		filename = malloc(MRE_STR_SIZE_MAX);
		sprintf (filename, "%c:\\%s", vm_get_removable_driver(), file_name);
		//Gotta allocate VMWSTR before we do anything to it!
		//wfilename_size = (strlen(filename) + 1) * 2;
		wfilename = vm_malloc(wfilename_size);
		wfilename = malloc(wfilename_size);
		sprintf(f_name, "%c:\\%s", vm_get_removable_driver(), file_name);
	*/

	/* String format conversion */
	/*
		vm_ascii_to_ucs2 (wfilename, MRE_STR_SIZE_MAX, filename);
		vm_ascii_to_ucs2(f_wname, MRE_STR_SIZE_MAX, f_name);
	*/

	/* Checking: Test that the file exists, we have access to it */
		/*
		if (does_this_file_exist(f_name) == FALSE){
			vm_exit_app();
		}
		if (does_this_file_exist(filename) == FALSE){
			vm_exit_app();
		}
		if (does_this_wfile_exist(f_wname) == FALSE){
			vm_exit_app();
		}
		if (does_this_wfile_exist(wfilename) == FALSE){
			vm_exit_app();
		}
		*/ 
	
	/* If using vm4res, we need to initialise the resource. 
		We don't use resouces */ 
		//vm_res_init();

	/* Steps according to MRE Developer Guide 
		7.2.4 How do I draw an image on the screen?  Follow these steps to draw an image:  
			1. Use the vm_load_resource or the vm_file_read interface to load image data into memory;  
			2. Use the vm_graphic_load_image interface to decode the image onto a canvas; */
		//1. Load image data into memory, read the image file:
		file_handle = vm_file_open(f_wname, MODE_WRITE, TRUE); //Mode as Binary cuz image, right?
		vm_file_getfilesize(file_handle, &imgres_size);
		/* Allocate mem for image resource. calloc 0 initialises, malloc: leaves uninitialised */
		data = vm_malloc(imgres_size + 1);
		//data = malloc(imgres_size + 1);
		file_read_ret = vm_file_read(file_handle, data, imgres_size+1, &nread);
		if (file_read_ret == 0){
			printf("\n File Read Error ! \n");
			return;
		}
		imgres = &data[0];
		vm_file_close(file_handle); //Don't need the file no more. Closing it.
		
	
	/*	vm4res.h way 1: */
	/*	
		// To use MRE 2.0 resources, we should start with this..
		vm_res_init();
		IMAGE_1 = 1;
		ret = 0;
		//VMINT vm_res_get_image_and_size_from_file(VMWSTR filename, VMUINT32 image_id, void* buff, VMUINT32 *size);
		//result = vm_res_get_image_and_size_from_file("E:\\sample.vxp", IMAGE_1,NULL, &size);
		  
	
		ret = vm_res_get_image_and_size_from_file(f_wname, IMAGE_1, NULL, &imgres_size);
		//1 is ok 0 is error
		if (ret == 0){
			printf("\n	ERROR \n");
			vm_exit_app();
		}
	*/
	
	/* vm4res.h way 2: */	
	/*
		vm_res_init();
		ret = 0;
		IMAGE_1 = 1;
		ret = vm_res_get_string_and_size_from_file(f_wname, IMAGE_1, NULL, &imgres_size);
		if (ret == 0){
			printf("\n ERROR2 \n");
		}
	*/ 
	
	/* Load Image inside Resource 1.0 */ 
	res = vm_load_resource(file_name, &res_size ) ;//resouce only not file. Can remove this but checking anyway
	
    /* if no image found */
	if(res == NULL)
	{
		printf("\nImage from resource not found!");
	}
	if(imgres == NULL){
		printf("\nImage from file not found!");
	}

	/* Get img from file*/ 
	if(imgres != NULL && res != NULL)
	{
		/* Check what is really the difference */
		if (strcmp(imgres, res) == 0){
			printf("\nImage data from res and file are equal");
		}else{
			printf("\nImage data from resource and file are Not equal");
		}
		if(imgres_size == res_size){
			printf("\nSizes are equal!");
		}else{
			printf("\nSize not equal");
		}
	}

	//To Force it to use resources image if available 
	//imgres = NULL;

	if (imgres != NULL){ 
		/* image will be loaded in memory */
		img = vm_graphic_load_image(imgres, imgres_size); 
		//img = vm_graphic_load_image(data, imgres_size); //Testing
	}
	// Second priority to Resource. First priority to filesystem
	else if (res != NULL){
		//2. Use vm_graphic_load_image interface to decode the image onto a canvas:
		//vm_graphic_load_image(res, res_size)
		//img = vm_graphic_load_image(imgres, imgres_size);
		img = vm_graphic_load_image(res, res_size); // Trying.. 

		//img = vm_graphic_load_image(&da3ta[0], imgres_size); //Doesnt make a difference
	}else{
		printf("\nCan't  proceed with either resource or file.");
		printf("\nCan't read resource or file. Exiting..");
		vm_exit_app();
	}

	switch(img){
		case VM_GDI_ERR_UNSUPPORT_IMAGE_TYPE:
			printf("\nUnsupported image type");
			vm_exit_app();
			break;
		case VM_GDI_SUCCEED:
			printf("\nGDI Success!");
			break;
		case VM_GDI_ERR_WRONG_PARAM:
			printf("\nWrong param internally");
			vm_exit_app();
			break;
		case VM_GDI_FAILED:
			printf("\nGDI failed.");
			vm_exit_app();
			break;
		case VM_GDI_ERR_MALLOC_FAILED:
			printf("\nGDI malloc failed.");
			vm_exit_app();
			break;
	}

	if(img==VM_GDI_FAILED){
		vm_exit_app();
	}

	image_prop = vm_graphic_get_img_property(img, 1);             
	target_layer_buf = vm_graphic_get_layer_buffer(mre_layer_hdl[0]); 
	
	/* controlling expansion, shrinking or no operation on image, moving will be automatically governed as we have already shift x or y position accordingly*/
	switch (size_ctrl)
	{
		case MRE_GRAPHIC_IMAGE_EXPAND:   
            /* changing the size of image*/
			g_mre_img_size_cng = g_mre_img_size_cng + MRE_IMG_EXP_RATE; 
			if (image_prop->width + g_mre_img_size_cng >= MRE_IMG_MAX_SIZE ||
			   image_prop->height + g_mre_img_size_cng >= MRE_IMG_MAX_SIZE)
			{
				g_mre_img_size_cng = g_mre_img_size_cng - MRE_IMG_EXP_RATE;
			}
			/* releaing the old canvas*/
			vm_graphic_release_canvas(img);
			/* resizing the image and getting its handle*/
			//img = vm_graphic_load_image_resized(res, res_size, image_prop->width + g_mre_img_size_cng,
			//									image_prop->height + g_mre_img_size_cng);
			img = vm_graphic_load_image_resized(imgres, imgres_size, image_prop->width + g_mre_img_size_cng,
												image_prop->height + g_mre_img_size_cng);
			/* getting layer_buffer from image handle*/
			canvas_buffer = vm_graphic_get_canvas_buffer(img);
			/* getting image properties*/
			image_prop = vm_graphic_get_img_property(img, 1);
			/* image layer_buffer is stored in BLUE layer_buffer*/
			vm_graphic_blt(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 
							MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, image_prop->width, 
							image_prop->height, 1);
			break;

		case MRE_GRAPHIC_IMAGE_SHRINK:   
			/* changing the size of image*/			
			g_mre_img_size_cng = g_mre_img_size_cng - MRE_IMG_EXP_RATE;
			/* controlling the minimum size of shrinking*/
			if (image_prop->width + g_mre_img_size_cng <= MRE_IMG_MIN_SIZE ||
			   image_prop->height + g_mre_img_size_cng <= MRE_IMG_MIN_SIZE)
			{
				g_mre_img_size_cng = g_mre_img_size_cng + MRE_IMG_EXP_RATE;
			}
			/* releaing the old canvas*/
			vm_graphic_release_canvas(img);
			/* resizing the image and getting its handle*/
			//img = vm_graphic_load_image_resized(res, res_size, image_prop->width + g_mre_img_size_cng, 
			//									image_prop->height + g_mre_img_size_cng);
			img = vm_graphic_load_image_resized(imgres, imgres_size, image_prop->width + g_mre_img_size_cng, 
												image_prop->height + g_mre_img_size_cng);
			/* getting layer_buffer from image handle*/
			canvas_buffer = vm_graphic_get_canvas_buffer(img);
			/* getting image properties*/
			image_prop = vm_graphic_get_img_property(img, 1);
			/* image layer_buffer is stored in BLUE buffer*/
			vm_graphic_blt(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 
						   MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, image_prop->width, 
						   image_prop->height, 1);
			break;

		/* no operation on image only maintain earlier changes */
		case MRE_GRAPHIC_IMAGE_CURRENT:  

			/* releaing the old canvas*/
			vm_graphic_release_canvas(img);
			/* resizing the image and getting its handle*/
			//img = vm_graphic_load_image_resized(res, res_size, image_prop->width + g_mre_img_size_cng, 
			//									image_prop->height + g_mre_img_size_cng);
			img = vm_graphic_load_image_resized(imgres, imgres_size, image_prop->width + g_mre_img_size_cng, 
												image_prop->height + g_mre_img_size_cng);
			/* getting layer_buffer from image handle*/
			canvas_buffer = vm_graphic_get_canvas_buffer(img);
			/* getting image properties*/
			image_prop = vm_graphic_get_img_property(img, 1);
			/* image buffer is stored in BLUE buffer*/
			//vm_graphic_blt(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 
			//				MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, image_prop->width,
			//				image_prop->height, 1);
			vm_graphic_blt(target_layer_buf, x, y, canvas_buffer, 
							MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, image_prop->width,
							image_prop->height, 1);
			break;

        case MRE_GRAPHIC_IMAGE_ROTATE_CW:
            g_mre_img_clockwise_cnt += MRE_SET_IMG_ROT;
            g_mre_img_clockwise_cnt %= MRE_WHOLE_ROT;
            /* releaing the old canvas*/
			vm_graphic_release_canvas(img);
			/* resizing the image and getting its handle*/
			//img = vm_graphic_load_image_resized(res, res_size, image_prop->width + g_mre_img_size_cng, 
			//									image_prop->height + g_mre_img_size_cng);
			img = vm_graphic_load_image_resized(imgres, imgres_size, image_prop->width + g_mre_img_size_cng, 
												image_prop->height + g_mre_img_size_cng);
			/* getting buffer from image handle*/
			canvas_buffer = vm_graphic_get_canvas_buffer(img);
			/* getting image properties*/
			image_prop = vm_graphic_get_img_property(img, 1);
            if(g_mre_img_clockwise_cnt == 90 || g_mre_img_clockwise_cnt == 180 || g_mre_img_clockwise_cnt == 270)
            {
                vm_graphic_rotate(target_layer_buf,  g_mre_img_x_pos,  g_mre_img_y_pos, canvas_buffer, 1, g_mre_img_clockwise_cnt);
            }
            else
            {
			    /* image buffer is stored in BLUE buffer*/
			    vm_graphic_blt(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 
							    MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, image_prop->width,
							    image_prop->height, 1);
            }
            break;

       case MRE_GRAPHIC_IMAGE_ROTATE_AW:
            g_mre_img_clockwise_cnt = g_mre_img_clockwise_cnt + MRE_WHOLE_ROT - MRE_SET_IMG_ROT;
            g_mre_img_clockwise_cnt %= MRE_WHOLE_ROT;
            /* releaing the old canvas*/
			vm_graphic_release_canvas(img);
			/* resizing the image and getting its handle*/
			//img = vm_graphic_load_image_resized(res, res_size, image_prop->width + g_mre_img_size_cng, 
			//									image_prop->height + g_mre_img_size_cng);
			img = vm_graphic_load_image_resized(imgres, imgres_size, image_prop->width + g_mre_img_size_cng, 
												image_prop->height + g_mre_img_size_cng);
			/* getting buffer from image handle*/
			canvas_buffer = vm_graphic_get_canvas_buffer(img);
			/* getting image properties*/
			image_prop = vm_graphic_get_img_property(img, 1);
            if(g_mre_img_clockwise_cnt == 90 || g_mre_img_clockwise_cnt == 180 || g_mre_img_clockwise_cnt == 270)
            {
                vm_graphic_rotate(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 1, g_mre_img_clockwise_cnt);
            }
            else
            {
			    /* image buffer is stored in BLUE buffer*/
			    vm_graphic_blt(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 
							    MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, image_prop->width,
							    image_prop->height, 1);
            }
             break;

       case MRE_GRAPHIC_IMAGE_MIRROR:
            /* releaing the old canvas*/
			vm_graphic_release_canvas(img);
			/* resizing the image and getting its handle*/
			//img = vm_graphic_load_image_resized(res, res_size, image_prop->width + g_mre_img_size_cng, 
			//									image_prop->height + g_mre_img_size_cng);
			img = vm_graphic_load_image_resized(imgres, imgres_size, image_prop->width + g_mre_img_size_cng, 
												image_prop->height + g_mre_img_size_cng);
			/* getting buffer from image handle*/
			canvas_buffer = vm_graphic_get_canvas_buffer(img);
			/* getting image properties*/
			image_prop = vm_graphic_get_img_property(img, 1);
            vm_graphic_mirror(target_layer_buf, g_mre_img_x_pos, g_mre_img_y_pos, canvas_buffer, 1, g_mre_img_dir);
            break;

		default :
			break;
		}

	/* freeing memories*/
	vm_graphic_release_canvas(img);	
	/* freeing thoughts */ 
	vm_free(data);
}

