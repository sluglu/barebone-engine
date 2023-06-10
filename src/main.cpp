#include <GLContext.h>


float i = 0.0f;

void Draw(GLContext& context) {
	context.drawLine(vec2(0, -1), vec2(0, 1));
	context.drawPoint(vec2(0.5, 0));
	context.drawPoint(vec2(-0.5, 0));
	context.background = vec4(i, 0, 0, 0);
	i += 0.0001f;
}

void Input(GLContext& context, int key) {

}

int main() {
	GLContext context;
	context.window_name = "test";
	context.background = vec4(0, 0, 0, 0);
	context.onDraw = Draw;
	context.onInput = Input;
	context.init(800, 800);
	

}