#pragma once

#include <deque>
#include <cmath>
#include <sstream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
// #include "Particle.h"
#include "Camera.h"
#include "Shader.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Particle {
	glm::vec3 pos, vel, col;
	float lifetime;
	Particle() {

		glm::vec3 a(1.0f, 0.0f, 0.0f);

		lifetime = drand48() * -6.4f;
		pos = glm::vec3(0.0f, drand48() * 5.0f, 0.0f);
		vel = glm::vec3(0.0f, 0.0f, 0.0f);
		col = glm::vec3(1.0f, 1.0f, 1.0f);
	}
};

class ParticleSystem {
public:
	ParticleSystem(GLFWwindow * window) {
		m_window = window;
		m_particle_shader = new Shader("./particle.vs", "./particle.frag");
		m_advect_shader = new Shader("./advect.vs", 0);

		m_system_stop = false;
		m_total_time = 0;
		m_max_p_num = 50000;

		m_camera = new Camera(glm::vec3(0.0, 5.0, 15.0));

		m_particles = new Particle[m_max_p_num];

		glGenVertexArrays(1, &m_particleVAO);

		glGenTransformFeedbacks(2, m_transformFeedback);
		glGenBuffers(2, m_particleBuffer);

		glBindVertexArray(m_particleVAO);

		for (int i = 0; i < 2; ++i) {
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
			glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_max_p_num, m_particles, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
		}

		// glBindVertexArray(0);

		m_isFirst = true;
		m_currVB = 0;
		m_currTFB = 1;

		m_x = m_z = 0;

		// std::stringstream pngIndex;

		// for (size_t i = 0; i < 8; ++i) {
		// 	for (size_t j = 0; j < 8; ++j) {
		// 		pngIndex << "../src/textures/ParticleAtlas/" << i << "-" << j << ".png";
		// 		m_p_tex[i * 8 + j] = loadTexture(pngIndex.str().c_str());
		// 		pngIndex.str("");
		// 	}
		// }

		m_tex = loadTexture("../src/textures/Particle-Sprite-Smoke-1.png");

		m_particle_shader->Use();
		glActiveTexture(GL_TEXTURE0);
	    // glBindTexture(GL_TEXTURE_2D, m_p_tex[0]);
	    glBindTexture(GL_TEXTURE_2D, m_tex);
	}

	void update(float time_step) {
		m_total_time += time_step;

		glEnable(GL_RASTERIZER_DISCARD);

		m_advect_shader->Use();
		GLint advectShaderLoc = m_advect_shader->Program;
		glUniform1f(glGetUniformLocation(advectShaderLoc, "dt"), time_step);
		glUniform3f(glGetUniformLocation(advectShaderLoc, "iniPos"), m_x, 0.0f, m_z);

		glBindVertexArray(m_particleVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
		
		glBeginTransformFeedback(GL_POINTS);

		if (m_isFirst) {
			glDrawArrays(GL_POINTS, 0, m_max_p_num);
			m_isFirst = false;
		} else {
			glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
		}

		glEndTransformFeedback();
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
		
		glDisable(GL_RASTERIZER_DISCARD);
	}

	void render() const {
		glClearColor(0.08f, 0.08f, 0.16f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    m_particle_shader->Use();

	    int w, h;
	    glfwGetFramebufferSize(m_window, &w, &h);

	    glm::mat4 view;
	    view = m_camera->GetViewMatrix();
	    glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	    GLint viewLoc  = glGetUniformLocation(m_particle_shader->Program, "view");
	    GLint projLoc  = glGetUniformLocation(m_particle_shader->Program, "projection");
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	    glUniform3f(glGetUniformLocation(m_particle_shader->Program, "cameraPos"), m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

	    glBindVertexArray(m_particleVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
		glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

		glfwSwapBuffers(m_window);
	}

	void advance(float time_step) {
		if (!m_system_stop) update(time_step);
		render();

		m_currVB = m_currTFB;
		m_currTFB = (m_currTFB + 1) & 0x1;
	}

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

	Camera * m_camera;

	bool m_system_stop;
	float m_x, m_z;

private:

	GLFWwindow * m_window;

	Shader * m_particle_shader;
	Shader * m_advect_shader;

	GLuint m_tex;

	Particle * m_particles;

	GLuint m_particleVAO;
	GLuint m_particleBuffer[2];
	GLuint m_transformFeedback[2];
	GLuint m_currVB, m_currTFB;

	bool m_isFirst;

	float m_total_time;
	size_t m_max_p_num;
	
};

