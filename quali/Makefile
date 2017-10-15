all: quali.pdf

.PHONY: quali.pdf clean

clean:
	latexmk -C

quali.pdf:
	latexmk --pdf -interaction=nonstopmode quali.tex
