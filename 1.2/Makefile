SRC=$(wildcard *.c)
BIN=$(SRC:%.c=%)

all:$(BIN)

$(BIN):%:%.c
	$(CC) -o $@ $^

clean:
	$(RM) .*.swp

distclean:clean
	$(RM) $(BIN)

.PHONY:all clean distclean
