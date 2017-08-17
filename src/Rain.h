#pragma once

#include <deque>
#include <cmath>
#include <sstream>

#include "ResourceManager.h"

struct Raindrop {
	glm::vec3 pos, vel, col;
	Raindrop() {
		float scale = 400.0f;
		pos = glm::vec3(-scale + drand48()*2*scale, 0.5 * (-scale + drand48()*2*scale) + 100.0f, -scale + drand48()*2*scale);
		vel = glm::vec3(0.0f, 0.0f, 0.0f);
		col = glm::vec3(1.0f, 1.0f, 1.0f);
	}
};

class Rain {
public:
	Rain() {
		m_particle_shader = new Shader("./rain.vs", "./rain.fs", "./rain.gs");
		m_advect_shader = new Shader("./fall.vs", 0, 0, 2);

		m_total_time = 0;
		m_max_p_num = 1000000;

		m_camera = RM::getInstance().camera;

		m_particles = new Raindrop[m_max_p_num];

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

		m_isFirst = true;
		m_currVB = 0;
		m_currTFB = 1;

		m_ctrl = RM::getInstance().ctrl;

		m_particle_shader->Use();
		
		int w, h;
	    glfwGetFramebufferSize(RM::getInstance().window, &w, &h);
		glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
		GLint projLoc  = glGetUniformLocation(m_particle_shader->Program, "projection");
	    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void update(float time_step) {
		m_total_time += time_step;

		glEnable(GL_RASTERIZER_DISCARD);

		m_advect_shader->Use();
		GLint advectShaderLoc = m_advect_shader->Program;
		glUniform1f(glGetUniformLocation(advectShaderLoc, "dt"), time_step);
		glUniform3f(glGetUniformLocation(advectShaderLoc, "cameraPos"), m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

		glBindVertexArray(m_particleVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

		glEnableVertexAttribArray(0);		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

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

	    m_particle_shader->Use();

	    glm::mat4 view = m_camera->GetViewMatrix();
	    GLint viewLoc  = glGetUniformLocation(m_particle_shader->Program, "view");
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	    glUniform3f(glGetUniformLocation(m_particle_shader->Program, "cameraPos"), m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

	    glBindVertexArray(m_particleVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);
	}

	void advance(float time_step) {
		if (!m_ctrl->system_stop) update(time_step);
		render();

		m_currVB = m_currTFB;
		m_currTFB = (m_currTFB + 1) & 0x1;
	}

private:

	Shader * m_particle_shader;
	Shader * m_advect_shader;
	Camera * m_camera;

	Raindrop * m_particles;

	GLuint m_particleVAO;
	GLuint m_particleBuffer[2];
	GLuint m_transformFeedback[2];
	GLuint m_currVB, m_currTFB;

	bool m_isFirst;

	float m_total_time;
	size_t m_max_p_num;

	Ctrl * m_ctrl;
};

