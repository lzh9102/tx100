
PREFIX=/usr
DATA_PATH=$(PREFIX)/share/tx100

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

install: $(TARGET)
	install -d $(DESTDIR)$(DATA_PATH)
	install -m 644 player*.png $(DESTDIR)$(DATA_PATH)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 tx100 $(DESTDIR)$(PREFIX)/bin/

uninstall:
	rm -rf $(DESTDIR)$(DATA_PATH)
	rm -rf $(DESTDIR)$(PREFIX)/bin/tx100
