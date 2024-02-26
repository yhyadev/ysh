OUT := out
SRC := src

CFLAGS = -Werror -Wall -Wextra -O2
LDFLAGS = -lreadline

all: $(OUT) $(OUT)/ysh

$(OUT):
	mkdir $@

$(OUT)/ysh: $(wildcard $(SRC)/*.c)
	$(CC) $(CFLAGS) $(LDFLAGS) $? -o $@

clean: $(OUT)
	rm -rf $<
