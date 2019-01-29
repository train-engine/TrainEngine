#!/usr/bin/env bash

cd "$(dirname "$0")"

cd ..

mkdir -p bin/windows32/debug
cp -r assets/data/. bin/windows32/debug/data
cp -r assets/logs/. bin/windows32/debug/logs
cp -r assets/res/. bin/windows32/debug/res
cp libs/SFML-2.4.2/bin/openal32_32.dll bin/windows32/debug/openal32.dll
