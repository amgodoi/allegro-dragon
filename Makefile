# Makefile to create a small project
TARGET ?= dragon
SRC_DIRS ?= ./

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC := gcc 
LDLIBS := -lallegro -lallegro_font -lallegro_dialog -lallegro_image -lallegro_ttf
CFLAGS ?= $(INC_FLAGS) -g -O0 -Wall

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

.PHONY: all clean
.PRECIOUS: %.o

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
