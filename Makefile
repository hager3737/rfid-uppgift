PROG = main.exe
SRC = main.c src/safeinput.c src/card_manager.c src/utils.c
CFLAGS = -g -Wall -Werror 
LIBS = 

all: $(PROG)

$(PROG): $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(LIBS)

clean:
	rm -f $(PROG)

.PHONY: all clean

