.PHONY: all
all:
	$(MAKE) test_all -C source
	cd docs; doxygen
	$(MAKE) -C thesis
	mv thesis/main.pdf docs/html/thesis.pdf
