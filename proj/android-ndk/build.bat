@echo off
echo ---BUILD STARTED---
echo.

echo Updating project...
call android update project --target "android-22" --path . || goto error
echo.

echo Compiling...
call ndk-build NDK_OUT=../../build/android-ndk/obj || goto error

echo.
echo ---BUILD COMPLETE---
exit /b

:error
echo.
echo Error occured, stopping execution.
echo ---BUILD FAILED---
pause
exit /b %errorlevel%