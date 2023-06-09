#include <GLContext.h>


float i = 0.0f;

void Draw(GLContext& context) {
	context.background = vec4(i, 1, 1, 1);
	i += 0.0001f;
}

void Input(GLContext& context, int key) {

}

int main() {
	GLContext context;
	context.window_name = "test";
	context.background = vec4(0, 1, 1, 1);
	context.onDraw = Draw;
	context.onInput = Input;
	context.init(800, 800);
	

}