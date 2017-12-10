all: quali.pdf

.PHONY: quali.pdf clean full pvc

clean:
	latexmk -C

quali.pdf:
	latexmk --pdf -interaction=nonstopmode quali.tex

full:
	$(MAKE) full -C slides
	texliveonfly --arguments="-interaction=nonstopmode" quali.tex
	latexmk --pdf quali.tex
	cp quali.pdf ../docs/html/quali.pdf

pvc:
	latexmk --pdf -pvc -interaction=nonstopmode quali.tex
