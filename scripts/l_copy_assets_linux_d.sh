#!/bin/bash

cd "`dirname "$0"`"

cd ..

cp -r assets/data bin/linux/debug/data
cp -r assets/logs bin/linux/debug/logs
cp -r assets/res bin/linux/debug/res