
DATA_PATH=../

CXX=g++
CXXFLAGS=-O3 -Wall
LIBS=-lsfml-system -lsfml-graphics -lsfml-window
TARGET=tx100
DEFS=-DDATA_PATH=\"$(DATA_PATH)\"
OBJ=bullet.o game.o menu.o player.o tx100.o

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(LIBS) $(OBJ)

%.o: %.cpp *.h
	$(CXX) -c $(CXXFLAGS) $(DEFS) $< 

clean:
	rm -f *.o
	rm -f tx100
