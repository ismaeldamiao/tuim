# The Tuim Project

## About this document

This document describe the Tuim project and the ABI that conforming programs
must follow.

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

## Donations

Did you like the project? Make a donation so that I can continue working on it.

- **GitHub Sponsors**: https://github.com/sponsors/ismaeldamiao

- **PayPal**: <https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=D66EM3DGU35EE>.

- **PIX**: `ismaellxd@gmail.com`.

