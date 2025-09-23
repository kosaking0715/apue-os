ROOT       ?= ..
PLATFORM   ?= $(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

EXTRALD    =
SO_NAME    = libapue_db.so.1
SO_LINK    = libapue_db.so

ifeq "$(PLATFORM)" "linux"
  LDCMD = $(CC) -shared -Wl,-soname,$(SO_NAME) -o $(SO_NAME) db.o -L$(ROOT)/lib -lapue -lc
else
  LDCMD = $(CC) -shared -o $(SO_NAME) db.o -L$(ROOT)/lib -lapue -lc
endif

ifeq "$(PLATFORM)" "freebsd"
  EXTRALD += -Wl,-rpath,.
endif
ifeq "$(PLATFORM)" "macos"
  EXTRALD += -Wl,-rpath,.
endif

CFLAGS += -I../include -DLINUX -D_GNU_SOURCE -fPIC

all: $(SO_NAME) t4

$(SO_NAME): db.c $(LIBAPUE)
	$(CC) $(CFLAGS) -c db.c
	$(LDCMD)
	ln -sf $(SO_NAME) $(SO_LINK)

t4: $(LIBAPUE) $(SO_NAME)
	$(CC) $(CFLAGS) -c -I. t4.c
	$(CC) $(EXTRALD) -Wl,-rpath,'$$ORIGIN' -o t4 t4.o -L$(ROOT)/lib -L. -lapue_db -lapue

clean:
	rm -f *.o a.out core temp.* t4 libapue_db.so.* *.dat *.idx $(SO_LINK)

include $(ROOT)/Make.libapue.inc
