cd ..

robocopy /s assets\data bin\windows32\debug\data
robocopy /s assets\logs bin\windows32\debug\logs
robocopy /s assets\res bin\windows32\debug\res
copy libs\SFML-2.4.2\bin\openal32_32.dll bin\windows32\debug\openal32.dll
