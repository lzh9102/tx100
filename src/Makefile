
DATA_PATH=../

CXX=g++
CXXFLAGS=-O3 -Wall
LIBS=-lsfml-system -lsfml-graphics -lsfml-window
OBJ=bullet.o game.o gameserver.o mainmenu.o menu.o \
	player.o resource.o scene.o tx100.o
TARGET=tx100
DEFS=-DDATA_PATH=\"$(DATA_PATH)\"

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(LIBS) $(OBJ)

%.o: %.cpp *.h
	$(CXX) -c $(CXXFLAGS) $(DEFS) $< 

clean:
	rm -f *.o
	rm -f tx100
