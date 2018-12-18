cd ..

robocopy /s assets\data bin\windows32\release\data
robocopy /s assets\logs bin\windows32\release\logs
robocopy /s assets\res bin\windows32\release\res
copy libs\SFML-2.4.2\bin\openal32_32.dll bin\windows32\release\openal32.dll