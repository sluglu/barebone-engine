#include <GLContext.h>
#include <random>

using namespace GLContext;

bool pause = false;
int gridSize = 300;
float mutation = 10.0f;
bool negativeMut = true;
bool randomizeColor = true;
bool randomizePosition = true;

int newGridSize = gridSize;
string message = " ";
float pointSize = 3.5f;

uvec2 getRandomElement(const vector<uvec2>& vec) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, vec.size() - 1);
    return vec[dis(gen)];
}

vec4 getRandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    float red = dis(gen);
    float green = dis(gen);
    float blue = dis(gen);
    float alpha = 1.0f;

    return vec4(red, green, blue, alpha);
}

vec2 getRandomPos() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    return vec2(dis(gen),dis(gen));
}

struct Cell {
    vec4 color = vec4(0, 0, 0, 0);
    bool empty = true;
    uvec2 pos = uvec2(0, 0);
    vector<uvec2> neighbors;
};

struct StartingPoint {
    vec2 pos = vec2(0.5f, 0.5f);
    vec4 color = getRandomColor();
};

std::vector<StartingPoint> startingPoints(1, StartingPoint());
vector<vector<Cell>> grid(gridSize, vector<Cell>(gridSize));
vector<uvec2> livingCell;

// Function to mutate the hue of an RGBA color
vec4 mutateHue(vec4 color) {
    std::random_device rd;
    std::mt19937 gen(rd());
    float min = 0.0f;
    if (negativeMut) { min = -mutation; }
    std::uniform_real_distribution<float> dis(min, mutation);
    float hueMutation = dis(gen);
    // Convert RGB to HSV
    float r = color.x;
    float g = color.y;
    float b = color.z;
    float maxVal = std::max(std::max(r, g), b);
    float minVal = std::min(std::min(r, g), b);
    float delta = maxVal - minVal;

    float h, s, v;
    v = maxVal;

    if (maxVal != 0.0f) {
        s = delta / maxVal;
        if (r == maxVal) {
            h = (g - b) / delta;
        }
        else if (g == maxVal) {
            h = 2.0f + (b - r) / delta;
        }
        else {
            h = 4.0f + (r - g) / delta;
        }

        h *= 60.0f;
        if (h < 0.0f) {
            h += 360.0f;
        }
    }
    else {
        h = 0.0f;
        s = 0.0f;
    }

    // Mutate the hue
    h += hueMutation;
    if (h > 360.0f) {
        h -= 360.0f;
    }

    // Convert HSV back to RGB
    float c = v * s;
    float x = c * (1 - std::abs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    if (h >= 0.0f && h < 60.0f) {
        r = c + m;
        g = x + m;
        b = m;
    }
    else if (h >= 60.0f && h < 120.0f) {
        r = x + m;
        g = c + m;
        b = m;
    }
    else if (h >= 120.0f && h < 180.0f) {
        r = m;
        g = c + m;
        b = x + m;
    }
    else if (h >= 180.0f && h < 240.0f) {
        r = m;
        g = x + m;
        b = c + m;
    }
    else if (h >= 240.0f && h < 300.0f) {
        r = x + m;
        g = m;
        b = c + m;
    }
    else if (h >= 300.0f && h <= 360.0f) {
        r = c + m;
        g = m;
        b = x + m;
    }
    return vec4(r, g, b, color.w);
}

void populateNeighbors(Cell& cell) {
    int x = cell.pos.x;
    int y = cell.pos.y;
    if  (x - 1 > 0        && y + 1 < gridSize)    cell.neighbors.push_back(uvec2(x - 1, y + 1));
    if  (                    y + 1 < gridSize)    cell.neighbors.push_back(uvec2(x    , y + 1));
    if  (x + 1 < gridSize && y + 1 < gridSize)    cell.neighbors.push_back(uvec2(x + 1, y + 1));
    if  (x + 1 < gridSize                    )    cell.neighbors.push_back(uvec2(x + 1, y    ));
    if  (x + 1 < gridSize && y - 1 > 0       )    cell.neighbors.push_back(uvec2(x + 1, y - 1));
    if  (                    y - 1 > 0       )    cell.neighbors.push_back(uvec2(x    , y - 1));
    if  (x - 1 > 0        && y - 1 > 0       )    cell.neighbors.push_back(uvec2(x - 1, y - 1));
    if  (x - 1 > 0                           )    cell.neighbors.push_back(uvec2(x - 1, y    ));
}


void populateGrid() {
    livingCell = vector<uvec2>();
    grid = vector<vector<Cell>>(gridSize, vector<Cell>(gridSize));
    pointSize = ceil((float)GLContext::SCR_WIDTH / (float)gridSize);
    for (int i = 0; i < gridSize; i++) {
        for (int u = 0; u < gridSize; u++) {
            grid[i][u].pos = uvec2(i, u);
            grid[i][u].empty = true;
            grid[i][u].color = vec4(0, 0, 0, 0);
            populateNeighbors(grid[i][u]);
        }
    }
    for (StartingPoint p : startingPoints) {
        ivec2 r = (p.pos * (float)gridSize);
        if (r.x == gridSize) { r.x -= 1; }
        if (r.y == gridSize) { r.y -= 1; }
        grid[r.x][r.y].empty = false;
        grid[r.x][r.y].color = p.color;
        livingCell.push_back(grid[r.x][r.y].pos);
    }
}

vector<uvec2> getEmptyNeighbors(const Cell& cell) {
    vector<uvec2> emptyNeighbors;
    for (const uvec2& c : cell.neighbors) {
        if (grid[c.x][c.y].empty) {
            emptyNeighbors.push_back(c);
        }
    }
    return emptyNeighbors;
}

void drawCell(const Cell& cell) {
    float x = ((float)cell.pos.x / gridSize) * 2.0f - 1.0f + (1.0f / (float)gridSize);
    float y = ((float)cell.pos.y / gridSize) * 2.0f - 1.0f + (1.0f / (float)gridSize);
    drawPoint(vec2(x, y), pointSize, cell.color);
}

void propagation() {
    vector<uvec2> newCell;
    vector<uvec2> oldCell;
    for(uvec2& c : livingCell){
        if (!grid[c.x][c.y].empty) {
            vector<uvec2>emptyNeighbors = getEmptyNeighbors(grid[c.x][c.y]);
            if (emptyNeighbors.size() > 0) {
                uvec2 pos = getRandomElement(emptyNeighbors);
                grid[pos.x][pos.y].empty = false;
                grid[pos.x][pos.y].color = mutateHue(grid[c.x][c.y].color);
                newCell.push_back(grid[pos.x][pos.y].pos);
            }
            else {
                oldCell.push_back(c);
            }
        }
    }
    for (uvec2 c : oldCell) {
        livingCell.erase(std::remove(livingCell.begin(), livingCell.end(), c), livingCell.end());
    }
    for (uvec2 c : newCell) {
        livingCell.push_back(c);
    }
}

void randomize() {
    for (StartingPoint& p : startingPoints) {
        if (randomizeColor) { p.color = getRandomColor(); }
        if (randomizePosition) { p.pos = getRandomPos(); }
    }
}

void drawGrid() {
    for (int i = 0; i < gridSize; i++) {
        for (int u = 0; u < gridSize; u++) {
            drawCell(grid[i][u]);
        }
    }
}

void init() {
    populateGrid();
}

void draw() {
    if (!pause) {
        propagation();
    }
    drawGrid();
}

void drawStartingPointsList() {
    for (int i = 0; i < startingPoints.size(); i++)
    {
        ImGui::PushID(i);
        if (ImGui::CollapsingHeader(("point " + to_string(i)).c_str())) {
            ImGui::SliderFloat2("position", &startingPoints[i].pos.x, 0.0f, 1.0f);
            ImGui::ColorEdit4("color", &startingPoints[i].color.x);
        }
        ImGui::PopID();
    }
    if (ImGui::Button("Remove")) { startingPoints.pop_back(); }
    if (ImGui::Button("Add point")) { startingPoints.push_back(StartingPoint()); }
}

void ui() {
    ImGui::Begin("parameters");

    ImGui::SeparatorText("starting points");
    drawStartingPointsList();
    ImGui::Spacing();

    ImGui::SeparatorText("Generation");
    ImGui::SliderFloat("mutation", &mutation, 0.0f, 50.0f);
    ImGui::Checkbox("negative mutation", &negativeMut);
    ImGui::Spacing();
    
    ImGui::SeparatorText("Randomize");
    ImGui::Checkbox("Randomize position", &randomizePosition);
    ImGui::Checkbox("Randomize color", &randomizeColor);
    if (ImGui::Button("Randomize")) { randomize(); populateGrid(); }
    ImGui::Spacing();

    ImGui::SeparatorText("General");
    ImGui::InputInt("grid size", &newGridSize);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        if (newGridSize < 1) { newGridSize = 1; }
        else if (newGridSize > 1000) { newGridSize = 1000; }
        gridSize = newGridSize;
        populateGrid();
    }
    ImGui::Checkbox("pause", &pause);
    if (ImGui::Button("Next frame") && pause == true) {
        propagation();
    }
    if (ImGui::Button("Reset")) { populateGrid(); }
    if (ImGui::Button("Take screenshot")) { message = TakeScreenshot(); }
    ImGui::Text(message.c_str());
    ImGui::Spacing();

    ImGui::End();
}

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* szCmdLine, int iCmdShow) {
    GLContext::window_name = "Huegene";
    initialize = init;
    onDraw = draw;
    onDrawUI = ui;
    GLContext::init(1200, 1000);
    return 0;
}

//TODO : optimise
//TODO : many starting point with mouse

