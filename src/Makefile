CC 				:=	gcc

# ==================	Configuration 	======= #
ASAN 			:= 	#-fsanitize=address
STDFLAGS 		:=	-g -c -Wall -Wextra -std=c11 #-Werror
# ============================================= #

TARGET 			?=	s21_decimal.a
TEST_TARGET		?=	test_s21_decimal.a
TEST_EXE 		?=	test
TEST_GCOV_EXE	?=	test_gcov
GCOV_TARGET		 =	./coverage
GCOV_INFO		 =	coverage.info

MODULES 		:=	$(shell find . -maxdepth 1 -type d | grep -v -E "docker|objs|tests")
TEST_MODULES 	:=	$(shell find tests -maxdepth 1 -type d | grep -v -E "objs")

SRC_DIR 		?=	$(MODULES)
TEST_SRC_DIR	?=	$(TEST_MODULES)

ALL_OBJS		:=	$(shell find . -name "*.o")
OBJ_DIR 		?=	./objs
GCOV_OBJ_DIR	 =	./tests/gcov_obj

INC 			:=	$(shell find $(SRC_DIR) -maxdepth 1 -name "s21_*.h")
SRC 			:=	$(shell find $(SRC_DIR) -maxdepth 1 -name "s21_*.c")

TEST_INC		:=	$(shell find $(TEST_SRC_DIR) -maxdepth 1 -name "*.h")
TEST_SRC 		:=  $(shell find $(TEST_SRC_DIR) -maxdepth 1 -name "*.c")

OBJS 			:=	$(notdir $(SRC:.c=.o))
TEST_OBJS 		:=	$(notdir $(TEST_SRC:.c=.o))
GCOV_OBJS 		:= 	$(notdir $(SRC:.c=.o))
GCOV_OBJS 		+= 	$(notdir $(TEST_SRC:.c=.o))

GCDA 			= 	$(shell find . -name "*.gcda")
GCNO 			= 	$(shell find . -name "*.gcno")

# VPath configuration. Order of args is their priority. 
vpath %.c $(MODULES) : $(TEST_MODULES)
vpath %.o $(OBJ_DIR) : $(GCOV_OBJ_DIR)

# Default system utilities
AR 	:=	ar rc
RAN :=	ranlib
RM 	:= 	rm -f
MK 	:=	mkdir -p

# Setting flags
CFLAGS 		?= 	$(STDFLAGS) $(ASAN)
TST_CFLAGS 	:= 	$(STDFLAGS) #$(shell pkg-config --cflags check)
GCOV_FLAGS 	?=	-fprofile-arcs -ftest-coverage
TST_LIBS 	:=	$(shell pkg-config --libs --cflags check) $(shell pkg-config --libs --cflags gmp)

all 			: 	Makefile $(TARGET) test # gcov_report 

lint 			: Makefile
				-cp ../materials/linters/CPPLINT.cfg .
				-find . -type f -name "*.c" | xargs python3 ../materials/linters/cpplint.py --extensions=c
				-find . -type f -name "*.h" | xargs python3 ../materials/linters/cpplint.py --extensions=c
				-find . -type f -name "*.c" | xargs cppcheck --enable=all --suppress=missingIncludeSystem
				rm -f CPPLINT.cfg

$(TARGET) 		: $(OBJS) Makefile
				$(AR) $(TARGET) $(OBJ_DIR)/*
				$(RAN) $(TARGET)

$(TEST_TARGET)	: $(GCOV_OBJS) $(INC)
				$(AR) $(TEST_TARGET) $(GCOV_OBJ_DIR)/*
				$(RAN) $(TEST_TARGET)

test 			: $(TARGET) $(TEST_OBJS) $(TEST_INC) Makefile
				$(CC) $(OBJ_DIR)/* $(ASAN) $(GCOV_FLAGS) -o $(TEST_EXE) $(TST_LIBS) -L. $(TARGET)
				./test

test_gcov 		: $(TEST_TARGET) $(GCOV_OBJS) Makefile
				$(CC) $(GCOV_OBJ_DIR)/* $(GCOV_FLAGS) -o $(TEST_GCOV_EXE) $(TST_LIBS) -L. $(TEST_TARGET)

%.o 			: %.c $(INC) 
				@$(MK) $(OBJ_DIR)
				$(CC) $(CFLAGS) -o $(OBJ_DIR)/$(notdir $@) -c $<

gcov_report: $(GCOV_TARGET)

$(GCOV_TARGET) 	: OBJ_DIR=$(GCOV_OBJ_DIR)
$(GCOV_TARGET) 	: CFLAGS += $(GCOV_FLAGS)
$(GCOV_TARGET) 	: $(GCOV_INFO)
				genhtml $(GCOV_INFO) -o $(GCOV_TARGET)

$(GCOV_INFO) 	: test_gcov
				./$(TEST_GCOV_EXE)
				geninfo $(GCOV_OBJ_DIR) -b . -o ./$(GCOV_INFO)

open 			:
				open coverage/index.html

clean 			:
				$(RM) $(ALL_OBJS)
				$(RM) $(GCDA)
				$(RM) $(GCNO)

fclean 			: clean
				$(RM) $(TARGET)
				$(RM) $(TEST_TARGET)
				$(RM) $(TEST_EXE)
				$(RM) $(TEST_GCOV_EXE)
				$(RM) $(GCOV_INFO)
				$(RM) -r $(GCOV_TARGET)

re 				: fclean all

.PHONY 			: all clean fclean re open 
