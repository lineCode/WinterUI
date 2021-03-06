IF NOT EXIST %~dp0package (
mkdir %~dp0package
)

IF NOT EXIST %~dp0package\libs (
mkdir %~dp0package\libs
)

IF NOT EXIST %~dp0package\include (
mkdir %~dp0package\include
)

IF NOT EXIST %~dp0package\bin (
mkdir %~dp0package\bin
)

XCOPY "%~dp0bin\*.dll" "%~dp0package\bin\" /R /Y /F /I /Q

XCOPY "%~dp0libs\*.dll.a" "%~dp0package\libs\" /R /Y /F /I /Q

XCOPY "%~dp0*.hh" "%~dp0package\include\" /R /Y /F /I /Q