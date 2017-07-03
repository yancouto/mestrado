.PHONY: all
all:
	$(MAKE) -C source
	cd docs; doxygen
	$(MAKE) -C thesis
	mv thesis/main.pdf docs/html/thesis.pdf
