# Commands.
CC          := gcc
CFLAGS      := -std=c99 -O2 -Wall -Wextra -pedantic -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wwrite-strings -Waggregate-return -Wcast-qual -Wunreachable-code -Wformat=2 -Wmissing-prototypes -Wold-style-definition -Wpadded
RM          := rm -rf
DIFF        := diff -y --suppress-common-lines

# Files.
PROG        := ./a.out
DATA_FILE   := data_06.txt
RESULT_FILE := result_06.txt
SRC_FILE    := object.c human.c student.c score_board.c func.c prog_06.c
ANS_FILE    := answer_06.txt


.PHONY: run result test clean


run: $(PROG) $(DATA_FILE)
	$(PROG) < $(DATA_FILE)


result: $(RESULT_FILE)


test: $(ANS_FILE) $(RESULT_FILE)
	$(DIFF) $(ANS_FILE) $(RESULT_FILE)


clean:
	$(RM) $(PROG)


$(PROG): $(SRC_FILE)
	$(CC) $(CFLAGS) -o $(PROG) $(SRC_FILE)


$(RESULT_FILE): $(PROG) $(DATA_FILE)
	$(PROG) < $(DATA_FILE) > $@
