#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"
#include <glm/glm.hpp>
#include <iostream>


using namespace std;
using namespace glm;

class GLContext {
private:
	int oldWidth = 1000;
	int oldHeight = 1000;
	int oldX = 500;
	int oldY = 500;
	bool fullScreenUpdate = false;
	GLFWwindow* window;
	void renderLoop();
public:
	const char* window_name = "  ";
	bool alpha = false;  
	bool fullscreen = false;
	int SCR_WIDTH = 1000;
	int SCR_HEIGHT = 1000;
	vec4 background = vec4(0, 0, 0, 0);
	int init(int width, int height);
	void TakeScreenshot(string);
	void updateFullScreen();
	void(*onInput)(GLContext& context, int key) {};
	void(*onDraw)(GLContext& context) {};
	
};

