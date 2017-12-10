full:
	$(MAKE) -C source
ifndef NO_VALGRIND
	$(MAKE) memcheck -C source
endif
	cd docs; doxygen
	$(MAKE) full -C thesis
ifndef NO_QUALI
	$(MAKE) full -C quali
endif

