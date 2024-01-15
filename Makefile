all:
	{g++ -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2} ; if ($?) { .\main }

	g++ $fileName -o $fileNameWithoutExt

g++ -I src/include -L src/lib as.cpp -o as -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf