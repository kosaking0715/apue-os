$(LIBAPUE):
	(cd $(ROOT)/lib && $(MAKE) clean)
	(cd $(ROOT)/lib && $(MAKE) CFLAGS+=' -fPIC')
