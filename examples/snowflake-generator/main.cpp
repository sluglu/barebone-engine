#include <GLContext.h>
#include <random>

//generator parameters
float PI = 3.14159265359f;
int armsN = 5;
int gen = 10;
float radius = 0.7f;
float pointSize = 1.0f;
vec4 pointColor = vec4(1, 1, 1, 1);

//genral fuhnction
bool showInit = true;
bool pause = false;

//randomize bool
bool RPI = true;
bool RarmsN = true;
bool Rgen = false;
bool Rradius = true;
bool RpointSize = false;
bool RpointColor = true;
bool Rbackground = true;

//utillity
string message = " ";

int ClampInt(int value, int min, int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}


float getRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

int getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gene(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gene);
}

void randomize() {
    if (RPI) PI = getRandomFloat(-10.0f, 10.0f);
    if (RarmsN) armsN = getRandomInt(2, 50);
    if (Rgen) gen = getRandomInt(1, 10000);
    if (Rradius) radius = getRandomFloat(0.0f, 1.0f);
    if (RpointSize) pointSize = getRandomFloat(0.001f, 10.0f);
    if (RpointColor) pointColor = vec4(getRandomFloat(0.0f, 1.0f) , getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f));
    if (Rbackground) GLContext::background = vec4(getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f), getRandomFloat(0.0f, 1.0f));
}

std::vector<vec2> distributePointsInCircle(int numPoints, double radius) {
    std::vector<vec2> points;

    // Calculate the angle between each point
    float angle = 2 * PI / numPoints;

    // Distribute the points evenly in the circle
    for (int i = 0; i < numPoints; i++) {
        float theta = i * angle;

        // Calculate the coordinates using polar to Cartesian conversion
        float x = radius * cos(theta);
        float y = radius * sin(theta);

        // Create a point and add it to the vector
        vec2 point;
        point.x = x;
        point.y = y;
        points.push_back(point);
    }

    return points;
}

vec2 getMidpoint(const vec2& p1, const vec2& p2) {
    vec2 midpoint;
    midpoint.x = (p1.x + p2.x) / 2.0;
    midpoint.y = (p1.y + p2.y) / 2.0;
    return midpoint;
}

int getRandomIntExclud(int maxValue, int exclud = -1) {
    int result = exclud;
    while (result == exclud) {
        std::random_device rd;
        std::mt19937 gene(rd());
        std::uniform_int_distribution<> dist(0, maxValue - 1);
        result = dist(gene);
    }
    return result;
}

//core algorithm
void Draw() {
    std::vector<vec2> points = distributePointsInCircle(armsN, radius);
    if (showInit) {
        for (vec2 p : points) {
            GLContext::drawPoint(p);
        }
    }
    if (!pause) {
        int startIndex = getRandomIntExclud(armsN, -1);
        vec2 inPoint = points[startIndex];
        int index = getRandomIntExclud(armsN, startIndex);
        vec2 sidePoint = points[index];

        for (int i = 0; i < gen; i++) {
            vec2 newPoint = getMidpoint(inPoint, sidePoint);
            GLContext::drawPoint(newPoint, pointSize, pointColor);
            inPoint = newPoint;
            index = getRandomIntExclud(armsN, index);
            sidePoint = points[index];
        }
    }
    
}



void Ui() {
    ImGui::Begin("Parameters");

    //PI
    //ImGui::Text("value of PI");
    ImGui::SliderFloat("value of PI", &PI, -10.0f, 10.0f);
    if (ImGui::Button("Reset")) { PI = 3.14159265359f; }

    //ArmsN
    //ImGui::Text("arms number");
    ImGui::InputInt("arms number", &armsN);
    armsN = ClampInt(armsN, 2, 50);

    //gen
    //ImGui::Text("generation between frame");
    ImGui::InputInt("generation/frame", &gen);
    gen = ClampInt(gen, 1, 10000);

    //radius
    //ImGui::Text("radius");
    ImGui::SliderFloat("radius", &radius, 0.0f, 1.0f);

    //pointSize
    //ImGui::Text("point size");
    ImGui::SliderFloat("point size", &pointSize, 0.001f, 10.0f);

    //background
    ImGui::ColorEdit4("background", &GLContext::background.x);

    //point color
    ImGui::ColorEdit4("point color", &pointColor.x);

    //alpha
    ImGui::Checkbox("alpha blending", &GLContext::alpha);

    //showInit
    ImGui::Checkbox("show inits points", &showInit);

    //randomize
    if (ImGui::Button("randomize")) {
        randomize();
    }
    if (ImGui::BeginMenu("randomize parameters")) {
        ImGui::Checkbox("randomize PI", &RPI);
        ImGui::Checkbox("randomize arms number", &RarmsN);
        ImGui::Checkbox("randomize generation/frame", &Rgen);
        ImGui::Checkbox("randomize radius", &Rradius);
        ImGui::Checkbox("randomize point size", &RpointSize);
        ImGui::Checkbox("randomize point color", &RpointColor);
        ImGui::Checkbox("randomize background", &Rbackground);
        ImGui::EndMenu();
    }
    //take screenshot
    if (ImGui::Button("take a screenshot")) {
        
        message = GLContext::TakeScreenshot();
        
    }
    ImGui::Text(message.c_str());

    //pause
    ImGui::Checkbox("pause", &pause);

    ImGui::End();
}

//context init
int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* szCmdLine, int iCmdShow) {
    GLContext::window_name = "snowflake generator";
    GLContext::onDraw = Draw;
    GLContext::onDrawUI = Ui;
    GLContext::init(1500, 1000);
    return 0;
}

//TODO : explication paramettre (mouse temp window)

