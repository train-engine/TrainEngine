cd ..

robocopy /s assets\data bin\windows64\debug\data
robocopy /s assets\logs bin\windows64\debug\logs
robocopy /s assets\res bin\windows64\debug\res
copy assets\dll\openal32_64.dll bin\windows64\debug\openal32.dll