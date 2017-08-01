#pragma once

#include <deque>
#include <cmath>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
// #include "Particle.h"
#include "Camera.h"
#include "Shader.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Particle {
	glm::vec3 pos, vel, col;
	float lifetime;
	Particle() {
		pos = glm::vec3(-1.0f + drand48()*2.0f, -1.0f + drand48()*2.0f, -1.0f + drand48()*2.0f);
		// double d = drand48() * M_PI / 180;
		// vel = glm::vec3(sin(d) * 2, 1.0f, cos(d) * 2);
		vel = glm::vec3(-1.0f + drand48()*2.0f, 0, -1.0f + drand48()*2.0f);
		col = glm::vec3(drand48(), drand48(), drand48());
		lifetime = 0.0f;
	}
};

class FluidSystem {
public:
	FluidSystem(GLFWwindow *);

	void update(float);

	void render() const;

	void advance(float);

	Camera * m_camera;
private:

	GLFWwindow * m_window;
	Shader * m_particle_shader;
	Shader * m_advect_shader;

	// GLuint m_advectVAO;
	GLuint m_particleVAO;
	GLuint m_particleBuffer[2];

	GLuint m_transformFeedback[2];

	float m_total_time;
	size_t m_max_p_num;

	bool m_isFirst;

	GLuint m_currVB, m_currTFB;

	Particle * m_particles;
};