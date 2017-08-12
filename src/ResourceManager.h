#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <sstream>
#include <vector>
#include <random>

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
	float x, y, z;
	float a;
    float p;
	Ctrl() {
		system_stop = false;
		x = y = z = 0;
		a = 0;
        p = 0;
	}
};

class RM {
private:
	RM() {
		camera = new Camera(glm::vec3(-15.0, 15.0, 30.0));
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

	GLFWwindow * window;
	Camera * camera;
	Ctrl * ctrl;
};

unsigned int loadTexture(char const * path) {
	unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width = 0, height = 0, nrComponents = 0;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}