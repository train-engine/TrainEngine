cd "`dirname "$0"`"

cd ..

cp -r assets/data bin/windows32/release/data
cp -r assets/logs bin/windows32/release/logs
cp -r assets/res bin/windows32/release/res
cp libs/SFML-2.4.2/bin/openal32_32.dll bin/windows32/release/openal32.dll