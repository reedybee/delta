TESTFILE := test/main.c
OUTPUTFILE := test/main.exe
DELTA := delta/

CC := gcc
DEBUG := gdb
CFLAGS := -g -v -Wall -std=c99 -no-pie
CWD := $(shell pwd) 

GLADPATH := test/glad
GLADINCLUDE := $(GLADPATH)/include
GLADSOURCE := $(GLADPATH)/src/glad.c

INCLUDE := -I$(DELTA) -I$(GLADINCLUDE)
LINK := -lopengl32 -lgdi32 -ldwmapi

test: 
	$(CC) $(CFLAGS) $(INCLUDE) $(GLADSOURCE) $(TESTFILE) -o $(OUTPUTFILE) $(LINK)
debug: test
	$(DEBUG) -q ./$(OUTPUTFILE)
release: test
	./$(OUTPUTFILE)
clean:
	rm -f $(OUTPUTFILE)
.PHONY: test run release