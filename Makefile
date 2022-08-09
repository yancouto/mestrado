full:
	$(MAKE) -C source
ifndef NO_VALGRIND
	$(MAKE) memcheck -C source
endif
	cd docs; doxygen
	$(MAKE) full -C thesis
	$(MAKE) full -C thesis_english
ifndef NO_QUALI
	$(MAKE) full -C quali
endif
ifndef NO_DEFESA
	$(MAKE) full -C defesa
endif
