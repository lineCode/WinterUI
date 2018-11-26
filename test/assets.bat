IF EXIST %~dp0cmake-build-debug (
XCOPY "%~dp0bin\*.dll" "%~dp0cmake-build-debug\" /R /Y /F /I /Q /S
)

IF EXIST %~dp0cmake-build-release (
XCOPY "%~dp0bin\*.dll" "%~dp0cmake-build-release\" /R /Y /F /I /Q /S
)
