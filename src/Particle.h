// #pragma once

// #define GLM_FORCE_RADIANS
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include "Shader.h"
// #include "ResourceManager.h"

// class Particle {
// public:
// 	Particle(Shader * _shader, glm::vec3 _pos, glm::vec3 _v = glm::vec3(0, 0, 0), float _m = 1) {

// 		m_shader = _shader;
// 		m_hori = RM::getInstance().p_hori;
// 		m_vert = RM::getInstance().p_vert;

// 		pos = _pos;
// 		v = _v;
// 		a = glm::vec3(0, -0.98, 0);	// gravity acceleration
// 		m = _m;
// 		d = 0;	// needs change
// 		p = 0;	// needs change
// 		col = glm::vec3(1, 1, 1);	// white
// 		f = a * m;	// gravity
// 	}

// 	void advance(float dt) {	// less accurate
// 		pos += v * dt + 0.5f * a * dt * dt;
// 		v += a * dt;

// 		// boundary condition
// 		if (pos.y < -2.0f) {
// 			pos.y = -2.0f + (-2.0f - pos.y);
// 			v.y = 0.4 * -v.y;
// 		}
// 	}

// 	void render() const {

// 		if (m_shader) {
// 			m_shader->Use();

// 			glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), glm::vec3(pos));
// 			translation_matrix = glm::scale(translation_matrix, glm::vec3(0.05, 0.05, 0.05));

//             glUniformMatrix4fv(glGetUniformLocation(m_shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(translation_matrix));

// 			GLint objectColorLoc = glGetUniformLocation(m_shader->Program, "objectColor");
//             glUniform3f(objectColorLoc, col[0], col[1], col[2]);

//             // printf("%f, %f, %f\n", pos.x, pos.y, pos.z);
// 		}

// 		glBindVertexArray(RM::getInstance().p_VAO);
// 		glDrawArrays(GL_TRIANGLES, 0, 6 * 4 * m_hori * m_vert);
// 		glBindVertexArray(0);
// 	}

// 	glm::vec3 getV() const {return v;}
// 	glm::vec3 getPos() const {return pos;}
// private:

// 	Shader * m_shader;
// 	int m_hori, m_vert;	// resolution

// 	// physical data
// 	glm::vec3 pos, v, a;	// position, velocity, acceleration
// 	float m, d, p;	// mass, density, pressure
// 	glm::vec3 col;	// color
// 	glm::vec3 f;	// force

// 	// structural data
// 	// int grid_index;

// };