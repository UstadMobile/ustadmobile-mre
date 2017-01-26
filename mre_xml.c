/*
mre_xml.c: Xml related Definitions
*/
#include "share.h"
#include "mre_xml.h"

void read_file_as_xml(VMSTR file_path, VMFILE file_handle){
	printf("\nRead file as XML");
}

void get_component_from_xml_file(VMSTR file_path, VMFILE file_handle, VMSTR component){
	printf("\nGet component from XML file");
}

static XML_start_doctype_hdlr start_doctype_hdlr(void *data,
							const VMCHAR *doctypeName,
							const VMCHAR **sysid,
							const VMCHAR **pubid, 
							VMINT internal_subset, 
							VMINT error){
	printf("\nHello everybody!");
} 
static XML_end_doctype_hdlr end_doctype_hdlr(void *data,
                                        const VMCHAR *doctypeName,
                                        const VMCHAR **sysid,
										const VMCHAR **pubid, 
										VMINT internal_subset, 
										VMINT error){
	printf("\nHello eeverybody!");
} 
static XML_start_elem_hdlr start_element_hdlr (void *data, 
											   const VMCHAR *el, 
											   const VMCHAR **attr, 
											   VMINT error){
	printf("\n Hello EveryBody!");
}

static XML_end_elem_hdlr end_element_hdlr(	void *data, 
											const VMCHAR *el, 
											const VMCHAR **attr, 
											VMINT error){
	printf("\n Hello Everybody!");
}

static XML_data_hdlr data_hdlr(void *resv, 
							   const VMCHAR *el, 
							   const VMCHAR *data, 
							   VMINT len, 
							   VMINT error){
	printf("\n Hi everybody!");
}


void test_xml(){
	const VMSTR xml_file = "page.xhtml";	
	const VMSTR xml_file_path = "E:\\page.xhtml";
	VMINT xml_parse_ret, new_parser_ret;
	VM_XML_PARSER_STRUCT *parser;
	VMCHAR *file_contents;
	VMINT file_size;

	parser = (VM_XML_PARSER_STRUCT *)vm_malloc(sizeof(VM_XML_PARSER_STRUCT));

	new_parser_ret = vm_xml_new_parser(parser);
	if(new_parser_ret >=0 ){
		printf("\n Created XML Parser object OK..\n");
	}else{
		printf("\n Failed to create the XML Parser object.\n");
	}
	vm_xml_set_doctype_handlers(parser, start_doctype_hdlr, 
								end_doctype_hdlr);
	vm_xml_set_element_handlers(parser, start_element_hdlr, 
								end_element_hdlr);
	vm_xml_set_data_handler(parser, data_hdlr);

	//1. Get file's size
	//2. Allocate file_contents to that size
	//3. Read file contents
	//4. Parse from contents

	file_size = get_file_size(xml_file_path);
	file_contents = vm_malloc(file_size);
	mre_read_file(xml_file, file_contents);
	
	//xml_parse_ret =  vm_xml_parse(parser, "<h2>Hey Jude<i> Take a sad song </i> and make it <b> Better </b></h2>", 0);
	xml_parse_ret =  vm_xml_parse(parser, file_contents, 0);


	if (xml_parse_ret == VM_XML_RESULT_FAIL){
		printf("\n failed to parse XML");
	}else if(xml_parse_ret == VM_XML_RESULT_OK){
		printf("\n XML parsed OK\n");
	}else{
		printf("\n Something went wrong with parsing XML");
	}

	//Get some elements and build a UI from it..

	// Free allocations..
	vm_free(parser);
	vm_free(file_contents);
	printf("\nhi");


}
