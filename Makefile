CC = g++ -std=c++0x
CFLAGS =
LDFLAGS =

objects = C4board.o Player.o main.o
headers = C4board.h Player.h
executable = run.out

%.o:%.cc $(headers)
	$(CC) -c -o $@ $< $(CFLAGS)

$(executable): $(objects) 
	$(CC) $(LDFLAGS) $(objects) -o $@ $(CFLAGS)

.PHONY: clean test

clean :
	$(RM) $(objects) $(executable)

test: $(executable)
	./$(executable)
