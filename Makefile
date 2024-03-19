PARENT := parent
CHILD := child
COMPILER := gcc

PR_SRC := ./src
PR_OBJ := ./obj

FLAGS := -g -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

PR_CHILD_SRC :=  $(PR_SRC)/child/
PR_PARENT_SRC := $(PR_SRC)/parent/
PR_CHILD_OBJ :=  $(PR_OBJ)/child/
PR_PARENT_OBJ := $(PR_OBJ)/parent/

CHILD_SRC := $(wildcard $(PR_CHILD_SRC)*.c)
PARENT_SRC := $(wildcard $(PR_PARENT_SRC)*.c)

CHILD_OBJ := $(patsubst $(PR_CHILD_SRC)%.c, $(PR_CHILD_OBJ)%.o, $(CHILD_SRC))
PARENT_OBJ := $(patsubst $(PR_PARENT_SRC)%.c, $(PR_PARENT_OBJ)%.o, $(PARENT_SRC))

all : $(PARENT) $(CHILD)

$(PARENT) : $(PARENT_OBJ)
	$(COMPILER) $(PARENT_OBJ) -o  $(PARENT) $(FLAGS)

$(PR_PARENT_OBJ)%.o : $(PR_PARENT_SRC)%.c
	$(COMPILER) -c $< -o $@ $(FLAGS)

$(CHILD) : $(CHILD_OBJ)
	$(COMPILER) $(CHILD_OBJ) -o  $(CHILD) $(FLAGS)

$(PR_CHILD_OBJ)%.o : $(PR_CHILD_SRC)%.c
	$(COMPILER) -c $< -o $@ -g

remove :
	rm $(PARENT) $(CHILD) $(PARENT_OBJ) $(CHILD_OBJ)