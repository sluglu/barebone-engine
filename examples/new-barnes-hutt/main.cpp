#include <GLContext.h>

//using namespace GLContext;

vec2 pos = vec2 (0,0);
float radius = 1.0f;
int numSegments = 5;
float Thickness = 2.0f;
vec4 Color = vec4(1, 1, 1, 1);

void frame() {
	GLContext::drawCircle(pos,radius,numSegments,Thickness,Color);
}

void ui() {
	ImGui::Begin("Parameters");
		ImGui::InputFloat2("Position", &pos[0]);
		ImGui::InputInt("numSegments", &numSegments);
		ImGui::InputFloat("radius", &radius);
		ImGui::InputFloat("Thickness", &Thickness);
		ImGui::ColorEdit4("Color", &Color[0]);
	ImGui::End();
}

void main() {
	GLContext::onDrawUI = ui;
	GLContext::onDraw = frame;
	GLContext::fpsCounter = true;
	GLContext::init(500, 500);
}