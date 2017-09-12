all :
	g++ -std=c++11 -I include *.cpp common/*.cpp -o main.exec -lGL -lGLU -lGLEW -L/usr/include/GL -lglfw -L/usr/include/GLFW -lX11 -lXxf86vm -lXrandr -lpthread -lXi

clean:
	@rm -rf $(TARGET)

re: clean all
