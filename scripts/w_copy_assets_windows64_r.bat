cd ..

robocopy /s assets\data bin\windows64\release\data
robocopy /s assets\logs bin\windows64\release\logs
robocopy /s assets\res bin\windows64\release\res
copy assets\dll\openal32_64.dll bin\windows64\release\openal32.dll