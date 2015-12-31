all:
	$(MAKE) -C server all
	$(MAKE) -C client all

clean:
	$(MAKE) -C server clean
	$(MAKE) -C client clean

.PHONY: all clean $(SUBDIRS)
