C = main.c
O = main

all:
	gcc $(C) -o $(O) -Wall -O2 -lm -lGL -lglut

run: all
	./$(O)

clean:
	rm $(O)
