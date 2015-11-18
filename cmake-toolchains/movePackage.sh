#! /bin/sh

echo "Build package with $1"
rm -rf ./*
mkdir -p ../deploy
mkdir -p ../deployLogs
THISLOG="../deployLogs/${1}.log"
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$1" .. > "${THISLOG}" && make package >> "${THISLOG}" || cat "${THISLOG}"
find . -maxdepth 1 \( -iname '*.tar.gz' -o -iname '*.zip' \) -print0 | xargs -0 mv -t "../deploy/"
