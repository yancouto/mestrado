all: main.pdf

.PHONY: main.pdf clean

clean:
	latexmk -C

main.pdf:
	latexmk --pdf main.tex
