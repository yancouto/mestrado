.PHONY: all
all:
	$(MAKE) -C thesis
	$(MAKE) test_all -C source
	cd docs; doxygen
	mv thesis/main.pdf docs/html/thesis.pdf
