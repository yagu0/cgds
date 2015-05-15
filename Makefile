#http://stackoverflow.com/questions/6273608/how-to-pass-argument-to-makefile-from-command-line
#to reconstruct dependency rules or not

LIBRARY = libcgds.so
INSTALL_PREFIX = /usr/local

src:
	cd src && $(MAKE) && cd ..

test:
	cd test && $(MAKE) && cd ..

doc:
	cd doc && $(MAKE) && cd ..

clean:
	cd src && $(MAKE) clean && cd ..
	cd test && $(MAKE) clean && cd ..
	cd doc && $(MAKE) clean && cd ..

install:
#	if [ ! -e src/obj/libcds.so ]; then make src; fi
	cp src/obj/$(LIBRARY) $(INSTALL_PREFIX)/lib/
	mkdir -p $(INSTALL_PREFIX)/include/cgds
	cp src/*.h $(INSTALL_PREFIX)/include/cgds/

uninstall:
	rm -f ${INSTALL_PREFIX}/lib/${LIBRARY}
	[[ -d ${INSTALL_PREFIX}/include/cgds ]] && rm -rf ${INSTALL_PREFIX}/include/cgds

.PHONY: src test doc clean install uninstall
