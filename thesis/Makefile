all: main.pdf

TEX = $(wildcard *.tex **/*.tex **/**/*.tex)
LATEX_OPT = -interaction=nonstopmode -halt-on-error

deb: $(TEX)
	@echo $^

main.pdf: $(TEX) main.bib
	pdflatex $(LATEX_OPT) main.tex
	bibtex main.aux
	pdflatex $(LATEX_OPT) main.tex
	pdflatex $(LATEX_OPT) main.tex
