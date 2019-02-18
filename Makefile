.PHONY: all clean

MKDIR = mkdir
RM = rm
RMFLAGS = -fr


CC      := clang
AR      = ar
RANLIB  = ranlib
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I.
CFLAGS  := -g -Wall -fPIC $(DEFINES) $(INCLUDE)
RULE 	:= -fsanitize-blacklist=./dfsan_abilist.txt


DIR_OBJS = objs
DIR_TARGETS = .
DIR_DEPS = deps
DIRS = $(DIR_OBJS) $(DIR_DEPS)

TARGET = libdfsan.a
TARGET := $(addprefix $(DIR_TARGETS)/, $(TARGET))

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))
DEPS = $(SRCS:.c=.dep)
DEPS := $(addprefix $(DIR_DEPS)/, $(DEPS))

  
all : clean $(TARGET)
	@$(MAKE) -C test

# ifneq ($(MAKECMDGOALS), clean)
# -include $(DEPS)
# endif

$(DIRS):
	$(MKDIR) $@

$(TARGET): $(OBJS) 
	$(AR) cru $(TARGET) $(OBJS)
	$(RANLIB) $(TARGET)

$(DIR_OBJS)/%.o: $(DIR_OBJS) %.c 
	$(CC) $(CFLAGS) $(RULE) -o $@ -c $(filter %.c, $^)

$(DIR_DEPS)/%.dep: $(DIR_DEPS) %.c
	@echo "Making $@ ..."
	@set -e; \
	$(RM) $(RMFLAGS) $@.tmp ; \
	$(CC) -E -MM $(filter %.c, $^) > $@.tmp ; \
	sed 's,\(.*\)\.o[ :]*,objs/\1.o $@: ,g' < $@.tmp > $@ ; \
	$(RM) $(RMFLAGS) $@.tmp

clean:
	$(RM) $(RMFLAGS) $(DIRS) $(TARGET)
	@$(MAKE) -C test clean
