CC = gcc
OUTPUT = jovarkos-software-recommendations
all: 
	$(CC) -o $(OUTPUT) main.c `pkg-config --cflags --libs gtk4`
clean: 
	rm $(OUTPUT) 
run:
	./$(OUTPUT)

