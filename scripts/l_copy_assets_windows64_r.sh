#!/bin/bash

cd "$(dirname "$0")"

cd ..

cp -r assets/data/. bin/windows64/release/data
cp -r assets/logs/. bin/windows64/release/logs
cp -r assets/res/. bin/windows64/release/res
cp libs/SFML-2.4.2/bin/openal32_64.dll bin/windows64/release/openal32.dll
