// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include "glfw3.h"
GLFWwindow* window;

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Bomberman", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to init glfw\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
	// Load the texture
	GLuint WallTexture = loadDDS("wall.DDS");
	GLuint FloorTexture = loadDDS("floor.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> WallVertices;
	std::vector<glm::vec2> WallUvs;
	std::vector<glm::vec3> WallNormals;
	bool res = loadOBJ("cube.obj", WallVertices, WallUvs, WallNormals);

	// Read our .obj file
	std::vector<glm::vec3> FloorVertices;
	std::vector<glm::vec2> FloorUvs;
	std::vector<glm::vec3> FloorNormals;
	bool res2 = loadOBJ("cube.obj", FloorVertices, FloorUvs, FloorNormals);
	
	std::vector<unsigned short> wall_indices;
	std::vector<glm::vec3> wall_indexed_vertices;
	std::vector<glm::vec2> wall_indexed_uvs;
	std::vector<glm::vec3> wall_indexed_normals;
	indexVBO(WallVertices, WallUvs, WallNormals, wall_indices, wall_indexed_vertices, wall_indexed_uvs, wall_indexed_normals);
	
	std::vector<unsigned short> floor_indices;
	std::vector<glm::vec3> floor_indexed_vertices;
	std::vector<glm::vec2> floor_indexed_uvs;
	std::vector<glm::vec3> floor_indexed_normals;
	indexVBO(FloorVertices, FloorUvs, FloorNormals, floor_indices, floor_indexed_vertices, floor_indexed_uvs, floor_indexed_normals);
	
	// Load it into a VBO
	GLuint WallVertexbuffer;
	glGenBuffers(1, &WallVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, WallVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, wall_indexed_vertices.size() * sizeof(glm::vec3), &wall_indexed_vertices[0], GL_STATIC_DRAW);

	GLuint Walluvbuffer;
	glGenBuffers(1, &Walluvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Walluvbuffer);
	glBufferData(GL_ARRAY_BUFFER, wall_indexed_uvs.size() * sizeof(glm::vec2), &wall_indexed_uvs[0], GL_STATIC_DRAW);

	GLuint WallNormalbuffer;
	glGenBuffers(1, &WallNormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, WallNormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, wall_indexed_normals.size() * sizeof(glm::vec3), &wall_indexed_normals[0], GL_STATIC_DRAW);
	
	GLuint FloorVertexbuffer;
	glGenBuffers(1, &FloorVertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, FloorVertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, floor_indexed_vertices.size() * sizeof(glm::vec3), &floor_indexed_vertices[0], GL_STATIC_DRAW);

	GLuint Flooruvbuffer;
	glGenBuffers(1, &Flooruvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Flooruvbuffer);
	glBufferData(GL_ARRAY_BUFFER, floor_indexed_uvs.size() * sizeof(glm::vec2), &floor_indexed_uvs[0], GL_STATIC_DRAW);

	GLuint FloorNormalbuffer;
	glGenBuffers(1, &FloorNormalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, FloorNormalbuffer);
	glBufferData(GL_ARRAY_BUFFER, floor_indexed_normals.size() * sizeof(glm::vec3), &floor_indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint WallElementbuffer;
	glGenBuffers(1, &WallElementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WallElementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, wall_indices.size() * sizeof(unsigned short), &wall_indices[0] , GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint FloorElementbuffer;
	glGenBuffers(1, &FloorElementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FloorElementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices.size() * sizeof(unsigned short), &floor_indices[0] , GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do{

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		glUseProgram(programID);

		glm::vec3 lightPos = glm::vec3(4,4,4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"

		float x = 0.0;
		float z = 0.0;
		float xMax = 20.0;
		float zMax = 20.0;
		while (z <= zMax) {
			while (x <= xMax) {
				std::cout << "x = " << x << " and z = " << z << std::endl;
				if (((z != 0.0 && z != zMax) && (x != 0.0 && x != xMax)) && ((z != 0.0 && z != zMax) && (x != 0.0 || x != xMax))) {
						glm::mat4 ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, -1.0f, z));
						glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

						// Send our transformation to the currently bound shader, 
						// in the "MVP" uniform
						glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						// 1rst attribute buffer : vertices
						glEnableVertexAttribArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, FloorVertexbuffer);
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

						// 2nd attribute buffer : UVs
						glEnableVertexAttribArray(1);
						glBindBuffer(GL_ARRAY_BUFFER, Flooruvbuffer);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

						// 3rd attribute buffer : normals
						glEnableVertexAttribArray(2);
						glBindBuffer(GL_ARRAY_BUFFER, FloorNormalbuffer);
						glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

						// Index buffer
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FloorElementbuffer);

						// Draw the triangles !
						glDrawElements(GL_TRIANGLES, floor_indices.size(), GL_UNSIGNED_SHORT, (void*)0);
						
				}
				x += 2.0;
			}
			x = 0.0;
			z += 2.0;
		}


//		GLuint WallTexture = loadDDS("wall.DDS");
		x = 0.0;
		z = 0.0;
		xMax = 20.0;
		zMax = 20.0;
		while (z <= zMax) {
			while (x <= xMax) {
				std::cout << "x = " << x << " and z = " << z << std::endl;
				if (((z == 0.0 || z == zMax) && (x != 0.0 || x != xMax)) || ((z != 0.0 || z != zMax) && (x == 0.0 || x == xMax))) {
						glm::mat4 ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, 0.0f, z));
						glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

						// Send our transformation to the currently bound shader, 
						// in the "MVP" uniform
						glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

						// 1rst attribute buffer : vertices
						glEnableVertexAttribArray(0);
						glBindBuffer(GL_ARRAY_BUFFER, FloorVertexbuffer);
						glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

						// 2nd attribute buffer : UVs
						glEnableVertexAttribArray(1);
						glBindBuffer(GL_ARRAY_BUFFER, Flooruvbuffer);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

						// 3rd attribute buffer : normals
						glEnableVertexAttribArray(2);
						glBindBuffer(GL_ARRAY_BUFFER, FloorNormalbuffer);
						glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

						// Index buffer
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FloorElementbuffer);

						// Draw the triangles !
						glDrawElements(GL_TRIANGLES, floor_indices.size(), GL_UNSIGNED_SHORT, (void*)0);
						
				}
				x += 2.0;
			}
			x = 0.0;
			z += 2.0;
		}
		// END OF OBJ RENDER

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0 );
	// Cleanup VBO and shader
	glDeleteBuffers(1, &WallVertexbuffer);
	glDeleteBuffers(1, &Walluvbuffer);
	glDeleteBuffers(1, &WallNormalbuffer);
	glDeleteBuffers(1, &WallElementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &WallTexture);
	glDeleteVertexArrays(1, &VertexArrayID);

	glDeleteBuffers(1, &FloorVertexbuffer);
	glDeleteBuffers(1, &Flooruvbuffer);
	glDeleteBuffers(1, &FloorNormalbuffer);
	glDeleteBuffers(1, &FloorElementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &FloorTexture);
	glDeleteVertexArrays(1, &VertexArrayID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

