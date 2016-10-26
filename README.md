# UstadMobile Mediatek MRE Application

*Note: This repo is under development and currently in API testing and UI testing using MRE's implementation of Nuklear* 

#### Quick notes of an MRE app

An MRE app can have access to file system, play media, draw 2D graphics with a 2D engine, has keyboard, touch and pen access, play audio, record, have access to Network with http, tcp socket support, have 5 levels of logging, parse xmls and has its own simulator.  Mediatek has today ended support for MRE and even its tools and discussions making it quite hard to develop for this platform. 

The file extension for an MRE App is .vxp and it is a binary executable file that runs on an phone that has a Mediatek chip and supports MRE applications. 

During development, an MRE application gets built for x86 based processors that can run on the MRE simulator on Windows. In order to get the same project built for an actual MRE application, there is another step where we "make" the application that gives a vxp file that can be executed on the Mediatek MRE capable phone. 

An MRE application structured is based around events. At the start of the application you attach event handlers to system, key and pen events. It seems you cannot change the handlers again once they are assigned. 	

#### Development Setup

You will need a Windows machine, Visual Studio 2008 is recommended and works the best (thats how old the documentation is) and have an ARM compiler (ADS1.2, RVCT3.1). I couldn't get GCC to cooperate. 

The next step is to Download Mediatek MRE's SDK  from [here](http://www.ustadmobile.com/mre/MRE_SDK_3.0.00.20_Normal_Eng_0.zip) (since Mediatek has removed any reference, discussion or tools relating to MRE) and install it. This SDK will make sure the necessary include header files for MRE SDK are available to be included in your project. This will also install a Wizard tool that will make you a demo application. The SDK also comes with a simulator and a Visual Studio 2008 toolbar that gets installed on to Visual Studio. 

## Visual Studio Steps##

Get Visual Studio2008 / Visual C++ 2008 Express. Might work with newer versions but I haven't tried those.

Tools > Options> Projects and Solutions > VC++ Directories

$(VCInstallDir)include

C:\Program Files(x86)\MRE DSK V3.0.00\include

C:\Program Files(x86)\MRE SDK V3.0.00\include\service

![vs01](images\vs01.png)

 Library files: 

$(VCInstallDir)lib

$(WindowsSdkDir)\lib

C:\Program Files(x86)\MRE SDK V3.0.00\lib

 ![vs02](images\vs02.png)

 

Or edit the file in:

C:\Users\<username>\AppData\Local\Microsoft\VisualStudio\9.0\VCComponents.dat

In order to installthe "plugin" into Visual Studio:

Make a addin folder:

C:\Program Files(x86)\Microsoft Visual Studio 9.0\VC\bin\Addins

And copy the files :MRE.AddIn and MRE2.0.dll from C:\Program Files (x86)\MRE SDK V3.0.00\tools intothis folder

 ![vs03](images\vs03.png) ![vs04](images\vs04.png) 

Then tell VisualStudio to look for Add Ins in that folder by going to:

VS2008>Tools>Options> Environment > Add-in/Macros Security 

Here"Add.." the Add in File Path 

 ![vs05](images\vs05.png)

Restart VisualStudio.

The go to :

VS2008 > Tools> Add-in Manager and you can see the MRE toolbar in there.

 ![vs06](images\vs06.png)

 

If you build via MRE toolbar it will make the vxp file. Also You need to start VS2008as Administrator because it needs access to Program Files / MRE folder.

 ![build-success](images\build-success.png)

#### Folder tree structure of the Project after compilation

< Project >

​	├───arm                        	#ARMcompilationoutput                        

         						#Hereis where the vxp/o file will be

​	├───config                        	#Definitionsand ALL Configs

​	│   		└───Default    	#compilation+ project config

​							#Madewhen compiled

​	├───Debug                 		#Buildoutput

​	├───res                          	#Resources,localisation, etc

​	└───ResID                      	#ResourceEdition

​		       ResID.h                  	#Resourceeditor generates this

​							#DefinesResources

​	│  config.xml                        	#Configfile (pre build)

​	│  custom_prebuild.bat 	#ResEditor stuff

​	│  mre_def.h                        	#definesMRE params 

​	│  Packagedll.bat                	#RunMRE DllPackage.exe after build

​							#andcopy vpp file to MoDis

​							#MoDisis the MRE Simulator.

​	│  TestProject.c               	#Themain C file (same as C++)

​	│  TestProject.cpp                #Themain C++ file (same as C)

​	│  TestProject.def               	#Declaresthe module parameters for 

​							#theDLL.

​	│  TestProject.h                	#Themain header file

​							#Doesnthave anything much

​							#Justdefines 

​	│  TestProject.sln                	#TheVisual Studio Solution

​	│  TestProject.vcproj        	#TheVisual C Project

​	│  TestProject.vcproj.user	#TheVisual C User Project

#### Brief application start and flow

Application starts with vm_main(void) function. This bit initialises a lot of the things: layer handle, register system events, keyboard event, pen events (not needed) and importantly the MRE resources. Then the function handle_sysevt(VMINT message, VMINT param) is called. The variable 'message' has value 4 and 'param' as 0. The variable message is 4 which is: VM_MSG_CREATE. This breaks

Then message becomes 1 which is VM_MSG_PAINT

​	This is to repaint the screen
​	It creates a base layer that has the same size as the screen
​	It then sets the graphic clip area
​	Uses: vm_graphic_get_screen_width() and height

Then calls: draw_hello() which contains commands to draw the text on screen.

#### Setting up ARM compiler 

SDK doesn'trecognize GCC then you'll have to get RVCT3.1, obtain a license and the SDK seems to be happy about that. 

RealViewDevelopment Suite 3.1  RVDS 3.1下载地址 [ARM Download link](https://silver.arm.com/download/Development_Tools/RVDS/RealView:_RVDS_deprecated/RVS31-BN-00000-r3p1-00rel0/RVS31-BN-00000-r3p1-00rel0.tgz)

You need to have a license to proceed further. Obtain it.

To access this, hit the Settings/Options button on the MRE toolbar in Visual Studio or in the MRE SDK 3.0 application (MRELauncher.exe) after opening and loading your project. 

 ![armop01](images\armop01.png) ![armop02](images\armop02.png)

```
	ARM Processor: ARM7EJ-S
	Compiler Options:-O2 -g --split_sections --apcs=/fpic -cpu ARM7EJ-S -		D__MRE_COMPILER_RVCT__ 
	Linker options: --fpic --sysv --entry rvct_entry --first rvct_entry

```

### Using Nuklear to create a UI and render html on screen 

...

### Structure of this project 

The structure of the project is divided into Headers and corresponding Source files. Most of the header files have declaration and Source file have the definitions. Exception to "nuklear_mre.h" and "mre_menu.h" which are source files renamed as header files.

 ![structure](images\structure.JPG)

The main source file here is MRE_API_Tests.c where vm_main() is present which gets executed first. 

* mre_base64.h	:  string to base64 conversion related functions 
* mre_display.h       :  Display alerts, updates, popups, etc
* mre_events.h        :  Event handlers for this project 
* mre_file.h              :  File related functions
* mre_http.h            :  Network and http related functions
* mre_io.h                :  IO related functions (everything that doesnt fall in either file or http)
* mre_menu.h         :  Function declaration and definition for menu structure User Interface
* mre_tcp_socket.h :  Socket related network access functions 
* mre_video.h          :  Media esp video playback functions 
* mre_views.h          :  The logic for the views related to MRE using Nuklear
* nuklear.h               :  External nuklear library framework 
* nuklear_mre.h      :  MRE's implementation of nuklear GUI framework used in this project
* ResID.h                  :  For resources. We don't really need it at this point 
* share.h                  :  Common header file with SDK header include, global variables, and macros
* MRE_API_Tests.c  :  The main source file for the application where vm_main() is
* Remaining .c source files correspond to their .h header counterparts. 



 ![flowchart01](images\flowchart01.PNG)

#### Notes about the above flowchart

...

We expect this project to read epub files, parse them and display them so we have an epub reader that can 

	1. Read epub material, and show at least images, text, play video
	2. Be able to log in to UstadMobile's UMCloud portal and send usage statistics logs

