all: main.pdf

.PHONY: main.pdf preview clean

clean:
	latexmk -C

main.pdf:
	latexmk --pdf -interaction=nonstopmode main.tex

preview:
	latexmk --pdf -pvc main.tex
