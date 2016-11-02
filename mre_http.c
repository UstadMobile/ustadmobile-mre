/*
mre_http.c Http related method
*/
#include "share.h"
#include "mre_http.h"

/*****************************************************************************
 * FUNCTION
 *  http_request_hook_cb
 * DESCRIPTION
 *  This function will receive response of http connection as a structure
 * PARAMETERS
 *  bResponse      [IN]       to ensure response has been arrived
 *  session        [IN]       structure variable containing http_session other parameters
 * RETURNS
 *	none This HAS TO BE VOID for it to work and get the session properly!
*****************************************************************************/
static void http_request_hook_cb(VMINT bResponse, void* pSession)
{
	if (pSession)
	{
		/* local variables */
		http_session_t *session = pSession;
		//	int i, count;
		//	VMCHAR *name, *value;
		VMFILE  file_handle;
		VMINT   response;
		VMUINT  bytes_written;
		VMWCHAR wfile_name[MRE_FILE_NAME_SIZE];
		VMCHAR  file_name[MRE_FILE_NAME_SIZE];

		/* function body */
		vertical_scrolling_text("Response Success....");
		/* to count head number of http_session response */
        /*	count = get_http_head_number(session);
		    name = vm_malloc(10000);
		    value = vm_malloc(10000);
		    for (i= MRE_INTIAL_VALUE; i< count; i++)
		    {
				get_http_head_by_index(session, i, name, value);
				vertical_scrolling_text(name);
				vertical_scrolling_text(value);	
			} */


		/* set  file location and open file */
		sprintf(file_name,"%c:\\%s",mre_get_drv(),"HTTP.TXT");
		vm_ascii_to_ucs2(wfile_name, 100, file_name);
		file_handle = vm_file_open(wfile_name, MODE_CREATE_ALWAYS_WRITE, FALSE);
		if(file_handle < 0)
		{
			vertical_scrolling_text("File Handle Fail....");
		}
		if (!session->resbuf)
		{
			vm_log_debug("empty head ");
			vertical_scrolling_text("empty head");
		}
		if ( !session->resbody)
		{
			vm_log_debug("empty body");
			vertical_scrolling_text("empty body");
		}
		/* write response header and body  in file */
		response = vm_file_write (file_handle, 
					session->resbuf,  //session->resbuf/resbody: the address of the data
					session->nresbuf, //nresbuf/nresbody:  bytes to be written
					&bytes_written);  //&bytes_written: Bytes actually written
		response = vm_file_write (file_handle, session->resbody, session->nresbody, &bytes_written);

		vm_log_debug("%d bytes has been written in file", bytes_written);
		vertical_scrolling_text("Response written. Press 0 for Main Menu");
		
		vm_file_close(file_handle);
		//vm_free(name);
		//vm_free(value);

		//mre_start_menu();
		//return *session;

		if (session->res_code == 200){
			vm_log_file("Login Success\n");
			vertical_scrolling_text("Login Success!");
		}else{
			vm_log_file("Login Fail\n");
			vertical_scrolling_text("Login Fail!");
		}
		//mre_start_menu();
	}
	else
	{
		vertical_scrolling_text("Response Failure....");	
	}
	

}

/****************************************************************************
* FUNCTION
*	make_http_request
* DESCRIPTION
*	Make a post request
*
*****************************************************************************/
void make_http_request(VMSTR method, VMSTR http_url, http_head_t head[], 
					   int headers_count, http_head_t post_parameters[], 
					   int post_parameters_count){

	/* local variables declare*/
	VMINT http_req_response; //http request response
	asyn_http_req_t req; //the request
	//VMWSTR params_bytes; //bytes of the parameter 
	//VMSTR params; //the POST parameter string
	VMINT params_size; //the size of the params
	VMINT post_parameter_string_size; //the size of the params string
	VMSTR content_type_name;
	VMSTR content_type_value;
	int http_request_t_size;
	int i;
	char to_append[200];
	char post_parameter_string[1000];
	
	/* initialise */
	strcpy(post_parameter_string, "");
	content_type_value= "application/x-www-form-urlencoded";
	content_type_name = "Content-Type";
	http_request_t_size = sizeof(http_request_t);
	post_parameter_string_size = 0;
	params_size = 0;

	/* set the request: set parameters required for http connection */
	if (strcmp(method, "POST") == 0 ){
		req.req_method = POST;	//set the request Method
	}else if (strcmp(method, "GET") == 0 ){
		req.req_method = GET;
	}

	if (strcmp(method,"POST") == 0){
		sprintf(head[headers_count].name, "Content-Type");
		sprintf(head[headers_count].value, "application/x-www-form-urlencoded");
		headers_count += 1;
	}

	for(i=0;i<post_parameters_count;i++){
		sprintf(to_append, "%s=%s",post_parameters[i].name, post_parameters[i].value);
		strcat(post_parameter_string, to_append);
		if(i<post_parameters_count - 1)
		{
			strcat(post_parameter_string, "&");
		}
	}

	if (post_parameters_count > 0){
		post_parameter_string_size = sizeof(char) * (strlen(post_parameter_string));
	}

	//Log start
	vm_log_file(" in make_http_request \n");
	
	
	req.use_proxy = HTTP_USE_CMNET;//_PRIORITY;
	req.http_request = 	(http_request_t*)vm_malloc(http_request_t_size); //allocate mem
		/*
		The C library function void *memset(void *str, int c, size_t n) copies the 
		character c (an unsigned char) to the first n characters of the string pointed 
		to, by the argument str. eg: memset("Who do you think you are?", 'O', 10)
		gives: OOOOOOOOOOWho do you think you are? 
		Note the O is 'O' which is int. If its 0 like below then?
		//MRE_INITIAL_VALUE is (0)
		//size of http_request_t is: 
		*/
    memset(req.http_request, MRE_INTIAL_VALUE, http_request_t_size); //set mem
	

	strcpy(req.http_request->url, http_url);
	req.http_request->nhead = 3;
	req.http_request->heads = head;
	
	if (post_parameters_count > 0 ){
		//memory allocate and set for request body:
		req.http_request->body = (char*)vm_malloc(post_parameter_string_size); //allocate mem
		memset(req.http_request->body, MRE_INTIAL_VALUE, post_parameter_string_size); //set mem
		req.http_request->nbody = (int)post_parameter_string_size;
		strcpy(req.http_request->body, post_parameter_string); // not &params because -> means we're dealing with values -> is the same as (*http_request.body) 
	}

	vertical_scrolling_text("HTTP start");

	/* create connection : Start an http request */
	http_req_response = vm_asyn_http_req(&req, 
 		http_request_hook_cb, //http_request_hook_cb:	This is the callback function: Gets called AFTER getting response
		http_state_notify_cb);//http_state_notify_cb: This is an event callback for connecting,ed,req send,get, etc

	if (http_req_response != ASYN_HTTP_REQ_ACCEPT_SUCCESS)
	{
		/* unable to create connection .. various reason of not able to create connection */
        switch (http_req_response)
        {
        case ASYN_HTTP_REQ_NOT_SUPPORT_METHOD:
            vertical_scrolling_text("ASYN_HTTP_REQ_NOT_SUPPORT_METHOD");
            break;
        case ASYN_HTTP_REQ_NOT_SUPPORT_HTTP_COMM:
            vertical_scrolling_text("ASYN_HTTP_REQ_NOT_SUPPORT_HTTP_COMM");
            break; 
        case ASYN_HTTP_REQ_ONLY_SUPPORT_PROXY:
            vertical_scrolling_text("ASYN_HTTP_REQ_ONLY_SUPPORT_PROXY");
            break;
        case ASYN_HTTP_REQ_NOT_ENOUGH_RESOURCE:
            vertical_scrolling_text("ASYN_HTTP_REQ_NOT_ENOUGH_RESOURCE");
            break;
        case ASYN_HTTP_REQ_URL_FORMAT_ERROR:
            vertical_scrolling_text("ASYN_HTTP_REQ_URL_FORMAT_ERROR");
            break;
        }
	}
	else
	{
		vertical_scrolling_text("HTTP REQ PASS");
	}
	vm_free(req.http_request);

	//return http_req_response;
}


/*****************************************************************************
* FUNCTION
*	um_login_controller
*   Current problem: It seems headers cant be more than 20 characters 
*    will need to figure out how to fit in xapi header version in..
*    for now we just return a 200
* DESCRIPTION
*	Checks login
* PARAMETERS
*	VMSTR username, VMSTR password
* RETURNS
*	Status code 
*****************************************************************************/
void um_login_controller(VMSTR username, VMSTR password){
	/* local variables declare */
	//VMINT http_return;
	VMCHAR url[MRE_STR_SIZE_MAX + 1];
	VMCHAR method[MRE_STR_SIZE_MAX + 1];
	http_head_t headers[3]; //Have to do headers_count + 1 such that you can add content type
	http_head_t post_parameters[2];	
	char authorization[250];
	char auth_base64[200];

	/* local variables initialise */
	int post_parameters_count = 2;
	int headers_count = 2;
	//Convert username:password to base64 
	sprintf(auth_base64, "a2FybWFraWQwMjprYXJtYWtpZDAy"); //Hardcoding for now
	sprintf(authorization, "Basic %s", auth_base64); //if not declared, messes up
	sprintf(method, "GET");
	//sprintf(method, "POST");
	//sprintf(url, "http://umcloud1.ustadmobile.com/umlrs/statements/?limit=1");
	sprintf(url, "http://umcloud1.ustadmobile.com/testrequest/");
	strcat(authorization, " hello");
	sprintf(headers[0].name, "X-Experience-API-Version");
	sprintf(headers[0].value, "1.0.1");
	sprintf(headers[1].name, "Authorization");		
	sprintf(headers[1].value, authorization);
	sprintf(headers[2].name, "");
	sprintf(headers[2].value,"");

	sprintf(post_parameters[0].name,"username");
	sprintf(post_parameters[0].value, username);
	sprintf(post_parameters[1].name,"password");
	sprintf(post_parameters[1].value, password);
	//post_parameters_count = 0;
	
	//http_return = 
	make_http_request(method,url,headers,headers_count,post_parameters,post_parameters_count);

	//return http_return;
}

/*****************************************************************************
 * FUNCTION
 *  http_state_notify_cb
 * DESCRIPTION
 *  This function will receive various http connection state after connection 
 *  has been established
 * PARAMETERS
 *  state          [IN]       state of http connection
 *  param          [IN]       
 *  session        [IN]       structure variable containing http_session other parameters
 * RETURNS
 *	none
*****************************************************************************/
static void http_state_notify_cb(VMINT state, VMINT param, void *session)
{
	/* function body */
	switch(state)
	{
	case HTTP_STATE_GET_HOSTNAME:
		vertical_scrolling_text("Getting Host Name...");
		vm_log_debug(" getting host name");
		break;
	case HTTP_STATE_CONNECTING:
		vertical_scrolling_text("Connecting");
		vm_log_debug(" connecting");
		break;
	case HTTP_STATE_SENDING:
		vertical_scrolling_text("Sending Request...");
		vm_log_debug(" sending request");
		break;
	case HTTP_STATE_RECV_STATUS:
		vertical_scrolling_text("Receiving Response Status...");
		vm_log_debug(" receiving response status");
		break;
	case HTTP_STATE_RECV_HEADS:
		vertical_scrolling_text("Receiving Response Header...");
		vm_log_debug("receiving response header");
		break;
	case HTTP_STATE_RECV_BODY:
		vertical_scrolling_text("Receiving Response Body");
		vm_log_debug(" receiving response body");
		break;	
	default:
		vertical_scrolling_text("Unknown Event");
		vm_log_debug(" unknown event");
		break;
	}
}


/*****************************************************************************
 * FUNCTION
 *  http_hook_cb
 * DESCRIPTION
 *  This function will receive response of http connection as a structure
 * PARAMETERS
 *  bResponse      [IN]       to ensure response has been arrived
 *  session        [IN]       structure variable containing http_session other parameters
 * RETURNS
 *	none
*****************************************************************************/
static void http_hook_cb(VMINT bResponse, void* pSession)
{
	if (pSession)
	{
		/* local variables */
		http_session_t *session = pSession;
		//	int i, count;
		//	VMCHAR *name, *value;
		VMFILE  file_handle;
		VMINT   response;
		VMUINT  bytes_written;
		VMWCHAR wfile_name[MRE_FILE_NAME_SIZE];
		VMCHAR  file_name[MRE_FILE_NAME_SIZE];

		/* function body */
		vertical_scrolling_text("Response Success....");
		/* to count head number of http_session response */
        /*	count = get_http_head_number(session);
		    name = vm_malloc(10000);
		    value = vm_malloc(10000);
		    for (i= MRE_INTIAL_VALUE; i< count; i++)
		    {
				get_http_head_by_index(session, i, name, value);
				vertical_scrolling_text(name);
				vertical_scrolling_text(value);	
			} */


		/* set  file location and open file */
		sprintf(file_name,"%c:\\%s",mre_get_drv(),"HTTP.TXT");
		vm_ascii_to_ucs2(wfile_name, 100, file_name);
		file_handle = vm_file_open(wfile_name, MODE_CREATE_ALWAYS_WRITE, FALSE);
		if(file_handle < 0)
		{
			vertical_scrolling_text("File Handle Fail....");
		}
		if (!session->resbuf)
		{
			vm_log_debug("empty head ");
			vertical_scrolling_text("empty head");
		}
		if ( !session->resbody)
		{
			vm_log_debug("empty body");
			vertical_scrolling_text("empty body");
		}
		/* write response header and body  in file */
		//session->resbuf/resbody: the address of the data
		//nresbuf/nresbody:  bytes to be written
		//&bytes_written: Bytes actually written
		response = vm_file_write (file_handle, session->resbuf, session->nresbuf, &bytes_written);
		response = vm_file_write (file_handle, session->resbody, session->nresbody, &bytes_written);

		vm_log_debug("%d bytes has been written in file", bytes_written);
		vertical_scrolling_text("Response written. Press 0 for Main Menu");
		
		vm_file_close(file_handle);
	//	vm_free(name);
	//	vm_free(value);
		mre_start_menu();
	}
	else
	{
		vertical_scrolling_text("Response Failure....");
	}

}
/*****************************************************************************
 * FUNCTION
 *  connect_google_http
 * DESCRIPTION
 *  This function connects http_connection
 * PARAMETERS
 *  none
 * RETURNS
 *	none
*****************************************************************************/
void connect_http(void) 
{
	/* local variables */
	VMINT http_req_response;
	asyn_http_req_t req;
    http_head_t head[1];

	/* function body */
	/* log information */
	vm_log_debug(" in connect_http ");
	/* set parameters required for http connection */
	req.req_method = GET;
	req.use_proxy = HTTP_USE_CMNET;//_PRIORITY;
	req.http_request = 	(http_request_t*)vm_malloc(sizeof(http_request_t));
	sprintf(head[0].name, "RANGEkdjdfjkjkdjkdkkdjkdkdfjkdkfjdkfdjkfjkd");		
	sprintf(head[0].value, "bytes=-1");

    memset(req.http_request, MRE_INTIAL_VALUE, sizeof(http_request_t));

    strcpy(req.http_request->url, MRE_HTTP_URL);

   	req.http_request->nhead = 1;
	req.http_request->heads = head;
	vertical_scrolling_text("HTTP start");
	/* create connection */
	http_req_response = vm_asyn_http_req(&req, http_hook_cb, http_state_notify_cb);
	if (http_req_response != ASYN_HTTP_REQ_ACCEPT_SUCCESS)
	{
		/* unable to create connection .. various reason of not able to create connection */
        switch (http_req_response)
        {
        case ASYN_HTTP_REQ_NOT_SUPPORT_METHOD:
            vertical_scrolling_text("ASYN_HTTP_REQ_NOT_SUPPORT_METHOD");
            break;
        case ASYN_HTTP_REQ_NOT_SUPPORT_HTTP_COMM:
            vertical_scrolling_text("ASYN_HTTP_REQ_NOT_SUPPORT_HTTP_COMM");
            break;
        case ASYN_HTTP_REQ_ONLY_SUPPORT_PROXY:
            vertical_scrolling_text("ASYN_HTTP_REQ_ONLY_SUPPORT_PROXY");
            break;
        case ASYN_HTTP_REQ_NOT_ENOUGH_RESOURCE:
            vertical_scrolling_text("ASYN_HTTP_REQ_NOT_ENOUGH_RESOURCE");
            break;
        case ASYN_HTTP_REQ_URL_FORMAT_ERROR:
            vertical_scrolling_text("ASYN_HTTP_REQ_URL_FORMAT_ERROR");
            break;
        }
	}
	else
	{
		vertical_scrolling_text("HTTP REQ PASS");
	}
	vm_free(req.http_request);
}

