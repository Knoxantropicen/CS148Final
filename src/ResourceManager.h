#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "LoadMesh.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Ctrl {
	bool system_stop;
	float x, z;
	float a;
	Ctrl() {
		system_stop = false;
		x = z = 0;
		a = 0;
	}
};

class RM {
private:
	RM() {
		camera = new Camera(glm::vec3(0.0, 5.0, 15.0));
		ctrl = new Ctrl();
	}
	~RM() {}
	RM(RM const& copy);
	RM& operator=(RM const& copy);
public:
	static RM& getInstance() {
		static RM instance;
		return instance;
	}

	Camera * camera;
	Ctrl * ctrl;

	GLuint * skyboxTexture;
};