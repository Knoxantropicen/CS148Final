#pragma once

#include <vector>
#include <cmath>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "Particle.h"
#include "Camera.h"


class FluidSystem {
public:
	FluidSystem(GLFWwindow * window) {
		m_window = window;
		m_shader = new Shader("./shader.vs", "./shader.frag");

		// add particles
		for (int i=0; i<10; ++i) {
			m_particles.push_back(Particle(m_shader, glm::vec3(0, 0, 0), glm::vec3(sin(i), 1, cos(i))));
		}
		
		// set time step
		m_time_step = 0.1;

		m_camera = new Camera(glm::vec3(0.0, 0.0, 10.0));
	}

	void advance() {
		for (size_t i=0; i<m_particles.size(); ++i) {
			m_particles[i].advance(m_time_step);
		}
	}

	void render() const {
		glClearColor(0.08f, 0.08f, 0.16f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // GLint viewPosLoc = glGetUniformLocation(m_shader->Program, "viewPos");
     //    glUniform3f(viewPosLoc, m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

        int w, h;
        glfwGetFramebufferSize(m_window, &w, &h);

        glm::mat4 view;
        view = m_camera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
        GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
        GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (size_t i=0; i<m_particles.size(); ++i) {
			m_particles[i].render();
		}

		glfwSwapBuffers(m_window);
	}

	float m_time_step;

	Camera * m_camera;
private:

	GLFWwindow * m_window;
	Shader * m_shader;

	std::vector<Particle> m_particles;
};