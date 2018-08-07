@echo off
echo ---INSTALL STARTED---
echo.

echo Removing previous installation...
rmdir ..\..\bin\android /s /q || goto error
echo.

echo Building .apk and installing...
call ant debug install || goto error

echo.
echo ---INSTALL COMPLETE---
exit /b

:error
echo.
echo Error occured, stopping execution.
echo ---INSTALL FAILED---
pause
exit /b %errorlevel%