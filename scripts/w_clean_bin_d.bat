cd ..

del /f /q bin\windows64\debug\*.exe
del /f /q bin\windows32\debug\*.exe
del /s /q bin\macos\Debug\*
del /f /q bin\linux\debug\*.
del /s /q bin\ios\Debug\*
del /s /q bin\ios\Debug-iphonesimulator\*
del /f /q bin\android\TrainEngine-debug-unaligned.apk
del /f /q bin\android\TrainEngine-debug-unaligned.apk.d
del /f /q bin\android\TrainEngine-debug.apk