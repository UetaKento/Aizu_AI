# Commands.
CC          := gcc
CFLAGS      := -g -std=c99 -O2 -Wall -Wextra -pedantic -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wcast-qual -Wunreachable-code -Wformat=2 -Wmissing-prototypes -Wold-style-definition -Wpadded
RM          := rm -rf
DIFF        := diff -y --suppress-common-lines

# Files.
PROG        := ./a.out
DATA_FILE   := data.txt
RESULT_FILE := result.txt
SRC_FILE    := prog.c func.c
HEADER_FILE := prog.h
ANS_FILE    := answer.txt


.PHONY: run result test clean


run: $(PROG) $(DATA_FILE)
	$(PROG) < $(DATA_FILE)


result: $(RESULT_FILE)


test: $(ANS_FILE) $(RESULT_FILE)
	$(DIFF) $(ANS_FILE) $(RESULT_FILE)


clean:
	$(RM) $(PROG)


$(PROG): $(SRC_FILE) $(HEADER_FILE)
	$(CC) $(CFLAGS) -o $(PROG) $(SRC_FILE) -lm


$(RESULT_FILE): $(PROG) $(DATA_FILE)
	$(PROG) < $(DATA_FILE) > $@
