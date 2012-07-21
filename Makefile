
PREFIX ?= /usr
LIBDIR ?= $(PREFIX)/lib64
INCLUDEDIR ?= $(PREFIX)/include
VAPIDIR ?= $(PREFIX)/share/vala/vapi

NAME = glibrdf
MAJOR_VERSION = 0
EXTRA_VERSION = .0.0
SONAME = lib$(NAME).so.$(MAJOR_VERSION)
LIB = $(SONAME)$(EXTRA_VERSION)
REQUIRES = glib-2.0 gobject-2.0 raptor2 redland

CC = gcc
CFLAGS ?= -g -O -Wall
CFLAGS += -fPIC -std=c99 -D_XOPEN_SOURCE $(shell pkg-config --cflags $(REQUIRES))
LD = gcc
LDFLAGS ?= -Wl,--as-needed -Wl,-O1
LIBS = $(shell pkg-config --libs $(REQUIRES))

OBJECTS = glibrdf.o

.PHONY: all
all: $(LIB) $(NAME).pc

$(LIB): $(OBJECTS)
	gcc -shared -Wl,-soname,$(SONAME) $(LDFLAGS) -o $@ $^ $(LIBS)

%.pc: %.pc.in
	sed -e 's!@LIBDIR@!$(LIBDIR)!g' \
	    -e 's!@INCLUDEDIR@!$(INCLUDEDIR)!g' \
	    -e 's!@VERSION@!$(VERSION)!g' \
	    -e 's!@REQUIRES@!$(REQUIRES)!g' \
	    $< >$@

glibrdf.o: glibrdf.h

.PHONY: clean
clean:
	rm -f $(LIB) $(OBJECTS) $(NAME).pc

.PHONY: install
install:
	install -m 0755 -d $(DESTDIR)$(VAPIDIR)
	install -m 0644 $(NAME).vapi $(DESTDIR)$(VAPIDIR)
	install -m 0755 -d $(DESTDIR)$(INCLUDEDIR)
	install -m 0644 glibrdf.h $(DESTDIR)$(INCLUDEDIR)
	install -m 0755 -d $(DESTDIR)$(LIBDIR)/pkgconfig
	install -m 0644 $(NAME).pc $(DESTDIR)$(LIBDIR)/pkgconfig
	install -m 0755 -d $(DESTDIR)$(LIBDIR)
	install -m 0755 $(LIB) $(DESTDIR)$(LIBDIR)
	ldconfig -n $(DESTDIR)$(LIBDIR)
	ln -s $(LIB) $(DESTDIR)$(LIBDIR)/lib$(NAME).so
