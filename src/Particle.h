#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtc/random.hpp>

class Particle {
public:
	Particle(glm::vec3 _pos, glm::vec3 _v = glm::vec3(0, 0, 0), float _m = 1) {
		pos = _pos;
		v = _v;
		a = glm::vec3(0, -9.8, 0);	// gravity acceleration
		m = _m;
		d = 0;	// needs change
		p = 0;	// needs change
		col = glm::vec3(1, 1, 1);	// white
		f = a * m;	// gravity
	}

	void advance(float dt) {	// less accurate
		pos += v * dt + 0.5f * a * dt * dt;
		v += a * dt;
	}

	glm::vec3 getV() const {return v;}
	glm::vec3 getPos() const {return pos;}
private:

	// physical data
	glm::vec3 pos, v, a;	// position, velocity, acceleration
	float m, d, p;	// mass, density, pressure
	glm::vec3 col;	// color
	glm::vec3 f;	// force

	// structural data
	// int grid_index;

};