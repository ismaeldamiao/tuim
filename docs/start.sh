#!/bin/sh

if [ $# -ne 1 ]; then
   PORT=3000
else
   PORT="$1"
fi

cd docs
exec python -m http.server ${PORT} --bind localhost
