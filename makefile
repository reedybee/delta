test_file := test/main.c
output_file := test/main.exe
delta := delta/

CC := gcc
CVERSION := -std=c99
CFLAGS := -g -v -Wall
CWD := $(shell pwd) 

test: 
	$(CC) $(CFLAGS) $(CVERSION) -I$(delta) $(test_file) -o $(output_file)
run: test
	./$(output_file)
clean:
	rm -f $(output_file)
.PHONY: test run