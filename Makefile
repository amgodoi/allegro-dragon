# Makefile to create a small project
TARGET ?= dragon
SRC_DIRS ?= ./

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

CC := gcc 
LDLIBS := -lallegro -lallegro_font -lallegro_dialog -lallegro_image -lallegro_ttf
#CFLAGS ?= -std=c99 -g -O0 -Wall -Werror -Wextra -Wpedantic -Wconversion
CFLAGS ?= -std=c99 -g -O0 -Wall -Werror -Wextra -Wpedantic

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

.PHONY: all clean
.PRECIOUS: %.o

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
