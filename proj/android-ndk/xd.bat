@echo off

call build || goto error
echo.

call install || goto error
echo.

call run

echo.
echo ---XD COMPLETE---
pause
exit /b

:error
exit /b