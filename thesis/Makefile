all: main.pdf

.PHONY: main.pdf full clean pvc

clean:
	latexmk -C

main.pdf:
	latexmk --pdf main.tex

full:
	texliveonfly --arguments="-interaction=nonstopmode" main.tex
	latexmk --pdf main.tex
	cp main.pdf ../docs/html/thesis.pdf

pvc:
	latexmk --pdf -pvc -interaction=nonstopmode main.tex
