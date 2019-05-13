.PHONY: all clean install uninstall

MKDIR = mkdir -p
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

PREFIX ?= /usr/local
INCLUDE_PATH ?= include/Polar
LIBRARY_PATH ?= lib

INSTALL_INCLUDE_PATH = $(DESTDIR)$(PREFIX)/$(INCLUDE_PATH)
INSTALL_LIBRARY_PATH = $(DESTDIR)$(PREFIX)/$(LIBRARY_PATH)

INSTALL ?= cp -a


DIR_OBJS = objs
DIR_TARGETS = target
DIR_DEPS = deps
DIRS = $(DIR_OBJS) $(DIR_DEPS) $(DIR_TARGETS)

TARGET = libPolar_dfsan.a
TARGET := $(addprefix $(DIR_TARGETS)/, $(TARGET))
INSTALL_LIB = Polar_label.h

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))
DEPS = $(SRCS:.c=.dep)
DEPS := $(addprefix $(DIR_DEPS)/, $(DEPS))

  
all : clean $(TARGET)
	@$(MAKE) -C test_without_intsall

# ifneq ($(MAKECMDGOALS), clean)
# -include $(DEPS)
# endif

$(DIRS):
	$(MKDIR) $@

#多源文件/目标生成静态库
$(TARGET): $(DIR_TARGETS) $(OBJS) 
	$(AR) -rc $(TARGET) $(OBJS)   


$(DIR_OBJS)/%.o: $(DIR_OBJS) %.c 
	$(CC) $(CFLAGS) $(RULE) -o $@ -c $(filter %.c, $^)

$(DIR_DEPS)/%.dep: $(DIR_DEPS) %.c
	@echo "Making $@ ..."
	@set -e; \
	$(RM) $(RMFLAGS) $@.tmp ; \
	$(CC) -E -MM $(filter %.c, $^) > $@.tmp ; \
	sed 's,\(.*\)\.o[ :]*,objs/\1.o $@: ,g' < $@.tmp > $@ ; \
	$(RM) $(RMFLAGS) $@.tmp

install:
	mkdir -p $(INSTALL_LIBRARY_PATH) $(INSTALL_INCLUDE_PATH)
	$(INSTALL) $(INSTALL_LIB) $(INSTALL_INCLUDE_PATH)
	$(INSTALL) $(TARGET) $(INSTALL_LIBRARY_PATH)
	@$(MAKE) -C test_with_intsall

uninstall: 
	$(RM) $(RMFLAGS) $(INSTALL_INCLUDE_PATH)
	$(RM) $(RMFLAGS) $(INSTALL_LIBRARY_PATH)/$(TARGET)

clean:
	$(RM) $(RMFLAGS) $(DIRS) $(TARGET)
	@$(MAKE) -C test_with_intsall clean
	@$(MAKE) -C test_without_intsall clean
