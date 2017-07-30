#include "FluidSystem.h"

using namespace std;

FluidSystem::FluidSystem(GLFWwindow * window) {

	m_window = window;

	// add particles
	for (int i=0; i<10; ++i) {
		m_particles.push_back(Particle(glm::vec3(0, 0, 0), glm::vec3(sin(i), 1, cos(i))));
	}
	
	// set time step
	m_time_step = 0.1;
}

void FluidSystem::advance() {

	for (size_t i=0; i<m_particles.size(); ++i) {
		m_particles[i].advance(m_time_step);
	}
}

void FluidSystem::render() const{

	glClearColor(0.08f, 0.08f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i=0; i<m_particles.size(); ++i) {
		glm::vec3 v = m_particles[i].getV();
		printf("%f, %f, %f\n", v.x, v.y, v.z);
	}

	glfwSwapBuffers(m_window);
}