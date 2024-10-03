test_file := test/main.c
output_file := test/main.exe
delta := delta/

CC := gcc
DEBUG := gdb
CVERSION := -std=c99
CFLAGS := -g -v -Wall $(CVERSION)
CWD := $(shell pwd) 

GLADPATH := test/glad
GLADINCLUDE := $(GLADPATH)/include
GLADSOURCE := $(GLADPATH)/src/glad.c
GLADOUTPUT := test/glad.o

INCLUDE := -I$(delta) -I$(GLADINCLUDE)

LINK := -lopengl32 -lgdi32

test: 
	$(CC) $(CFLAGS) $(INCLUDE) $(GLADSOURCE) $(test_file) -o $(output_file) $(LINK)
run: test
	$(DEBUG) -q ./$(output_file)
clean:
	rm -f $(output_file)
.PHONY: test run