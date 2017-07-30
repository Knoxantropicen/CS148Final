#include <iostream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "FluidSystem.h"

using namespace std;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

float curr_time = 0.0f, last_time = 0.0f, delta_time = 0.0f;
double previous_seconds = 0;
int frame_count = 0;

FluidSystem * fluidSystem;

GLFWwindow* setupWindow() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "SPH Fluids", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    glewInit();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    return window;
}

void setupInputHandlers(GLFWwindow * window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    fluidSystem->m_camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fluidSystem->m_camera->ProcessMouseScroll(yoffset);
}

void handleInput() {
	glfwPollEvents();

	if (!fluidSystem || !fluidSystem->m_camera) {
        return;
    }

    if (keys[GLFW_KEY_W]) fluidSystem->m_camera->ProcessKeyboard(FORWARD, delta_time);
    if (keys[GLFW_KEY_S]) fluidSystem->m_camera->ProcessKeyboard(BACKWARD, delta_time);
    if (keys[GLFW_KEY_A]) fluidSystem->m_camera->ProcessKeyboard(LEFT, delta_time);
    if (keys[GLFW_KEY_D]) fluidSystem->m_camera->ProcessKeyboard(RIGHT, delta_time);
}

void cleanup() {
	glfwTerminate();

	if (fluidSystem) delete fluidSystem;
}

void updateFpsCounter(GLFWwindow* window) {
    double current_seconds;
    double elapsed_seconds;
    current_seconds = glfwGetTime();
    elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        char tmp[128];
        double fps = (double)frame_count / elapsed_seconds;
        sprintf(tmp, "SPH Fluids - fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    ++frame_count;
}

int main() {
	GLFWwindow * window = setupWindow();
	setupInputHandlers(window);

	fluidSystem = new FluidSystem(window);

	last_time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {

		handleInput();
		updateFpsCounter(window);

		curr_time = glfwGetTime();
		delta_time = curr_time - last_time;

		fluidSystem->advance(delta_time);
		fluidSystem->render();

		last_time = curr_time;
	}

	cleanup();
	return 0;
}