CC:= gcc
CFLAGS= -Werror -Wall -Wextra -std=c11 -Iincludes -MMD -MP

export BIN_DIR:= bin
export BUILD_DIR:= build
export OBJ_DIR:= $(BUILD_DIR)/obj
export SRC_DIR:= src

export CK_FORK = yes

SRCS:= $(wildcard $(SRC_DIR)/*.c)
OBJS:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS:= $(OBJS:.o=.d)

export TARGET:= $(BIN_DIR)/qlog.a

.phony: all clean debug test build_dir debug release format

all: $(TARGET)

build_dir:
	@echo $(OBJS)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(TARGET): build_dir $(OBJS)
	ar rsc $@ $(OBJS)
	@echo "--- Created library in $@ ---"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compile $< into $@"
	@$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

test: $(TARGET)
	$(MAKE) -C tests

run: test
	@echo "--- Running tests ---"
	@./$(BIN_DIR)/test

debug:
	CFLAGS += -g -O0
debug: clean test
	@CK_FORK=no; lldb $(BIN_DIR)/test
	@echo "--- builded with debug flags ---"
	
release: CFLAGS += -O3
release: clean all
	@echo "--- Release build ---"


format:
	clang-format -i */*.c */*.h --style=Google

clean:
	rm -rf $(BIN_DIR)/* $(BUILD_DIR)/*
	@echo "--- clean up complete ---"