#!/usr/bin/env bash

cd "$(dirname "$0")"

cd ..

mkdir -p bin/linux/debug
cp -r assets/data/. bin/linux/debug/data
cp -r assets/logs/. bin/linux/debug/logs
cp -r assets/res/. bin/linux/debug/res
