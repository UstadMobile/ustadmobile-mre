/*
Nuklear MRE Implementation
Author: varunasingh
Status: In progres
*/

/* ==============================================================
 *								API
 * =============================================================*/

/* If Implementation Header file is set: If declaration done
	Note: This will always be called at the start. 
	As you can see if NK_MRE_H_ will be	defined only when not 
	set. So these functions will be declared only once.
*/ 
#ifndef NK_MRE_H_
	#define NK_MRE_H_ 

	/*	Define Platform specific preprocessor directive (#define) macros.
		eg: #define WIN32_LEAN_AND_MEAN 
		This "reduces the size of the Win32 header files by excluding 
		some of the less frequently used APIs" -is Windows GDI 
		specific. Ignoring.

		Here you can also define some platform specific header files relevant.
	*/

	/*	Nuklear Header File 
		In new version v1.05, the nuklear.h file is included in main.c
		#include "nuklear.h"
	*/

	/* Font */
	typedef struct MreFont MreFont; //Gotta do this for it to know it exists
	NK_API MreFont* nk_mrefont_create(const char *name, int size);
	NK_API void nk_mre_set_font();
	NK_API void nk_mrefont_del();
	
	/* Declare the implementation:*/
	NK_API struct nk_context* nk_mre_init(MreFont *font,unsigned int width, unsigned int height);
	NK_API void nk_mre_handle_sys_event(VMINT message, VMINT param);
	NK_API void nk_mre_handle_key_event(VMINT message, VMINT keycode);
	NK_API void nk_mre_render(struct nk_color clear);
	NK_API void nk_mre_shutdown(void);

	/*Platform specific*/
	NK_API int nk_init_mre(struct nk_context *ctx, const struct nk_user_font *font);
	NK_INTERN void* mre_malloc(nk_handle unused, void *old, nk_size size); //nk_handle: A pointer, an int id; nk_size: unsigned long
	NK_INTERN void mre_free(nk_handle unused, void* old); //nk_handle: A pointer, an int id
	

#endif

/* ==============================================================
 *						MRE IMPLEMENTATION
 * =============================================================*/

/*TODO: Remove*/
#define NK_MRE_IMPLEMENTATION

/*if set to use MRE implementation*/
#ifdef NK_MRE_IMPLEMENTATION	

	/*********************
		MRE HEADER FILES
	*********************/
	#include "vmsys.h"


	/*********************
			STRUCTS
	*********************/
	/*	
		Font object
		MRE Doesn't really have a Font object
	*/
	struct MreFont{
		struct nk_user_font nk;
		int size;
	};

	struct FocusableObjects{
		int id;
		struct nk_rect area;
	};

	/* Create the mre display object */
	static struct {
		//some sort of window object - most similar in mre is layers?
		int window;
		//int memory_mre;
		unsigned int width;
		unsigned int height;
		struct nk_context ctx;
		
		//Focusable Object Areas:
		struct nk_rect fObjs[20]; 
	} mre;

	/*
	MRE-Nuklear Components
	Components used to draw
	*/
	struct mre_nk_c {
		int id;
		char *type;
		char *title;
		int len;
		struct nk_style style;
		int hovering;
		int is_hoverable;
		/* For Video: */ 
		char *url;
		char *thumbnail;
	};

	/* MRE View Properties */ 
	struct mre_view_p{
		int hoverable_counter;
		int hoverable_items;
		struct mre_nk_c **components;
		//struct mre_nk_c * components;
		int components_count;
	}mre_view;


	/*********************
		  FUNCTIONS
	*********************/

	/*	MRE's malloc 
	*/
	NK_INTERN void* mre_malloc(nk_handle unused, void *old, nk_size size){

		/*So basically what malloc does is set some memory aside in the, well, memory
		and pass it on to the pointer back. That way we know for sure that memory is
		allocated and you can store whatever data you want in it (puppy pics).
		Its important to allocate the right amount of memory needed. If an object 
		has multiple variables, we need to calculate it and allocate it. Its really
		bad to allocate a riddiculous huge amount of size that will never get filled.
		Not only that but in memory sensitive devices this will unneccesarily crash.
		Malloc will NOT initialise the memory with some data. Thats what calloc does. 
		calloc will initialise with 0s. Malloc just keeps it ready. Some cases we 
		would want to have some data but not here (afaik).
		This method gets in:
			a. nk_handle unused : Has a pointer and an int id
			b. void *old:	Old 
			c. nk_size size: The size of memory allocation in unsigned long
	
		Steps:
			i. Cast void to both unused and old
			ii. Malloc size and return it.
		*/

		void *malloc_pointer;
		
		/* Not sure what this does. Ignoring for now*/
		//NK_UNUSED(unused);
		//NK_UNUSED(old);

		//TODO: Check this for MRE phones
		//malloc_pointer = (void *)vm_malloc(size);
		malloc_pointer = (void *)vm_malloc(size); 
		return malloc_pointer;
		
	}

	/*	MRE's free
	*/
	NK_INTERN void mre_free(nk_handle unused, void *ptr){
		NK_UNUSED(unused);
		vm_free(ptr);
	}

	/*
		Initialises the context - MRE way
		We have to call this instead of nk_init_default (which is part of nuklear..h)
		because that method uses STL and we have to make our own implementation of it	
	*/
	NK_API int
	nk_init_mre(struct nk_context *ctx, const struct nk_user_font *font) //similar to nk_init_default
	{
		/*
			nk_allocator:
				a. nk_alloc Obj: userdata
					i. A pointer
					ii. An id
				b. alloc ()
				c. free ()

			Steps:
			1. Set the pointer to a value
			2. Set alloc method
			3. Set Free Method
			4. Call nk_init(ctx, font) to begin process

		*/

		struct nk_allocator alloc;
		alloc.userdata.ptr = 0;
		/*	Assign implementation's alloc and malloc
			alloc(nk_handle, void *old, nk_size)
			free(nk_handle, void*) 
			Make sure they work OK
		*/ 
		alloc.alloc = mre_malloc;
		alloc.free = mre_free;

		//NOT ALLOWING FONT TO BE SET HERE. 
		//return nk_init(ctx, &alloc, font);


		return nk_init(ctx, &alloc, font);

		/*Origi code: 
		struct nk_allocator alloc;
		alloc.userdata.ptr = 0;
		alloc.alloc = nk_malloc; //(nk_handle, void *old, nk_size);
		alloc.free = nk_mfree;   //(nk_handle, void*)
		return nk_init(ctx, &alloc, font);*/
	}

	/*
		Convert nuklear's color into implementation color!
		 Color being used for all that drawing we gotta do
		 Used internally, not an implementation 
		 Can be called anything..
	*/
	static VMINT 
	convert_color(struct nk_color c){
		//..convert nk_color c to MRE color..
		//returning red for now..
		//return VM_RGB32_ARGB(c.a, c.r, c.g, c.b);
		return VM_RGB565_ARGB(c.a, c.r, c.g, c.b);
		//return VM_COLOR_RED;
	}


	/***********************************************
		IMPL FUNCTIONS

		Stroke: is line drawing
		Fill: is line drawing + the area under the 
				outline (usually color)
	***********************************************/

	/*
		Scissor
		Something to do with scissor ? 
		Called by this command: NK_COMMAND_SCISSOR
	*/
	static void 
	nk_mre_scissor(void){
		vm_log_debug("Scissor-ing..");
	}

	/*
		Stroke line
		Draw a line as stroke (outline)
		Called by command: NK_COMMAND_LINE
	*/ 
	static void
	nk_mre_stroke_line(short x0, short y0, short x1, short y1, 
						unsigned int line_thickness, struct nk_color col)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMINT LINE_X1, LINE_X2, LINE_Y1, LINE_Y2;
		//Have the layer handle ready.. layer_hdl
		//Maybe as global or passed here..

		/* 
			get the target buffer from the layer
		*/
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//No need to fill a rectangle here..
		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//					 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_GREEN);

		color = VM_COLOR_RED;
		color = convert_color(col);
		LINE_X1 = x0; 
		LINE_X2 = x1;
		LINE_Y1 = y0;
		LINE_Y2 = y1;
		//line_thickness not implemented..

		vm_graphic_line(buffer, LINE_X1, LINE_Y1, LINE_X2, LINE_Y2, color);
		/* 
			Gotta flush 
			Flush updates the LCD
		*/
		//vm_graphic_flush_layer(layer_hdl, 1);
		vm_log_debug("draw line fun ends");
	}

	/*
		Stroke Rectangle
		Draw a rectangle as a stroke (outline)
		Called by: NK_COMMAND_RECT
	*/
	static void
	nk_mre_stroke_rect(short x, short y, unsigned short w,
		unsigned short h, unsigned short r, unsigned short line_thickness, struct nk_color col)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMINT REC_X, REC_Y, REC_W, REC_L;

		color = convert_color(col);
		REC_X =	x;
		REC_Y = y;
		REC_W = w;
		REC_L = h;

		/*Get the target buffer */
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//                 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_BLUE);

		vm_graphic_rect(buffer, REC_X, REC_Y, REC_W, REC_L, color);
		/* 
			Gotta flush:
			Flush updates the LCD 
		*/
		//vm_graphic_flush_layer(layer_hdl, 1); 

	}

	/*
		Fill Rectangle
		Draws a rectangle outline and color fills it
		Called by command: NK_COMMAND_RECT_FILLED
	*/
	static void
	nk_mre_fill_rect(short x, short y, unsigned short w,
		unsigned short h, unsigned short r, struct nk_color col)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMINT REC_X, REC_Y, REC_W, REC_L;

		color = convert_color(col);
		REC_X =	x;
		REC_Y = y;
		REC_W = w;
		REC_L = h;

		/*Get the target buffer */
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//                 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_BLUE);

		//vm_graphic_rect(buffer, REC_X, REC_Y, REC_W, REC_L, color);
		vm_graphic_fill_rect(buffer, REC_X, REC_Y, REC_W, REC_L, color, color);

		//Ignoring filled one first
		//vm_graphic_fill_rect(buffer, REC_X, REC_Y, REC_W, REC_L, color, color);
		/* 
			Gotta flush:
			Flush updates the LCD 
		*/
		//vm_graphic_flush_layer(layer_hdl, 1); 

	}

	/*
		Fill Triangle
		Draws a triangle and fills area in between with color
		Called by command: NK_COMMAND_TRIANGLE_FILLED
	*/
	static void
	nk_mre_fill_triangle(short x0, short y0, short x1, short y1,
		short x2, short y2, struct nk_color colr)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMINT TRN_X1, TRN_X2, TRN_X3, TRN_Y1, TRN_Y2, TRN_Y3;
		vm_graphic_color col;

		//vm_graphic_color col;
		vm_graphic_point points[3];

		color = convert_color(colr);
		TRN_X1 = x0;
		TRN_X2 = x1;
		TRN_X3 = x2;
		TRN_Y1 = y0;
		TRN_Y2 = y1;
		TRN_Y3 = y2;

		/* function body */
		vm_log_debug("draw triangle starts");
		points[0].x = TRN_X1;
		points[0].y = TRN_Y1;
		points[1].x = TRN_X2;
		points[1].y = TRN_Y2;
		points[2].x = TRN_X3;
		points[2].y = TRN_Y3;
	   
		col.vm_color_565 = VM_COLOR_RED;
		col.vm_color_888 = VM_COLOR_565_TO_888(VM_COLOR_RED);
		/* sets global color */
		vm_graphic_setcolor(&col);

		/* get the target buffer*/
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//					 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_GREEN);

		/* Draw 3 points from points on the layer */
		vm_graphic_fill_polygon(layer_hdl[0], points, 3);

		/* Gotta flush */
		//vm_graphic_flush_layer(layer_hdl, 1);

	}

	/*	
		Stroke Triangle
		Draws the outline of the triangle
		Called by command: NK_COMMAND_TRIANGLE
	*/
	static void
	nk_mre_stroke_triangle(short x0, short y0, short x1, short y1,
		short x2, short y2, unsigned short line_thickness, struct nk_color colr)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMINT TRN_X1, TRN_X2, TRN_X3, TRN_Y1, TRN_Y2, TRN_Y3;

		vm_graphic_color col;
		vm_graphic_point points[3];

		color = convert_color(colr);
		TRN_X1 = x0;
		TRN_X2 = x1;
		TRN_X3 = x2;
		TRN_Y1 = y0;
		TRN_Y2 = y1;
		TRN_Y3 = y2;

		/* function body */
		vm_log_debug("draw triangle starts");
		points[0].x = TRN_X1;
		points[0].y = TRN_Y1;
		points[1].x = TRN_X2;
		points[1].y = TRN_Y2;
		points[2].x = TRN_X3;
		points[2].y = TRN_Y3;
	   
		col.vm_color_565 = VM_COLOR_RED;
		col.vm_color_888 = VM_COLOR_565_TO_888(VM_COLOR_RED);

		/* sets global color */
		//vm_graphic_setcolor(&col);

		/* get the target buffer*/
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//					 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_GREEN);

		/* Draw 3 points from points on the layer */
		vm_graphic_fill_polygon(layer_hdl[0], points, 3);

		/* Gotta flush */
		//vm_graphic_flush_layer(layer_hdl, 1);

	}

	/*
		Fill Polygon
		Draws polygon with count number from points and fills color in 
		Called by command: NK_COMMAND_POLYGON_FILLED
	*/
	static void
	nk_mre_fill_polygon(const struct nk_vec2i *pnts, int count, struct nk_color colr)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		
		/*int a=42;
		const int b=a;
		int ipoints[b]; //Wont work because C89
		*/

		//int *a = new int[count]; //Not C syntax. More like c++
		//vm_graphic_point points[10]; //Works because number is known at the time of compilation

		//The way dynamically loading works:
		//char* utf8 = malloc(utf8size);
		int pointsize = count * sizeof(vm_graphic_point);
		vm_graphic_point* points = vm_malloc(pointsize);

		//VMINT TRN_X1, TRN_X2, TRN_X3, TRN_Y1, TRN_Y2, TRN_Y3;

		vm_graphic_color col;
		
		color = convert_color(colr);
		/*TRN_X1 = x0;
		TRN_X2 = x1;
		TRN_X3 = x2;
		TRN_Y1 = y0;
		TRN_Y2 = y1;
		TRN_Y3 = y2;*/

		/* function body */
		vm_log_debug("draw polygon starts");

		/*for every point in nk_vec2i pnts .. put value in points..*/

		/*points[0].x = TRN_X1;
		points[0].y = TRN_Y1;
		points[1].x = TRN_X2;
		points[1].y = TRN_Y2;
		points[2].x = TRN_X3;
		points[2].y = TRN_Y3;*/
	   
		col.vm_color_565 = VM_COLOR_RED;
		col.vm_color_888 = VM_COLOR_565_TO_888(VM_COLOR_RED);

		/* sets global color */
		vm_graphic_setcolor(&col);

		/* get the target buffer*/
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//					 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_GREEN);

		/* Draw 3 points from points on the layer */
		vm_graphic_fill_polygon(layer_hdl[0], points, 3);

		/* Gotta flush */
		//vm_graphic_flush_layer(layer_hdl, 1);
	}

	/*
		Stroke Polygon
		Draws polygon of count points from points as outline
		Called by command: NK_COMMAND_POLYGON
	*/
	static void
	nk_mre_stroke_polygon(const struct nk_vec2i *pnts, int count, 
		unsigned short line_thickness, struct nk_color colr)
	{
			
		VMUINT8 *buffer;
		VMUINT16 color;
		//VMINT TRN_X1, TRN_X2, TRN_X3, TRN_Y1, TRN_Y2, TRN_Y3;

		vm_graphic_color col;
		vm_graphic_point points[3];

		color = convert_color(colr);

		/*TRN_X1 = x0;
		TRN_X2 = x1;
		TRN_X3 = x2;
		TRN_Y1 = y0;
		TRN_Y2 = y1;
		TRN_Y3 = y2;*/

		/* function body */
		vm_log_debug("draw triangle starts");

		/* for every points in vec2i pts put value in points..*/

		/*points[0].x = TRN_X1;
		points[0].y = TRN_Y1;
		points[1].x = TRN_X2;
		points[1].y = TRN_Y2;
		points[2].x = TRN_X3;
		points[2].y = TRN_Y3;*/
	   
		col.vm_color_565 = VM_COLOR_RED;
		col.vm_color_888 = VM_COLOR_565_TO_888(VM_COLOR_RED);

		/* sets global color */
		//vm_graphic_setcolor(&col);

		/* get the target buffer*/
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//					 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_GREEN);

		/* Draw 3 points from points on the layer */
		vm_graphic_fill_polygon(layer_hdl[0], points, 3);

		/* Gotta flush */
		//vm_graphic_flush_layer(layer_hdl, 1);

	}

	/*
		Stroke PolyLine
		Draws a polyline as outline
		Called by command: NK_COMMAND_POLYLINE
	*/
	static void
	nk_mre_stroke_polyline(const struct nk_vec2i *pnts, int count,
		unsigned short line_thickness, struct nk_color col)
	{
		//VMUINT8 *buffer;
		VMUINT16 color;

		color = convert_color(col);

		if (count > 0) {
			int i;
			//MoveToEx(dc, pnts[0].x, pnts[0].y, NULL);
			for (i = 1; i < count; ++i)
			{
				//LineTo(dc, pnts[i].x, pnts[i].y);
				//Replace this with draw line
			}
		}

	}

	/*	
		Fill Circle
		Draws a circle and fills the area inside of it
		Called by command: NK_COMMAND_CIRCLE_FILLED
	*/
	static void
	nk_mre_fill_circle(short x, short y, unsigned short w,
		unsigned short h, struct nk_color col)
	{
		//VMUINT8 *buffer;
		VMUINT16 color;
		
		color = convert_color(col);
		

	}

	/*
		Stroke Circle
		Draws a circle's outline
		Called by command: NK_COMMAND_CIRCLE
	*/
	static void
	nk_mre_stroke_circle(short x, short y, unsigned short w,
		unsigned short h, unsigned short line_thickness, struct nk_color col)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMINT REC_X, REC_Y, REC_W, REC_L;

		color = convert_color(col);
		REC_X =	x;
		REC_Y = y;
		REC_W = w;
		REC_L = h;

		/*Get the target buffer */
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//                 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_BLUE);

		//void vm_graphic_ellipse (VMUINT8 * buf, VMINT x, VMINT y, VMINT width, VMINT height, VMUINT16 color);
		/*TODO: Figure this out */
		//vm_graphic_ellipse(buffer,...);

		/* 
			Gotta flush:
			Flush updates the LCD 
		*/
		//vm_graphic_flush_layer(layer_hdl, 1); 

	}

	/*
		Stroke Curve
		Draws a curve outline
		Called by command: NK_COMMAND_CURVE
	*/
	static void
	nk_mre_stroke_curve(struct nk_vec2i p1, struct nk_vec2i p2, 
		struct nk_vec2i p3, struct nk_vec2i p4, unsigned short line_thickness,
			struct nk_color col)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		//VMINT REC_X, REC_Y, REC_W, REC_L;

		color = convert_color(col);
		
		/*Get the target buffer */
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		//vm_graphic_fill_rect(buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width(), 
		//                 vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_BLUE);

		//void vm_graphic_ellipse (VMUINT8 * buf, VMINT x, VMINT y, VMINT width, VMINT height, VMUINT16 color);
		/*TODO: Figure this out*/
		//vm_graphic_fill_ellipse(buffer,...);

		/* 
			Gotta flush:
			Flush updates the LCD 
		*/
		//vm_graphic_flush_layer(layer_hdl, 1); 

	}

	/*
		Draw Text
		Draws text
		Called by command: NK_COMMAND_TEXT
	*/
	static void
	nk_mre_draw_text(short x, short y, unsigned short w, unsigned short h, 
		const char *text, int len, struct nk_color cbg, struct nk_color col)
	{
		VMUINT8 *buffer;
		VMUINT16 color;
		VMWCHAR display_string[MRE_STR_SIZE_MAX];
		color = convert_color(col);
		
		/*Get the target buffer */
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		/*
			Text it out
			We need to convert strings to usc2 format that 
			MRE only suppports 
			Then we draw the text on the screen
		*/
		/* converts string into usc2 format to display on the screen */
		vm_gb2312_to_ucs2(display_string, MRE_STR_SIZE_MAX, (VMSTR)text);

		/* 
			Draw Text 
			*  disp_buf : [IN] display buffer pointer 
			* x : [IN] x offset of start position
			* y : [IN] y offset of start position
			* s : [IN] string pointer
			* length : [IN] string length
			* color : [IN] string color
		*/
		vm_graphic_textout(buffer, x, 
			y, display_string, 
			wstrlen(display_string), VM_COLOR_WHITE);

		/* 
			Gotta flush:
			Flush updates the LCD 
		*/
		//vm_graphic_flush_layer(layer_hdl, 1);


	}

	/*
		Draw Text
		Draws text
		Called by command: NK_COMMAND_TEXT
	*/
	static void
	nk_mre_draw_image(short x, short y, short w, short h)
	{
		//struct nk_image img, int x, int y, const char *text
		VMUINT8 *buffer;
		VMUINT16 color;
		//VMWSTR filename;
		VMWCHAR display_string[MRE_STR_SIZE_MAX];
		VMINT res;
		int ret;
		/* File name related variables */
		VMWSTR wfilename;
		VMINT wfilename_size;
		VMSTR filename;
		VMCHAR f_name[MRE_STR_SIZE_MAX + 1];	//Old usage for video filename string
		VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1]; //Old usage for video filename
		VMSTR file_name = "tips.gif";

		
		/*Get the target buffer */
		buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		/* 
			Gotta flush:
			Flush updates the LCD 
		*/
		//vm_graphic_flush_layer(layer_hdl, 1);

		/* Draw the PIKCHAAA */ 
		/* Notes:
			VM_GDI_HANDlE is basically VMINT

		*/
		//VMINT vm_graphic_draw_image_from_file(VM_GDI_HANDLE dest_layer_handle, 
		//	VMINT x, VMINT y, const VMWSTR filename);
		//Testing:

		/* Getting em strings ready */
			//Gotta allocate VMSTR before we do anything to it!
			filename = vm_malloc(MRE_STR_SIZE_MAX);
			sprintf (filename, "%c:\\%s", vm_get_removable_driver(), file_name);
			//Gotta allocate VMWSTR before we do anything to it!
			wfilename_size = (strlen(filename) + 1) * 2;
			wfilename = vm_malloc(wfilename_size);
			sprintf(f_name, "%c:\\%s", vm_get_removable_driver(), file_name);

		/* String format conversion */
			vm_ascii_to_ucs2 (wfilename, MRE_STR_SIZE_MAX, filename);
			vm_ascii_to_ucs2(f_wname, MRE_STR_SIZE_MAX, f_name);

		ret = vm_graphic_draw_image_from_file(layer_handle[0],
			//80, 80, "E:\\tips.gif");
			80, 80, f_wname);
			//80, 80, "E:\umicon.jpg");
		if (ret == VM_GDI_SUCCEED){
			printf("Drew image?"); //Not sure what happens in there.
			mre_show_image(MRE_GRAPHIC_IMAGE_CURRENT, f_wname, file_name, layer_hdl, x, y);
		}else{
			printf("Couldn't draw image..");
		}

	}

	/*
		Clear It all
		No idea what this is doing
	*/
	static void
	nk_mre_clear(struct nk_color col)
	{
		VMUINT16 color;
		color = convert_color(col);

	}

	/*
		Whats a blit?
		No idea what this is doing
		I have a feeling this copies something from memory to memory
		In this case its copying the worked on device context 
			to the gdi's memory device contesxt
		(Probably not needed in MRE. Most similar is: flushing)
	*/
	static void
	nk_mre_blit(void)
	{
		//Do nothing	
	}

	/********************************************************
						FONT STUFF
	********************************************************/

	/*Create the Font*/
	MreFont*
	nk_mrefont_create(const char *name, int size)
	{
		MreFont *font;
		int obj_size = 1;
		//MreFont *font = (MreFont*)vm_calloc(1, sizeof(MreFont));
		obj_size = sizeof(MreFont);
		font = (MreFont*)vm_calloc(obj_size);
		if (!font){
			return NULL;
		}
		/*Create font params*/
		font->size = size;
		return font;
	}

	/*Get Text Width*/
	static float
	nk_mrefont_get_text_width()
	{
		//Do nothing
		return (float)1;
	}

	/*
		Set Font
		Sets the font required in the platform
		Called at the start of the implementation
	*/
	NK_API void
	nk_mre_set_font(void)
	{
		vm_log_debug("Not setting font - MRE will default it");
	}

	/*
		Delete the font
		Deletes the font set
	*/
	void 
	nk_mrefont_del()
	{
		//Do nothing
	}

	/* Get font float 
		Needed in nk_mre_init method for initialising nk's global font object:
		nk_user_font. We need to give it some value. Since MRE's font isn't
		really complete, we just give it some random float value (1) for now
	*/
	static float
	get_font_width(nk_handle handle, float height, const char *text, int len)
	{
		//Get font width in float
		float size;
		float letter_size = 7;
		size = letter_size * len;
		return (float)size;

	}



	/********************************************************
						CLIPBOARD STUFF
	********************************************************/

	/*Paste from clipbard?*/
	/*
	static void
	nk_gdi_clipbard_paste(nk_handle usr, struct nk_text_edit *edit)
	{
		(void)usr;
		if (IsClipboardFormatAvailable(CF_UNICODETEXT) && OpenClipboard(NULL))
		{
			HGLOBAL mem = GetClipboardData(CF_UNICODETEXT); 
			if (mem)
			{
				SIZE_T size = GlobalSize(mem) - 1;
				if (size)
				{
					LPCWSTR wstr = (LPCWSTR)GlobalLock(mem); 
					if (wstr) 
					{
						int utf8size = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)(size / sizeof(wchar_t)), NULL, 0, NULL, NULL);
						if (utf8size)
						{
							char* utf8 = malloc(utf8size);
							if (utf8)
							{
								WideCharToMultiByte(CP_UTF8, 0, wstr, (int)(size / sizeof(wchar_t)), utf8, utf8size, NULL, NULL);
								nk_textedit_paste(edit, utf8, utf8size);
								free(utf8);
							}
						}
						GlobalUnlock(mem); 
					}
				}
			}
			CloseClipboard();
		}
	}
	*/

	/*Copy from clipboard*/
	/*
	static void
	nk_gdi_clipbard_copy(nk_handle usr, const char *text, int len)
	{
		if (OpenClipboard(NULL))
		{
			int wsize = MultiByteToWideChar(CP_UTF8, 0, text, len, NULL, 0);
			if (wsize)
			{
				HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, (wsize + 1) * sizeof(wchar_t));
				if (mem)
				{
					wchar_t* wstr = GlobalLock(mem);
					if (wstr)
					{
						MultiByteToWideChar(CP_UTF8, 0, text, len, wstr, wsize);
						wstr[wsize] = 0;
						GlobalUnlock(mem); 

						SetClipboardData(CF_UNICODETEXT, mem); 
					}
				}
			}
			CloseClipboard();
		}
	}
	*/

	/********************************************************
					INITIALISE DECLARATIONS
	********************************************************/

	/*
		Initialise the Display stuff - fonts, window, etc
	*/
	NK_API struct nk_context*
	nk_mre_init(MreFont *mrefont, unsigned int width, unsigned int height){
		//Maybe keep some of this outside ? : Not sure. Needs to be thought after its working ok
		
		/* Define window/layer */
		//Layer already initiated in mre_io_sample.h:
		//VMINT		layer_hdl[1];

		/*Declaring the buffer for drawing*/
		VMUINT8 *layer_buf;

		/*Declare the nk_user_font*/
		struct nk_user_font *font = &mrefont->nk;

		/*Initialising the layer handle*/
		layer_hdl[0] = -1;

		/*Create nk_user_font object from implementations' font object*/
		//struct nk_user_font *font = &mrefont->nk;
		font->userdata = nk_handle_ptr(mrefont); //Creates an nk_hanlde off that ptr (nk_handle has id and ptr)
		font->height = (float)mrefont->size;
		font->width = get_font_width; //Set it to whatever
		//TODO: Check..

		/*Setting Global data for Graphics(set width, height, font size*/
		mre_set_graphic_global_data();

		/*Set language, system, key event callbacks*/
		vm_mul_lang_set_lang (VM_LANG_ENG);	//Sets Language

		/*
		vm_mul_lang_set_lang (VM_LANG_ENG);
		vm_reg_sysevt_callback (handle_sysevt);
		vm_reg_keyboard_callback (handle_keyevt);
		vm_reg_pen_callback (handle_penevt);
		*/
		
		/*
		vm_mul_lang_set_lang (VM_LANG_ENG);
		vm_reg_sysevt_callback (nk_mre_handle_sys_event); //looks at implementation event handler (does thesame thing)
		vm_reg_keyboard_callback (nk_mre_handle_key_event);	//Keyboard callback
		vm_reg_pen_callback (handle_penevt);	//Pen callback - not really needed
		vm_log_file ("declared buffer, initialised layer handle,set global data done.\n");
		*/
		
		/*Set the mre struct object*/
		mre.width = width;
		mre.height = height;

		/*PAINTING AND PREPARING THE LAYER HANDLER*/ 

		/* create base layer that has same size as the screen*/
		layer_hdl[0] = vm_graphic_create_layer(MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, 
			vm_graphic_get_screen_width(), 
			vm_graphic_get_screen_height(), VM_NO_TRANSPARENT_COLOR);

		/*
			set clip area
			Remember to reset clip region after drawing
		*/
		vm_graphic_set_clip (0, 0, 
				vm_graphic_get_screen_width (), 
				vm_graphic_get_screen_height ());
		//vm_graphic_set_clip(CLIP_LT_X, CLIP_LT_Y, CLIP_RT_X,  CLIP_RT_Y);

		/* get the layer handler'target buffer*/
		layer_buf = vm_graphic_get_layer_buffer (layer_hdl[0]);
		
		/* fill the whole screen with BLACK with WHITE as line color*/
		vm_graphic_fill_rect (layer_buf, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
							 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);
		
		/* Flushing - this will actually flush and display on the screen */
		vm_graphic_flush_layer (layer_hdl, 1);
		vm_log_debug ("Initialised ALL OK.");
	
		/*Init Memory Allocations for context*/
		//nk_init_default(&mre.ctx, 0); //Wont work uses StandardLib
		//nk_init_mre(&mre.ctx, 0); //Old before font bug was discovered.
		nk_init_mre(&mre.ctx, font);

		return &mre.ctx;
	}

	/*
		Handle Platform Events
		Platform specific event handler
		Declared at header file
		Initialised at init
	*/
	NK_API void
	nk_mre_handle_key_event(VMINT event, VMINT keycode){
		//nk_mre_handle_event(Event crumb(s)){
		//crumb being values we can judge what event happened..

		//Do stuff with the events. Like keypresses, etc

		//This is similar to MRE's handle_sysevt(VMINT message, VMINT param).
		// We could call that method from here..
		//handle_sysevt(message, param);
		//But this is not event for the whole application but for this Graphical bit
		//Which means this is where scrolling, pressing buttons, Copy/Paste, Right click,
		//other contexual options related to the GUI would appear.

		if (event == VM_KEY_EVENT_UP)
		{
			//int down = VM_KEY_DOWN;
 			int down = VM_KEY_EVENT_UP;
			//int down = (lparam >> 31) & 1;
			//int ctrl = GetKeyState(VK_CONTROL) & (1 << 15);
			int ctrl = VM_KEY_CTRL;

			switch (keycode)
			{
				case VM_KEY_SHIFT:	//Right, Left is all one in MRE
					nk_input_key(&mre.ctx, NK_KEY_SHIFT, down);
					//return 1;

				case VM_KEY_DEL:
					nk_input_key(&mre.ctx, NK_KEY_DEL, down);
					//return 1;

				case VM_KEY_OK: //OK/Enter should do the same, Yeah?
				case VM_KEY_QWERTY_ENTER:
					nk_input_key(&mre.ctx, NK_KEY_ENTER, down);
					//return 1;

				case VM_KEY_TAB:
					nk_input_key(&mre.ctx, NK_KEY_TAB, down);
					//return 1;

				case VM_KEY_LEFT:
					if (ctrl)
						nk_input_key(&mre.ctx, NK_KEY_TEXT_WORD_LEFT, down);
					else
						nk_input_key(&mre.ctx, NK_KEY_LEFT, down);
					//return 1;

				case VM_KEY_RIGHT:
					if (ctrl)
						nk_input_key(&mre.ctx, NK_KEY_TEXT_WORD_RIGHT, down);
					else
						nk_input_key(&mre.ctx, NK_KEY_RIGHT, down);
					//return 1;
				case VM_KEY_RIGHT_SOFTKEY: //Should be the same right?
				case VM_KEY_BACK:
					nk_input_key(&mre.ctx, NK_KEY_BACKSPACE, down);
					//return 1;

				/*
				case VK_HOME:
					nk_input_key(&gdi.ctx, NK_KEY_TEXT_START, down);
					nk_input_key(&gdi.ctx, NK_KEY_SCROLL_START, down);
					return 1;

				case VK_END:
					nk_input_key(&gdi.ctx, NK_KEY_TEXT_END, down);
					nk_input_key(&gdi.ctx, NK_KEY_SCROLL_END, down);
					return 1;
				*/

				case VM_KEY_DOWN:
					nk_input_key(&mre.ctx, NK_KEY_SCROLL_DOWN, down);
					//debug info: nk_input_key(ctx, 27, -2);
					if (mre_view.hoverable_counter < mre_view.hoverable_items){
						mre_view.hoverable_counter++;
					}

					update_gui();
					//update_gui();
					//return 1;
					break;

				case VM_KEY_UP:
					nk_input_key(&mre.ctx, NK_KEY_SCROLL_UP, down);
					if (mre_view.hoverable_counter > 0){
						mre_view.hoverable_counter--;
					}else{
						mre_view.hoverable_counter = 0;
					}
					update_gui();
					//update_gui();
					//return 1;
					break;

				case VM_KEY_POUND:
					//#
					//Testing:
					set_test2_view();

					update_gui();
					break;
				case VM_KEY_STAR:
					//*
					//Testing:
					set_test_view();

					update_gui();
					break;

				case VM_KEY_C:
					if (ctrl) {
						nk_input_key(&mre.ctx, NK_KEY_COPY, down);
						//return 1;
					}
					break;

				case VM_KEY_V:
					if (ctrl) {
						nk_input_key(&mre.ctx, NK_KEY_PASTE, down);
						//return 1;
					}
					break;

				case VM_KEY_X:
					if (ctrl) {
						nk_input_key(&mre.ctx, NK_KEY_CUT, down);
						//return 1;
					}
					break;

				case VM_KEY_Z:
					if (ctrl) {
						nk_input_key(&mre.ctx, NK_KEY_TEXT_UNDO, down);
						//return 1;
					}
					break;

				case VM_KEY_R:
					if (ctrl) {
						nk_input_key(&mre.ctx, NK_KEY_TEXT_REDO, down);
						//return 1;
					}
					break;
			}

			//return 0;
		}

	}

	/*
		Handles Key Press events
		Declared in header file
		Initialised at init
	*/
	NK_API void
	nk_mre_handle_sys_event	(VMINT message, VMINT param){
		//nk_mre_handle_event(Event crumb(s)){
		//crumb being values we can judge what event happened..

		//Do stuff with the events. System Event
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
				//mre_start_menu ();
				//initiate_nuklear_gui();
				initiate_nk_gui();
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

		//This is similar to MRE's handle_sysevt(VMINT message, VMINT param).
		// We could call that method from here..
		//handle_keyevt(message, param);
	}

	/*
		Shutdown the MRE display stuff
		Delete from mem and display/bitmap
		Declared during initialisation
	*/
	NK_API void
	nk_mre_shutdown(void){

		/*Flush layer - Like dry the layers together / Refresh*/ 
		//vm_graphic_flush_layer(layer_hdl, 2); 

		/* Delete top layer */ 
		//vm_graphic_delete_layer(layer_hdl[1]); 
		//layer_hdl[1] = -1; 
		//layer_buf[1] = NULL; 
		//Resets clip region to full size.
		//vm_graphic_reset_clip();	
	}

	/*
		Render Event handler - Actually draws the UI
		This function loops through all the primitive
		draw calls and calls MRE implementation equivalents.
		
		This function is triggered whenever the GUI needs 
		updating. This Event handler calls the implementation bits
		Declared at the begenning
	*/
	NK_API void
	nk_mre_render(struct nk_color clear){

		const struct nk_command *cmd;
		struct nk_context *ctx = &mre.ctx;
		
		//VMUINT8 *buffer;

		//Apart from command, there is some form
		//of context that xlib uses

		/* 
			get the target buffer from the layer
		*/
		//buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

		/* fill the screen*/
		//vm_graphic_fill_rect (buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, vm_graphic_get_screen_width (), 
		//					 vm_graphic_get_screen_height (), VM_COLOR_WHITE, VM_COLOR_BLACK);

		/*Test: Clearing*/
		//nk_clear(&mre.ctx); //Clears everything here..


		/* Something with memory */

		/*Actual Render bits*/
		nk_foreach(cmd, &mre.ctx)
		{
			switch (cmd->type) {
			case NK_COMMAND_NOP: break;
			case NK_COMMAND_SCISSOR: {
				const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
				//nk_gdi_scissor(memory_dc, s->x, s->y, s->w, s->h);
				//nk_mre_scissor(s->x, s->y, s->w, s->h);
				nk_mre_scissor();
			} break;
			case NK_COMMAND_LINE: {
				const struct nk_command_line *l = (const struct nk_command_line *)cmd;
				//nk_mre_stroke_line(memory_dc, l->begin.x, l->begin.y, l->end.x,
				//	l->end.y, l->line_thickness, l->color);
				nk_mre_stroke_line(l->begin.x, l->begin.y, l->end.x,
					l->end.y, l->line_thickness, l->color);
			} break;
			case NK_COMMAND_RECT: {
				const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
				//nk_mre_stroke_rect(memory_dc, r->x, r->y, r->w, r->h,
				//	(unsigned short)r->rounding, r->line_thickness, r->color);
				nk_mre_stroke_rect(r->x, r->y, r->w, r->h,
					(unsigned short)r->rounding, r->line_thickness, r->color);
			} break;
			case NK_COMMAND_RECT_FILLED: {
				const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
				//nk_mre_fill_rect(memory_dc, r->x, r->y, r->w, r->h,
				//	(unsigned short)r->rounding, r->color);
				nk_mre_fill_rect(r->x, r->y, r->w, r->h,
					(unsigned short)r->rounding, r->color);
			} break;
			case NK_COMMAND_CIRCLE: {
				const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
				//nk_mre_stroke_circle(memory_dc, c->x, c->y, c->w, c->h, c->line_thickness, c->color);
				nk_mre_stroke_circle(c->x, c->y, c->w, c->h, c->line_thickness, c->color);
			} break;
			case NK_COMMAND_CIRCLE_FILLED: {
				const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
				//nk_mre_fill_circle(memory_dc, c->x, c->y, c->w, c->h, c->color);
				nk_mre_fill_circle(c->x, c->y, c->w, c->h, c->color);
			} break;
			case NK_COMMAND_TRIANGLE: {
				const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
				//nk_mre_stroke_triangle(memory_dc, t->a.x, t->a.y, t->b.x, t->b.y,
				//	t->c.x, t->c.y, t->line_thickness, t->color);
				nk_mre_stroke_triangle( t->a.x, t->a.y, t->b.x, t->b.y,
					t->c.x, t->c.y, t->line_thickness, t->color);
			} break;
			case NK_COMMAND_TRIANGLE_FILLED: {
				const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
				//nk_mre_fill_triangle(memory_dc, t->a.x, t->a.y, t->b.x, t->b.y,
				//	t->c.x, t->c.y, t->color);
				nk_mre_fill_triangle(t->a.x, t->a.y, t->b.x, t->b.y,
					t->c.x, t->c.y, t->color);
			} break;
			case NK_COMMAND_POLYGON: {
				const struct nk_command_polygon *p =(const struct nk_command_polygon*)cmd;
				//nk_mre_stroke_polygon(memory_dc, p->points, p->point_count, p->line_thickness,p->color);
				nk_mre_stroke_polygon(p->points, p->point_count, p->line_thickness,p->color);
			} break;
			case NK_COMMAND_POLYGON_FILLED: {
				const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
				//nk_mre_fill_polygon(memory_dc, p->points, p->point_count, p->color);
				nk_mre_fill_polygon(p->points, p->point_count, p->color);
			} break;
			case NK_COMMAND_POLYLINE: {
				const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
				//nk_mre_stroke_polyline(memory_dc, p->points, p->point_count, p->line_thickness, p->color);
				nk_mre_stroke_polyline( p->points, p->point_count, p->line_thickness, p->color);
			} break;
			case NK_COMMAND_TEXT: {
				const struct nk_command_text *t = (const struct nk_command_text*)cmd;
				/*nk_mre_draw_text(memory_dc, t->x, t->y, t->w, t->h,
					(const char*)t->string, t->length,
					(GdiFont*)t->font->userdata.ptr,
					t->background, t->foreground);*/
				printf("Text starts at (%d,%d) height: %d, width:%d. \"%s\"  \n",
					t->x,t->y,t->h,t->w,t->string);
				nk_mre_draw_text(t->x, t->y, t->w, t->h,
					(const char*)t->string, t->length,
					t->background, t->foreground);
			} break;
			case NK_COMMAND_CURVE: {
				const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
				//nk_mre_stroke_curve(memory_dc, q->begin, q->ctrl[0], q->ctrl[1],
				//	q->end, q->line_thickness, q->color);
				nk_mre_stroke_curve(q->begin, q->ctrl[0], q->ctrl[1],
					q->end, q->line_thickness, q->color);
			} break;
			case NK_COMMAND_RECT_MULTI_COLOR:
				break;
			case NK_COMMAND_IMAGE: {
				// Get the image
				//nk_mre_draw_image(nk_image_img, pos_x, pos_y, image_file_path);
				const struct nk_command_image *i = (const struct nk_command_image *)cmd;
				printf("Finally, lets draw some pictures!");
				//nk_mre_draw_image(i->x, i->y, i->w, i->h, i->img.thumb .. );
				nk_mre_draw_image(i->x, i->y, i->w, i->h);
			} break;
			case NK_COMMAND_ARC:
			case NK_COMMAND_ARC_FILLED:
			default: break;
			}
			//Testing: Check as it draws..
			vm_graphic_flush_layer(layer_hdl, 1);
		}

		///*One last flush */
		vm_graphic_flush_layer(layer_hdl, 1); 

		/* Clear and Blit called */
		//nk_gdi_blit(gdi.window_dc);
		nk_clear(&mre.ctx);

	}

#endif


/**
MRE-Nuklear Components initialisation and return 
**/
struct mre_nk_c * mre_nk_c_create(char *type, char *title, int len, int hovering, char *url, char *thumb){
	/* Create the component */
	struct mre_nk_c *cmpnt;
	//cmpnt = (struct mre_nk_c *)malloc(sizeof(struct mre_nk_c));
	cmpnt = (struct mre_nk_c *)vm_malloc(sizeof(struct mre_nk_c));
	//cmpnt =	malloc(sizeof(struct mre_nk_c));
	/* Mem allocate the component */
	//cmpnt = malloc(sizeof(struct mre_nk_c));
	/* Mem allocate the component's variables */
	if(cmpnt != NULL){
		printf("Component mem-allocated OK.");
	}else{
		printf("\nSomething wrong with allocating memory to component");
	}
	cmpnt->title = vm_malloc(sizeof(char*)+ strlen(title) + 1);
	cmpnt->type = vm_malloc(sizeof(char*)+ strlen(type) + 1);
	cmpnt->hovering = vm_malloc(sizeof(int));
	cmpnt->len = vm_malloc(sizeof(int));

	cmpnt->hovering = 0;
	//Disabling for now:
	//cmpnt->hovering = hovering;
	cmpnt->title = title;
	cmpnt->type = type;
	if (strcmp(type, "button")>=0){
		cmpnt->is_hoverable = 1;
	}
	cmpnt->len = len;
	
	if(strcmp(type, "video")>=0){
		cmpnt->url = vm_malloc(sizeof(char*)+ strlen(url) + 1);
		cmpnt->thumbnail = vm_malloc(sizeof(char*) + strlen(thumb) + 1);
		cmpnt->url = url;
		cmpnt->thumbnail = thumb;
	}else{
		//Do it anyway (mem pointer allocation issue)
		cmpnt->url = vm_malloc(sizeof(char*)+ strlen(url) + 1);
		cmpnt->thumbnail = vm_malloc(sizeof(char*) + strlen(thumb) + 1);
		cmpnt->url = url;
		cmpnt->thumbnail = thumb;
	}
	return cmpnt;
}

/*	Loads the view again. Loads the components/widgets
	again and checks their state to basically start 
	to re-draw the whole thing again.
	I've seperated the call to the view.
	This method will set memory and go through
	the whole view to decide what needs to be 
	drawn and what goes on the screen at that time.
	Unfortunately this needs to be called every
	time the GUI needs refreshing. Whatever
	declared here will kind of start whats needed
	for rendering.

	The basic process is that As it reads the components
	declared in this method (ir: the widgets
	declared here) it will set if it can be drawn,
	each widget will have its own behaviour checks.
	For example button will check if it needs to 
	hover and if there is a click event. (Nuklear
	was kind of a mouse first GUI)

	TODO: Maybe make it take void and look at 
	mre_view.component object.. 
*/
void declare_view(struct mre_nk_c** ptr_cmpts){
	/*Declare context*/
    struct nk_context *ctx;

	/* get the set context */
	ctx = &mre.ctx;


	/* GUI: Start(will allocate mem for context, etc) */
    {
		struct nk_panel layout;
		if (nk_begin(ctx, &layout, "UstadMobile", nk_rect(10, 10, 
			(float)(mre.width - 20), (float)(mre.height - 20)),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE))
			//NK_WINDOW_SCALABLE|NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE
		{
			char *title;
			enum {EASY, HARD};
			static int op = EASY;
			static int property = 20;
			int i = 0;
			enum nk_button_behavior button_behavior;
			struct nk_image img;

			printf("Testing nk_text_wrap\n");
			//33 is the height of text 
			//33 is font size!
			//height = number of lines * 30
			nk_layout_row_static(ctx, 60 , mre.width - 50, 1);
			nk_text_wrap(ctx, 
				"The quick brown fox jumped over the lazy dog..",
				strlen("The quick brown fox jumped over the lazy dog..")
				);
			
			/* Default layout */
			//nk_layout_row_static(ctx, 30, mre.width, 1);

			/* mre_nk_c loop - Components Loop */
			
			for(i=0;i<=mre_view.components_count;i++){
				//Debug:
				//printf("\nComponent %d's title: %s",i,ptr_cmpts[i]->title);
				
				if(ptr_cmpts[i]->type == "button"){

					/* Button layout */
					nk_layout_row_static(ctx, 30, 80, 1);

					button_behavior = NK_BUTTON_DEFAULT;
					if (ptr_cmpts[i]->hovering == 1){
						//enum nk_button_behavior {NK_BUTTON_DEFAULT, NK_BUTTON_REPEATER, NK_BUTTON_HOVER};
						button_behavior = NK_BUTTON_HOVER;
					}
					if (i==mre_view.hoverable_counter){
						button_behavior = NK_BUTTON_HOVER;
					}else{
						button_behavior = NK_BUTTON_DEFAULT;
					}
					//nk_button_label(ctx, ptr_cmpts[i]->title, NK_BUTTON_HOVER);
					ctx->button_behavior = button_behavior;
					
					nk_button_label(ctx, ptr_cmpts[i]->title);
				}

				if(ptr_cmpts[i]->type == "text"){
					/* Text layout */
					//33 is the height of text 
					//33 is font size!
					//height = number of lines * 30
					//ctx->style.font->width
					
					//nk_layout_row_static(ctx, 60 , mre.width - 50, 1);
					//TODO: replace value with variable
					nk_layout_row_static(ctx, (7*strlen(ptr_cmpts[i]->title)/(mre.width - 45))* 30, 
						mre.width - 50, 1);
					nk_text_wrap(ctx, 
						ptr_cmpts[i]->title,
						strlen(ptr_cmpts[i]->title)
						);
				}

				if(ptr_cmpts[i]->type == "video"){
					/*	TODO: 
						Need a title to fill it in
						Need an image for thumbnail
						Need a play button 
						Need the video file path 
						Style: Center, Wide, 80%
						Hover should highlight with text "Click to play"
						Handle what happens when you come back from the video	
					*/ 

					/* Set Text Title on Top */
					nk_text(ctx, ptr_cmpts[i]->title,
						strlen(ptr_cmpts[i]->title),NK_TEXT_ALIGN_CENTERED);
					/*nk_text_wrap(ctx, 
						ptr_cmpts[i]->title,
						strlen(ptr_cmpts[i]->title)
						);
					*/

					/* Get the video thumbnail */


					/* Set Image
						Here is where we set the image object with an image id 
						That handle and other default values are 
					*/
					img = nk_image_id(1);
					/* Validates nk_image image object
						Checks bounds, Adds Image object to command
						queuing commands for the drawing process 
					*/ 
					nk_image(ctx, img);
					
				}
				
			}
			/**
			TODO:
				1. Figure out text - waiting for response on nuklear issue #250
				2. Do hover - Buttons, etc [Done]
				3. Do events in hover
				4. Have it loop without running out of memory
				5. Ship
			**/

		}
		nk_end(ctx);
	}

}

/**	Updates / Refreshes the View. 
This is a callable function that gets called whenever
we want the GUI to get updated. This function, clears 
the display for it to be rendered/painted again,(for now)
initialises all the view components (widgets), calls 
nuklear to start actually drawing, clears the drawing 
queue (cause its done with drawing)

Basically clears the screen and starts to work with 
the components.
	
**/
void update_gui(){
	/*Declare context*/
    struct nk_context *ctx;
	struct mre_nk_c* ptr_cmpts;

	VMUINT8 *buffer;

	/* Get components */
	ptr_cmpts = mre_view.components;

	/* get the target buffer from the layer */
	buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

	/* get the set context */
	ctx = &mre.ctx;

	printf("\nUpdating the current View..\n");

	/*	fill the screen
		Clears it before everything gets drawn bit by bit
	*/
	vm_graphic_fill_rect (buffer, MRE_SET_SRC_LEFT_TOP_X, MRE_SET_SRC_LEFT_TOP_Y, 
							vm_graphic_get_screen_width (), vm_graphic_get_screen_height (), 
							VM_COLOR_WHITE, VM_COLOR_BLACK);

	/* Testing commands to View */
	declare_view(ptr_cmpts);

	/*	Draw/Paint - Main render process
		Also set the color to Gray(30,30,30)
	*/
    nk_mre_render(nk_rgb(30,30,30));

	/*	Clears the drawing queue - After drawing
		The queue in context was adding up un necessarily
	*/
	nk_input_begin(ctx); 

}

/**
	This will initiate the GUI demo, the UI and initiate render
*/
void initiate_nk_gui(void){
	MreFont* font;
	VMUINT8 *buffer;

	/*	Declare context
		This is the nuklear context object.
		It has nk_input input, style, memory,
		clipboard, widget state flags
	*/
    struct nk_context *ctx;
	struct mre_nk_c * first_set_of_components;

    /* Variable declaration that are relevant to Implementation*/
    int running = 1;
    int needs_refresh = 1;
	//Some memory allocation issues in nk_mrefont_create..
	/*Memory Set and Allocation*/
	font = nk_mrefont_create("Arial", 12);


	/*Initialise the context*/ 
	ctx = nk_mre_init(font, WINDOW_WIDTH, WINDOW_HEIGHT);

	/* get the target buffer from the layer */
	buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);

	/* Updates mre_view */
	set_test_view();

	/* Refreshes display */
	update_gui();

}

/* Another page */
void set_test2_view(){
	//Initialise the pointers:
	struct mre_nk_c *b1,*b2,*b3, *b4, *b5, *b6;
	struct mre_nk_c *t1,*t2,*t3;
	struct mre_nk_c *ptr;
	struct mre_nk_c button1, button2, button3;

	struct mre_nk_c cmpnts[10];
	struct mre_nk_c *ptr_cmpts[10];
	struct mre_nk_c components[5];
	int i;
	int ptr_cmpts_count;

	b1 = mre_nk_c_create("button", "Mobilizing",0,0,"","");
	b2 = mre_nk_c_create("button", "Quality",0,1,"","");
	b3 = mre_nk_c_create("button", "Education",0,0,"","");
	b4 = mre_nk_c_create("button", "for", 0, 0,"","");
	b5 = mre_nk_c_create("button", "all", 0, 0,"","");
	b6 = mre_nk_c_create("button", "UstadMobile", 0, 0,"","");

	ptr_cmpts[0] = b1;
	ptr_cmpts[1] = b2;
	ptr_cmpts[2] = b3;
	ptr_cmpts[3] = b4;
	ptr_cmpts[4] = b5;
	ptr_cmpts[5] = b6;

	ptr_cmpts_count = 6;
	for(i=0;i<ptr_cmpts_count;i++){
		printf("\nComponent %d's title: %s",i,ptr_cmpts[i]->title);
	}

	/* Updating mre_view */
	mre_view.hoverable_counter = 0;
	mre_view.hoverable_items = 5;
	mre_view.components = ptr_cmpts;
	mre_view.components_count = 5;

	//OR we could send mre_view object as a whole..
	//return ptr_cmpts;
}

/* Updates mre_view to a test view */
void set_test_view(){
	//Initialise the pointers:
	struct mre_nk_c *b1,*b2,*b3;
	struct mre_nk_c *t1,*t2,*t3;
	struct mre_nk_c *v1;
	struct mre_nk_c *ptr;
	struct mre_nk_c button1, button2, button3;

	struct mre_nk_c cmpnts[10];
	struct mre_nk_c *ptr_cmpts[10];
	struct mre_nk_c components[5];
	int i;
	int ptr_cmpts_count;

	b1 = mre_nk_c_create("button", "Fox",0,0,"","");
	b2 = mre_nk_c_create("button", "Jumped",0,1,"","");
	b3 = mre_nk_c_create("button", "Dog",0,0,"","");
	t1 = mre_nk_c_create("text", "So then he went to the park to think about what just happened and couldn't keep that smile off of his dog's face..",
		strlen("So then he went to the park to think about what just happened and couldn't keep that smile off of his dog's face.."),0,"","");
	v1 = mre_nk_c_create("video", "How to UstadMobile",0,0,"small.mp4","/path/to/video_thumbnail.jpg");

	ptr_cmpts[0] = b1;
	ptr_cmpts[1] = b2;
	ptr_cmpts[2] = b3;
	ptr_cmpts[3] = t1;
	ptr_cmpts[4] = v1;

	ptr_cmpts_count = 5;
	for(i=0;i<ptr_cmpts_count;i++){
		printf("\nComponent %d's title: %s",i,ptr_cmpts[i]->title);
	}

	/* Updating mre_view */
	mre_view.hoverable_counter = 0;
	mre_view.hoverable_items = 4;
	mre_view.components = ptr_cmpts;
	mre_view.components_count = 4;

	//OR we could send mre_view object as a whole..
	//return ptr_cmpts;
}