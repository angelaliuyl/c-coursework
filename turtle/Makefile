CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -Werror -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

SDL = $(COMMON) `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90 $(LDLIBS) `sdl2-config --libs`

parser: turtle.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) turtle.c ADTs/General/general.c -o parser $(PRODUCTION) $(LDLIBS)

interp: turtle.c neillsdl2.c neillsdl2.h ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) turtle.c -DINTERPRETER=1 ADTs/General/general.c neillsdl2.c -o interp $(SDL)

parser_s: turtle.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) turtle.c ADTs/General/general.c -o parser_s $(SANITIZE) $(LDLIBS)

parser_v: turtle.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) turtle.c ADTs/General/general.c -o parser_v $(VALGRIND) $(LDLIBS)

# make debug version of interp where SDL is not on and more information is provided
interp_d: turtle.c neillsdl2.c neillsdl2.h ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) turtle.c -DINTERPRETER=1 -DDEBUG=1 -DTEST=1 ADTs/General/general.c neillsdl2.c -o interp_d $(SANITIZE) $(LDLIBS)

# make testing version of interp where SDL is not on
interp_t: turtle.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) turtle.c -DINTERPRETER=1 -DTEST=1 ADTs/General/general.c -o interp_t $(SANITIZE) $(LDLIBS)

# make extension parser
ext_pars: extension.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) extension.c ADTs/General/general.c -o ext_pars $(SANITIZE) $(LDLIBS)

# make testing version of extensions interpreter where SDL is not on
ext_itest: extension.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) extension.c -DINTERPRETER=1 -DTEST=1 ADTs/General/general.c -o ext_itest $(SANITIZE) $(LDLIBS)

# make extension interpreter
extension: extension.c ADTs/General/bool.h ADTs/General/general.h ADTs/General/general.c
	$(CC) extension.c -DINTERPRETER=1 ADTs/General/general.c -o ext_interp $(SDL)

test_p:
	make parser_s
	./parser_s simp.ttl
	make parser_v
	valgrind ./parser_v simp.ttl
	make parser
	rm -f test_p.txt
	printf "File: simp.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser simp.ttl 2>&1 | tee -a test_p.txt
	printf "File: simp_c.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser simp_c.ttl 2>&1 | tee -a test_p.txt
	printf "File: spiral1_c.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser spiral1_c.ttl 2>&1 | tee -a test_p.txt
	printf "File: spiral2_c.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser spiral2_c.ttl 2>&1 | tee -a test_p.txt
	printf "File: test1.ttl\n\t Expected output : No '{' at beginning of file\n\t Actual output : " >> test_p.txt
	./parser test1.ttl 2>&1 | tee -a test_p.txt
	printf "File: test2.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test2.ttl 2>&1 | tee -a test_p.txt
	printf "File: test3.ttl\n\t Expected output : Instruction not recognised.\n\t Actual output : " >> test_p.txt
	./parser test3.ttl 2>&1 | tee -a test_p.txt
	printf "File: test4.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test4.ttl 2>&1 | tee -a test_p.txt
	printf "File: test5.ttl\n\t Expected output : Variable not character A - Z\n\t Actual output : " >> test_p.txt
	./parser test5.ttl 2>&1 | tee -a test_p.txt
	printf "File: test6.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test6.ttl 2>&1 | tee -a test_p.txt
	printf "File: test7.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test7.ttl 2>&1 | tee -a test_p.txt
	printf "File: test8.ttl\n\t Expected output : Unrecognised operator\n\t Actual output : " >> test_p.txt
	./parser test8.ttl 2>&1 | tee -a test_p.txt
	printf "File: test9.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test9.ttl 2>&1 | tee -a test_p.txt
	printf "File: test10.ttl\n\t Expected output : Variable not character A - Z\n\t Actual output : " >> test_p.txt
	./parser test10.ttl 2>&1 | tee -a test_p.txt
	printf "File: test11.ttl\n\t Expected output : Variable more than 1 char\n\t Actual output : " >> test_p.txt
	./parser test11.ttl 2>&1 | tee -a test_p.txt
	printf "File: test12.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test12.ttl 2>&1 | tee -a test_p.txt
	printf "File: test13.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test13.ttl 2>&1 | tee -a test_p.txt
	printf "File: test14.ttl\n\t Expected output : Incorrect number format in instruction\n\t Actual output : " >> test_p.txt
	./parser test14.ttl 2>&1 | tee -a test_p.txt
	printf "File: test15.ttl\n\t Expected output : DO instruction: incorrect format, expecting FROM\n\t Actual output : " >> test_p.txt
	./parser test15.ttl 2>&1 | tee -a test_p.txt
	printf "File: test16.ttl\n\t Expected output : DO instruction: incorrect format, expecting TO\n\t Actual output : " >> test_p.txt
	./parser test16.ttl 2>&1 | tee -a test_p.txt
	printf "File: test17.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_p.txt
	./parser test17.ttl 2>&1 | tee -a test_p.txt
	printf "File: test18.ttl\n\t Expected output : DO instruction: incorrect format, expecting {\n\t Actual output : " >> test_p.txt
	./parser test18.ttl 2>&1 | tee -a test_p.txt
	printf "File: test19.ttl\n\t Expected output : Reached EOF without '}'\n\t Actual output : " >> test_p.txt
	./parser test19.ttl 2>&1 | tee -a test_p.txt
	printf "File: test20.ttl\n\t Expected output : SET instruction : expecting :=\n\t Actual output : " >> test_p.txt
	./parser test20.ttl 2>&1 | tee -a test_p.txt

test_i:
	make interp_t
	rm -f test_i.txt
	printf "File: simp.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t simp.ttl 2>&1 | tee -a test_i.txt
	printf "File: simp_c.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t simp_c.ttl 2>&1 | tee -a test_i.txt
	printf "File: spiral1_c.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t spiral1_c.ttl 2>&1 | tee -a test_i.txt
	printf "File: spiral2_c.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t spiral2_c.ttl 2>&1 | tee -a test_i.txt
	printf "File: test1.ttl\n\t Expected output : No '{' at beginning of file\n\t Actual output : " >> test_i.txt
	./interp_t test1.ttl 2>&1 | tee -a test_i.txt
	printf "File: test2.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t test2.ttl 2>&1 | tee -a test_i.txt
	printf "File: test3.ttl\n\t Expected output : Instruction not recognised.\n\t Actual output : " >> test_i.txt
	./interp_t test3.ttl 2>&1 | tee -a test_i.txt
	printf "File: test4.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t test4.ttl 2>&1 | tee -a test_i.txt
	printf "File: test5.ttl\n\t Expected output : Variable not character A - Z\n\t Actual output : " >> test_i.txt
	./interp_t test5.ttl 2>&1 | tee -a test_i.txt
	printf "File: test6.ttl\n\t Expected output : Incorrect Polish notation, not enough numbers in stack\n\t Actual output : " >> test_i.txt
	./interp_t test6.ttl 2>&1 | tee -a test_i.txt
	printf "File: test7.ttl\n\t Expected output : Incorrect Polish notation, numbers left on stack\n\t Actual output : " >> test_i.txt
	./interp_t test7.ttl 2>&1 | tee -a test_i.txt
	printf "File: test8.ttl\n\t Expected output : Unrecognised operator\n\t Actual output : " >> test_i.txt
	./interp_t test8.ttl 2>&1 | tee -a test_i.txt
	printf "File: test9.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t test9.ttl 2>&1 | tee -a test_i.txt
	printf "File: test10.ttl\n\t Expected output : Variable not character A - Z\n\t Actual output : " >> test_i.txt
	./interp_t test10.ttl 2>&1 | tee -a test_i.txt
	printf "File: test11.ttl\n\t Expected output : Variable more than 1 char\n\t Actual output : " >> test_i.txt
	./interp_t test11.ttl 2>&1 | tee -a test_i.txt
	printf "File: test12.ttl\n\t Expected output : Variable is undefined when referenced\n\t Actual output : " >> test_i.txt
	./interp_t test12.ttl 2>&1 | tee -a test_i.txt
	printf "File: test13.ttl\n\t Expected output : Parsed OK\n\t Actual output : " >> test_i.txt
	./interp_t test13.ttl 2>&1 | tee -a test_i.txt
	printf "File: test14.ttl\n\t Expected output : Incorrect number format in instruction\n\t Actual output : " >> test_i.txt
	./interp_t test14.ttl 2>&1 | tee -a test_i.txt
	printf "File: test15.ttl\n\t Expected output : DO instruction: incorrect format, expecting FROM\n\t Actual output : " >> test_i.txt
	./interp_t test15.ttl 2>&1 | tee -a test_i.txt
	printf "File: test16.ttl\n\t Expected output : DO instruction: incorrect format, expecting TO\n\t Actual output : " >> test_i.txt
	./interp_t test16.ttl 2>&1 | tee -a test_i.txt
	printf "File: test17.ttl\n\t Expected output : DO instruction: variable goes from higher number to lower number\n\t Actual output : " >> test_i.txt
	./interp_t test17.ttl 2>&1 | tee -a test_i.txt
	printf "File: test18.ttl\n\t Expected output : DO instruction: incorrect format, expecting {\n\t Actual output : " >> test_i.txt
	./interp_t test18.ttl 2>&1 | tee -a test_i.txt
	printf "File: test19.ttl\n\t Expected output : Reached EOF without '}'\n\t Actual output : " >> test_i.txt
	./interp_t test19.ttl 2>&1 | tee -a test_i.txt
	printf "File: test20.ttl\n\t Expected output : SET instruction : expecting :=\n\t Actual output : " >> test_i.txt
	./interp_t test20.ttl 2>&1 | tee -a test_i.txt
