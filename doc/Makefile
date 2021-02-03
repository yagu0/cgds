all: html latex

html:
	doxygen Doxyfile

latex: html
	cd latex && pdflatex refman.tex && pdflatex refman.tex && cd ..

clean:
	rm -rf html latex

.PHONY: latex html clean
