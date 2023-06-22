#include "GLContext.h"


namespace GLContext {

    int oldWidth = 1000;
    int oldHeight = 1000;
    int oldX = 500;
    int oldY = 500;
    bool fullScreenUpdate = false;
    GLFWwindow* window;
    GLuint fbo = 0;
    GLuint texture = 0;


    const char* window_name = "  ";
    bool alpha = false;
    bool GUI = true;
    bool fullscreen = false;
    int SCR_WIDTH = 1920;
    int SCR_HEIGHT = 1080;
    int viewportResW = 1000;
    int viewportResH = 1000;
    vec4 background = vec4(0, 0, 0, 0);
    void (*onInput)(int key) {};
    void (*onDraw)() {};
    void (*initialize)() {};
    void (*onDrawUI)() {};
    

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        glViewport(0, 0, width, height);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            if (onInput) { onInput(key); }
        }
    }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    void updateFullScreen() {
        if (GLContext::fullScreenUpdate == fullscreen) {
            return;
        }

        if (fullscreen)
        {
            glfwGetWindowPos(GLContext::window, &GLContext::oldX, &GLContext::oldY);
            glfwGetWindowSize(GLContext::window, &GLContext::oldWidth, &GLContext::oldHeight);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwSetWindowMonitor(GLContext::window, monitor, 0, 0, mode->width, mode->height, 0);
        }
        else
        {
            glfwSetWindowMonitor(GLContext::window, nullptr, GLContext::oldX, GLContext::oldY, GLContext::oldWidth, GLContext::oldHeight, 0);
        }
        GLContext::fullScreenUpdate = !GLContext::fullScreenUpdate;
    }

    void renderLoop() {
        while (!glfwWindowShouldClose(GLContext::window)) {
            updateFullScreen();
            glFlush();
            glfwPollEvents();
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            


            ImGui::Begin("main window", nullptr, window_flags);
            ImGui::PopStyleVar(2);
            ImGuiID dockspace_id = ImGui::GetID("main window");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Options"))
                {
                    ImGui::MenuItem("Fullscreen", NULL, &fullscreen);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();

            if (onDraw) { 
                glBindFramebuffer(GL_FRAMEBUFFER, GLContext::fbo);

                if (!alpha) { glClear(GL_COLOR_BUFFER_BIT); glClearColor(background.x, background.y, background.z, background.w); }
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GLContext::texture, 0);

                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if (status != GL_FRAMEBUFFER_COMPLETE) {
                    std::cout << "framebuffer creation failure" << endl;
                }

                glViewport(0, 0, viewportResW, viewportResW);

                onDraw(); 

                ImGui::Begin("Viewport");
                {
                    ImGui::BeginChild("Render");
                    ImVec2 wsize = ImGui::GetWindowSize();
                    ImGui::Image((void*)(intptr_t)GLContext::texture, wsize);
                    ImGui::EndChild();
                }
                ImGui::End();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            if (onDrawUI) { onDrawUI(); }

            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT); 
            glClearColor(background.x, background.y, background.z, background.w);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);

        }
    }

    int init(int width, int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
        const char* glsl_version = "#version 130";
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
        GLContext::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, window_name, NULL, NULL);
        if (GLContext::window == NULL)
        {
            glfwTerminate();
            return -1;
        }
        glfwSetFramebufferSizeCallback(GLContext::window, framebuffer_size_callback);
        glfwSetKeyCallback(GLContext::window, key_callback);
        glfwMakeContextCurrent(GLContext::window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(GLContext::window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << endl;
            return -1;
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //cout << "OpenGL version : " << glGetString(GL_VERSION) << endl;
        if (initialize) { initialize(); }
        if (onDraw) {
            glGenFramebuffers(1, &GLContext::fbo);
            glGenTextures(1, &GLContext::texture);
            glBindTexture(GL_TEXTURE_2D, GLContext::texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportResW, viewportResH, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        renderLoop();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(GLContext::window);
        glfwTerminate();
        return 0;
    }

    bool ShowSaveFileDialog(char* filePath, const char* filter) {
        HWND hwnd = glfwGetWin32Window(window);
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = filter;
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_OVERWRITEPROMPT;

        if (GetSaveFileNameA(&ofn))
        {
            return true;
        }

        return false;
    }

    string TakeScreenshot() {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = NULL;
        char filePath[MAX_PATH] = " ";
        if (ShowSaveFileDialog(filePath, ".png")) {
            std::vector<unsigned char> image(viewportResW * viewportResH * 4);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glReadPixels(0, 0, viewportResW, viewportResH, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            std::vector<unsigned char> PngBuffer;
            unsigned error = lodepng::encode(PngBuffer, image, viewportResW, viewportResH);
            if (!error) { lodepng::save_file(PngBuffer, filePath); return filePath; }
            if (error) { return lodepng_error_text(error); }
        }
        else { return "Not saved"; }
    }

    

    void drawPoint(vec2 position, float size, vec4 color) {
        glColor4f(color.x, color.y, color.z, color.w);
        glPointSize(size);
        glBegin(GL_POINTS);
        glVertex2f(position.x, position.y);
        glEnd();
    }

    void drawLine(vec2 position1, vec2 position2, float width, vec4 color) {
        glColor4f(color.x, color.y, color.z, color.w);
        glLineWidth(width);
        glBegin(GL_LINES);
        glVertex2f(position1.x, position1.y);
        glVertex2f(position2.x, position2.y);
        glEnd();
    }

}