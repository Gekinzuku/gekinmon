CC=g++
CFLAGS=-c `sdl-config --cflags` -I/opt/local/include/ 
LDFLAGS=`sdl-config --libs` -lSDL -lSDL_image -lSDL_ttf -llua -ldl
SOURCES=src/main.cpp src/game.cpp src/battle.cpp src/embed.cpp src/lua-api.cpp src/sprite.cpp src/quest.cpp src/npc.cpp src/object.cpp src/window.cpp src/player.cpp src/area.cpp src/xml/tinystr.cpp src/xml/tinyxml.cpp src/xml/tinyxmlerror.cpp src/xml/tinyxmlparser.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o src/xml/*.o

win32:
	make -f Makefile.win32
code:
	vim -u vim.rc
purge: clean
	rm -f test test.exe src/.*swp src/xml/.*swp Data/.*swp
