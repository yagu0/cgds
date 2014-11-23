#http://stackoverflow.com/questions/6273608/how-to-pass-argument-to-makefile-from-command-line
#to reconstruct dependency rules or not

LIBRARY = libcgds.so
INSTALL_PREFIX = /usr/local

src:
	cd src && make && cd ..

test:
	cd test && make && cd ..

doc:
	cd doc && make && cd ..

clean:
	cd src && make clean && cd ..
	cd test && make clean && cd ..
	cd doc && make clean && cd ..

install:
#	if [ ! -e src/obj/libcds.so ]; then make src; fi
	cp src/obj/$(LIBRARY) $(INSTALL_PREFIX)/lib/
	mkdir -p $(INSTALL_PREFIX)/include/cgds
	cp src/*.h $(INSTALL_PREFIX)/include/cgds/

uninstall:
	rm -f ${INSTALL_PREFIX}/lib/${LIBRARY}
	[[ -d ${INSTALL_PREFIX}/include/cgds ]] && rm -rf ${INSTALL_PREFIX}/include/cgds

.PHONY: src test doc clean install uninstall
