CC = gcc
OUTPUT = jovarkos-software-recommendations
all: build

build:
	$(CC) -o $(OUTPUT) build_ui.c main.c  `pkg-config --cflags --libs gtk+-3.0`
clean: 
	rm $(OUTPUT) 
run: build
	./$(OUTPUT)

