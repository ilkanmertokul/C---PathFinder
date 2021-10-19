TARGETS = myFind

CC=gcc

CFLAGS = -Wall -Werror -g 

all: clean $(TARGETS)

$(TARGETS):
	$(CC) $(CFLAGS) main.c file-type.c -o $@

clean:
	rm -f $(TARGETS)