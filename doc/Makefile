all: latex/refman.pdf

latex/refman.pdf:
	doxygen Doxyfile
	cd latex && pdflatex refman.tex && pdflatex refman.tex && cd ..

clean:
	rm -rf html latex

.PHONY: latex/refman.pdf clean
