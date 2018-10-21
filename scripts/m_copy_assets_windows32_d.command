cd "`dirname "$0"`"

cd ..

cp -r assets/data bin/windows32/debug/data
cp -r assets/logs bin/windows32/debug/logs
cp -r assets/res bin/windows32/debug/res
cp assets/dll/openal32_32.dll bin/windows32/debug/openal32.dll