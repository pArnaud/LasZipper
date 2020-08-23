SOURCES =	main.cpp
OBJECTS	=	$(SOURCES:.cpp=.o)
TARGET	=	LasZipper
LIBS	=	$(shell pdal-config --libs) \
			/usr/local/Cellar/boost/1.73.0/lib/libboost_filesystem.dylib

all: $(OBJECTS)
	g++ -Wall -Wextra $(LIBS) -o $(TARGET) $(OBJECTS)

%o: %cpp
	g++ -std=c++11 -Wall -Wextra -o $@ -c $<

x: all
	./$(TARGET)

clean:
	rm -rf $(OBJECTS)

superclean : clean
	rm -rf $(TARGET)
