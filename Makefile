TARGET = game

I = -I include

CFLAGS = g++ -std=c++11

LFLAGS = -lGL -lGLU -lGLEW -L/usr/include/GL -lglfw -L/usr/include/GLFW -lX11 -lXxf86vm -lXrandr -lpthread -lXi

SP = ./src/

GP = ./gfxUtils/

CP = ./include/common/

SRC = $(SP)main.cpp\
	  $(SP)GraphicsEngine.cpp

CSRC = $(CP)*.cpp

all : $(TARGET)

$(TARGET):
	@$(CFLAGS) $(I) $(SRC) $(CSRC) -o $(TARGET) $(LFLAGS)

test:
	@$(CFLAGS) $(I) main.cpp stb_image.cpp -o $(TARGET) $(LFLAGS)

clean:
	@rm -rf $(TARGET)

re: clean all
