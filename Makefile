.PHONY: all
all:
	$(MAKE) -C source
ifndef NO_VALGRIND
	$(MAKE) memcheck -C source
endif
	cd docs; doxygen
	$(MAKE) -C thesis
	mv thesis/main.pdf docs/html/thesis.pdf

