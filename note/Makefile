.PHONY: clean all exmaples

NAME = 	note
REFNAME = references


all: $(NAME).pdf
	- pkill -9 evince
	evince $(NAME).pdf &

$(NAME).pdf : $(NAME).tex $(REFNAME).bib $(REST)
	pdflatex $(NAME).tex
	bibtex $(NAME).aux
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

clean:
	@rm -rf auto *~ *.aux *.log *.blg *.bbl \
	$(NAME).pdf $(NAME).out \
	examples.pdf
