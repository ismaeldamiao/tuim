#!/bin/sh

[ -d "tmp" ] || { mkdir "tmp" || exit $?; }
cd tmp

TEXINPUTS=../doc: BIBINPUTS=../doc: latexmk \
   -pdflatex \
   -silent -quiet \
   -f -g \
   -logfilewarninglist \
   ../doc/main.tex

exit $?
