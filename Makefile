#good basic template, replace files when needed

all:
	g++  -o Snake client.cpp -lmingw32 -lws2_32

clean:
	if [ -a Snake.exe ]; then rm Snake.exe; fi;
	if [ -a highscore.txt ]; then rm highscore.txt; fi;

	