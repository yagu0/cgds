LIBRARY = libcgds.so
INSTALL_PREFIX = /usr/local

all: src

src:
	cd src && $(MAKE) && cd ..

test:
	cd test && ./makeMain.sh && $(MAKE) && cd ..

doc:
	cd doc && $(MAKE) && cd ..

clean:
	cd src && $(MAKE) clean && cd ..
	cd test && $(MAKE) clean && cd ..
	cd doc && $(MAKE) clean && cd ..

install:
	mkdir -p $(INSTALL_PREFIX)/lib/
	cp src/obj/$(LIBRARY) $(INSTALL_PREFIX)/lib/
	mkdir -p $(INSTALL_PREFIX)/include/cgds
	cp src/*.h $(INSTALL_PREFIX)/include/cgds/
	mv $(INSTALL_PREFIX)/include/cgds/cgds.h $(INSTALL_PREFIX)/include/

uninstall:
	rm -f ${INSTALL_PREFIX}/lib/${LIBRARY}
	rm -f ${INSTALL_PREFIX}/include/cgds.h
	[[ -d ${INSTALL_PREFIX}/include/cgds ]] && rm -rf ${INSTALL_PREFIX}/include/cgds

.PHONY: src test doc clean install uninstall
