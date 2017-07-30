#pragma once

#include <vector>
#include <cmath>

#include <GLFW/glfw3.h>
#include "Particle.h"


class FluidSystem {
public:
	FluidSystem(GLFWwindow * window);
	void advance();
	void render() const;

	float m_time_step;
private:

	GLFWwindow * m_window;
	std::vector<Particle> m_particles;
};