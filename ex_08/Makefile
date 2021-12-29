# Commands.
PROLOG := sicstus --noinfo --nologo
RM     := rm -rf
DIFF   := diff -y --suppress-common-lines

# Files.
DATA_FILE   := data_08.txt
RESULT_FILE := result_08.txt
ANS_FILE    := answer_08.txt
SRC_FILE    := prog_08.pl
PROG        := $(PROLOG) -l $(SRC_FILE)


.PHONY: run result test


run: $(SRC_FILE) $(DATA_FILE)
	$(PROG) < $(DATA_FILE)


result: $(RESULT_FILE)


test: $(ANS_FILE) $(RESULT_FILE)
	$(DIFF) $(ANS_FILE) $(RESULT_FILE)


$(RESULT_FILE): $(SRC_FILE) $(DATA_FILE)
	$(PROG) < $(DATA_FILE) 2> $@
