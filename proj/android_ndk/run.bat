@echo off

echo Running...
call adb shell am start -a android.intent.action.MAIN -n com.papertrain.trainengine/android.app.NativeActivity

exit /b