.PHONY: all clean

RM = rm
RMFLAGS = -fr

CC = clang
CFLAGS = -g -O0 -fsanitize=dataflow -fPIC
RULE = -fsanitize-blacklist=../dfsan_abilist.txt

PREENY=/root/preeny/x86_64-linux-gnu/desock.so

SRCS = $(wildcard *.c)
TARGETS := $(basename $(SRCS))

YELLOW = "\e[33;1m"

all: $(TARGETS)

%: %.c 
	$(CC) $(CFLAGS) $(RULE) -o $@ $^ -L../target -lPolar_dfsan
	@echo "\033[33m Testing $@ ...\033[0m" ; \
	LD_PRELOAD="$(PREENY)" ./$@ ; \
	echo ""

clean:
	$(RM) $(RMFLAGS) $(TARGETS)