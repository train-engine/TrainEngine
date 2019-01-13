cd ..

rmdir /s /q bin\windows64\debug\data
rmdir /s /q bin\windows64\debug\logs
rmdir /s /q bin\windows64\debug\res
del /f /q bin\windows64\debug\openal32.dll

rmdir /s /q bin\windows64\release\data
rmdir /s /q bin\windows64\release\logs
rmdir /s /q bin\windows64\release\res
del /f /q bin\windows64\release\openal32.dll

rmdir /s /q bin\windows32\debug\data
rmdir /s /q bin\windows32\debug\logs
rmdir /s /q bin\windows32\debug\res
del /f /q bin\windows32\debug\openal32.dll

rmdir /s /q bin\windows32\release\data
rmdir /s /q bin\windows32\release\logs
rmdir /s /q bin\windows32\release\res
del /f /q bin\windows32\release\openal32.dll

rmdir /s /q bin\linux\debug\data
rmdir /s /q bin\linux\debug\logs
rmdir /s /q bin\linux\debug\res

rmdir /s /q bin\linux\release\data
rmdir /s /q bin\linux\release\logs
rmdir /s /q bin\linux\release\res
