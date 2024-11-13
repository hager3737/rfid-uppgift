PROG = main.exe
SRC = main.c safeinput.c safeinput.h
CFLAGS = -g -Wall -Werror 
LIBS = 

all: $(PROG)

$(PROG): $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(LIBS)

clean:
	rm -f $(PROG)

.PHONY: all clean

