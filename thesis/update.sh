#!/bin/bash
pdflatex main.tex >/dev/null
bibtex main.aux >/dev/null
pdflatex main.tex >/dev/null
scp main.pdf yancouto@ime.usp.br:~/www/stuff/tese.pdf
