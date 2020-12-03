CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS =

all: testrealloc_s testrealloc_v testrealloc

testrealloc : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../General/general.h ../General/general.c ../General/bool.h
	$(CC) testboolarr.c Realloc/realloc.c ../General/general.c -o testrealloc -I./Realloc $(PRODUCTION) $(LDLIBS)

testrealloc_s : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../General/general.h ../General/general.c ../General/bool.h
	$(CC) testboolarr.c Realloc/realloc.c ../General/general.c -o testrealloc_s -I./Realloc $(SANITIZE) $(LDLIBS)

testrealloc_v : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../General/general.h ../General/general.c ../General/bool.h
	$(CC) testboolarr.c Realloc/realloc.c ../General/general.c -o testrealloc_v -I./Realloc $(VALGRIND) $(LDLIBS)



clean:
	rm -f testrealloc_s testrealloc_v testrealloc testlinked testlinked_s testlinked_v Linkeds.dot Linkeds.pdf Reallocs.dot Reallocs.pdf revstr_realloc revstr_linked

run: all
	./testrealloc_s
	valgrind ./testrealloc_v

































DOT := $(shell command -v dot 2> /dev/null)

Linkeds.dot:  testlinked
	./testlinked
Reallocs.dot: testrealloc
	./testrealloc

Linkeds.pdf:  testboolarr.c testlinked Linkeds.dot
ifndef DOT
    $(error "dot is not available please install graphviz")
endif
	dot -Tpdf -o Linkeds.pdf Linkeds.dot
Reallocs.pdf:  testboolarr.c testrealloc Reallocs.dot
ifndef DOT
    $(error "dot is not available please install graphviz")
endif
	dot -Tpdf -o Reallocs.pdf Reallocs.dot

pdfs : Linkeds.pdf Reallocs.pdf
