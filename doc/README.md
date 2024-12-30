# Tuim Developer Kit

## About this document

This document describe the utilities provided by a conforming
Tuim Developer Kit.

## Building

To build the document you may use
[latexmk](https://ctan.org/pkg/latexmk/).

```
mkdir .build && cd .build
TEXINPUTS=..: BIBINPUTS=..: latexmk \
   -pdflatex \
   -silent -quiet \
   -f -g \
   -logfilewarninglist \
   ../main.tex
```
