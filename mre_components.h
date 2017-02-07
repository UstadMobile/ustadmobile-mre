/*

	MRE Components : Header File
	The MRE Components file declares the component object that makes the view to draw using nuklear. 

	Author: Varuna Singh, UstadMobile

*/ 

	/*
		MRE-Nuklear Components
		Components used to draw
	*/
struct mre_nk_c {
	int id;
	char *type;	//type of component
	char *title;
	int len;
	//Placeholder. Not using style at the moment.
	//struct nk_style style; 
	int hovering;
	int is_hoverable;
	/* For Media assets */ 
	char *url;
	char *thumbnail;
};

	/* 
		MRE View Properties. The list of components and hoverable counter. 
		The view controller will process through this object.
	*/	
struct mre_view_p{
	int hoverable_counter;
	int hoverable_items;
	struct mre_nk_c **components;
	//struct mre_nk_c * components;
	int components_count;
}mre_view;


