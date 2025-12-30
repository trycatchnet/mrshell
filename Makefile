CC = gcc
TARGET = mrshell

BUILD_DIR = build
SRC_DIR = src

SRCS = main.c parser.c utils.c terminal.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) -o $(BUILD_DIR)/$(TARGET) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

re: clean all

.PHONY: all clean re
