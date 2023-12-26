all:
	gcc -std=c17 -I src/include -L src/lib -o snek src/*.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
