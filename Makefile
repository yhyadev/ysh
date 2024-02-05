OUT := out
SRC := src

CFLAGS = -Werror -Wall -Wextra -O2 -lncurses -g

all: $(OUT) $(OUT)/ysh

$(OUT):
	mkdir $@

$(OUT)/ysh: $(wildcard $(SRC)/*.c)
	$(CC) $(CFLAGS) $? -o $@

clean: $(OUT)
	rm -rf $<
