#pragma once
#include <libs/imgui/imgui.h>
#include "libs/imgui/backends/imgui_impl_glfw.h"
#include "libs/imgui/backends/imgui_impl_opengl3.h"
#include <stdio.h>

#include <glad/glad.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "libs/lodepng/lodepng.h"
#include <glm/glm.hpp>

#include <Windows.h>
#include <commdlg.h>
#include <iostream>
#include <chrono>
#include <random>



using namespace std;
using namespace glm;

namespace GLContext {	
	extern const char* window_name;
	extern bool alpha;  
	extern bool fullscreen;
	extern bool fpsCounter;
	extern int SCR_WIDTH;
	extern int SCR_HEIGHT;

	extern double simTime;
	extern double simTimeStep;

	extern int viewportResW;
	extern int viewportResH;

	extern vec4 background;
	extern void (*onInput)(int key);
	extern void (*onDraw)();
	extern void (*onInit)();
	extern void (*onDrawUI)();

	struct Time {
		int nanoseconds;
		int microseconds;
		int milliseconds;
		int seconds;
		int minutes;
		int hours;
		int days;
		int months;
		int years;

		Time() {

			auto now = std::chrono::system_clock::now().time_since_epoch();
			auto now_high_resolution = std::chrono::high_resolution_clock::now().time_since_epoch();
			
			nanoseconds = now.count();
			microseconds = nanoseconds / 1000;
			milliseconds = microseconds / 1000;
			seconds = milliseconds / 1000;
			minutes = seconds / 60;
			hours = minutes / 60;
			days = hours / 24;
			months = days / 31;
			years = days / 365;
		}
	};

	int randomInt(int min = -1000, int max = 1000);
	double randomDouble(double min = -1, double max = 1);

	int init(int width = 500, int height = 500);
	
	string TakeScreenshot();

	void setWindowName(const char* name);

	Time getAbsoluteTime();

	void restartSimTimer();
	double getSimTime();
	void incrementSimTimer();
	double getSimTimeStep();
	void setSimTimeStep(double step);

	void setWindowSize(int width, int height);
	void setViewportSize(int width, int height);
	pair<int, int> getWindowSize();
	pair<int, int> getViewportSize();
	
	void drawPoint(pair<double,double> position, double size = 5, vec4 color = vec4(1, 1, 1, 1));
	void drawLine(pair<double,double> position1, pair<double,double> position2, double width = 1, vec4 color = vec4(1, 1, 1, 1));
	void drawSquare(pair<double,double> pos, double Size = 1.0f, double Thickness = 2.0f, vec4 Color = vec4(1, 1, 1, 1));
	void drawRect(pair<double,double> superiorLeft, pair<double,double> inferiorRight, double Thickness = 1.5, vec4 Color = vec4(1, 1, 1, 1));
	void drawCircle(pair<double,double> pos, double radius = 1.0f, int numSegments = 5, double Thickness = 2.0f, vec4 Color = vec4(1, 1, 1, 1));
	void alphaClear();
}

