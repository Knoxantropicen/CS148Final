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

#include "ResourceManager.h"
#include "ParticleSystem.h"
#include "ModelSystem.h"
#include "Skybox.h"

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

ParticleSystem * particleSystem;
ModelSystem * modelSystem;
Skybox * skybox;
Ctrl * ctrl;

GLFWwindow* setupWindow() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    if (window == NULL) {
    	printf("failed\n");
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    glewInit();

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

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
    if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        RM::getInstance().ctrl->system_stop = !RM::getInstance().ctrl->system_stop;
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
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    RM::getInstance().camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    RM::getInstance().camera->ProcessMouseScroll(yoffset);
}

void handleInput() {
	glfwPollEvents();

    if (keys[GLFW_KEY_W]) RM::getInstance().camera->ProcessKeyboard(FORWARD, delta_time);
    if (keys[GLFW_KEY_S]) RM::getInstance().camera->ProcessKeyboard(BACKWARD, delta_time);
    if (keys[GLFW_KEY_A]) RM::getInstance().camera->ProcessKeyboard(LEFT, delta_time);
    if (keys[GLFW_KEY_D]) RM::getInstance().camera->ProcessKeyboard(RIGHT, delta_time);

    float speed = 0.1f, degree = M_PI / 360;
    if (keys[GLFW_KEY_I]) {
        ctrl->x -= speed * sin(ctrl->a);
        ctrl->z -= speed * cos(ctrl->a);
    }
    if (keys[GLFW_KEY_K]) {
        ctrl->x += speed * sin(ctrl->a);
        ctrl->z += speed * cos(ctrl->a);
    }
    if (keys[GLFW_KEY_J]) ctrl->a += degree;
    if (keys[GLFW_KEY_L]) ctrl->a -= degree;
}

void cleanup() {
	glfwTerminate();
	if (particleSystem) delete particleSystem;
    if (modelSystem) delete modelSystem;
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
        sprintf(tmp, "CS148Final - knoxtys - fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    ++frame_count;
}

int main() {
	GLFWwindow * window = setupWindow();
	setupInputHandlers(window);

	particleSystem = new ParticleSystem(window);
    modelSystem = new ModelSystem(window);
    skybox = new Skybox(window);
    ctrl = RM::getInstance().ctrl;

	last_time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {

		handleInput();
		updateFpsCounter(window);

		curr_time = glfwGetTime();
		delta_time = curr_time - last_time;

        glClearColor(0.08f, 0.08f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox->advance();
        particleSystem->advance(delta_time);
        modelSystem->advance();

		last_time = curr_time;

        glfwSwapBuffers(window);
	}

	cleanup();
	return 0;
}