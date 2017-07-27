all:
	pdflatex main.tex
	pdflatex main.tex
	mv main.pdf Dokumentation.pdf
	$(MAKE) clean

clean:
	rm -rf *.aux *.log *.out *.toc

cleanAll:
	$(MAKE) clean
	rm -rf *.pdf
