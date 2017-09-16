TARGET = game

I = -I include -I code

CFLAGS = g++ -std=c++11

LFLAGS = -lGL -lGLU -lGLEW -L/usr/include/GL -lglfw -L/usr/include/GLFW -lX11 -lXxf86vm -lXrandr -lpthread -lXi 

SRC = ./src/*.cpp

all : $(TARGET)

$(TARGET):
	@$(CFLAGS) $(I) $(SRC) -o $(TARGET) $(LFLAGS)

clean:
	@rm -rf $(TARGET)

re: clean all
