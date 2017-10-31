all: quali.pdf

.PHONY: quali.pdf clean

clean:
	latexmk -C

quali.pdf:
	latexmk --pdf -interaction=nonstopmode quali.tex

preview:
	latexmk --pdf -pvc -interaction=nonstopmode quali.tex
