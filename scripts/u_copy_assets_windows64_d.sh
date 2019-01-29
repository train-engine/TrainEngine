#!/usr/bin/env bash

cd "$(dirname "$0")"

cd ..

mkdir -p bin/windows64/debug
cp -r assets/data/. bin/windows64/debug/data
cp -r assets/logs/. bin/windows64/debug/logs
cp -r assets/res/. bin/windows64/debug/res
cp libs/SFML-2.4.2/bin/openal32_64.dll bin/windows64/debug/openal32.dll
