CC := gcc
CFLAGS := -Werror -Wall -Wextra -std=c11 -MMD -MP -g -O0

INCLUDES = -I../includes
LIBS = -lpthread -lcheck -lsubunit -lm

LIB_STATIC = ../$(TARGET)

TEST_DIR := tests
TEST := test

SRC = $(wildcard *.c)
OBJ := $(SRC:%.c=../$(OBJ_DIR)/$(TEST_DIR)/%.o)
DEPS := $(OBJ:.o=.d)

.PHONY: all build_dir

all: build_dir $(TEST)

build_dir:
	@mkdir -p ../$(OBJ_DIR)/$(TEST_DIR)

$(TEST): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIB_STATIC) $(LIBS) -o ../$(BIN_DIR)/$@
	@echo "--- build target $(BIN_DIR)/$@ ---"

../$(OBJ_DIR)/$(TEST_DIR)/%.o: %.c
	@echo "Compile $< into $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)