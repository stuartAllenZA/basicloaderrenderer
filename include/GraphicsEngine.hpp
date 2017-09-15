#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include "glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"

enum class GameState {PLAY, EXIT};

class GraphicsEngine {
	public:
		GraphicsEngine();
		GraphicsEngine(const char *, int, int);
		~GraphicsEngine();
		void	run();

	private:
		int		init();
		void	gameLoop();
		void	processInput();
		void	drawGame();
		void	drawData();

		GLFWwindow	*window;
		GameState	_gameState;
		int			_width;
		int			_height;
		const char *_windowName;
};
