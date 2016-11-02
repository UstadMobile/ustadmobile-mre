REM custom_prebuild.bat 
REM this is for call the ResEditor.exe to save the project resource, to generate ResID.h
REM  for avoid ResID.h is modified after build, this will effect debug

"C:\Program Files (x86)\MRE SDK V3.0.00\tools\ResEditor\CmdShell.exe" SAVE "C:\Users\Varuna\OneDrive\Ustad Mobile\Mediatek\MRE\Apps\MRE_API_Tests\MRE_API_Tests.vcproj"
if %errorlevel% == 0 (
 echo prebuild OK.
 exit 0
) else (
 echo prebuild error
 echo error code: %errorlevel%
 exit 1
)

