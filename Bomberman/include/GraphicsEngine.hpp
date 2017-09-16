#pragma once

class GraphicsEngine {
	private:
		void	loadScene();
		void	init();

		GLFWwindow	*_window;


	public:
		GraphicsEngine();
		~GraphicsEngine();
};
