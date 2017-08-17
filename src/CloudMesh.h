#pragma once 

#include "Cloud.h"

class CloudMesh {
public:
	CloudMesh() {

		m_shader = new Shader("./cloud.vs", "./cloud.fs");
		m_camera = RM::getInstance().camera;
		m_tex = loadTexture("../resources/textures/cloud.tga");

		m_cloudNum = 200;
		for (int i = 0; i < m_cloudNum; ++i) {
			m_cloudMesh.push_back(new Cloud(m_shader, m_tex, m_points, m_pointNum));
		}

		glm::vec3 * points = new glm::vec3[m_points.size()];
		if (!m_points.empty()) {
			memcpy(points, &m_points[0], m_points.size()*sizeof(glm::vec3)); 
		}

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_points.size(), points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	    glBindVertexArray(0);

		m_shader->Use();
		glActiveTexture(GL_TEXTURE6);
		glUniform1i(glGetUniformLocation(m_shader->Program, "cloudTex"), 6);
		glBindTexture(GL_TEXTURE_2D, m_tex);

		// for (int i = 0; i < m_cloudNum; ++i) {

		// }
	}

	void render() const {

		glDisable(GL_DEPTH_TEST);

		m_shader->Use();
		
	    glm::mat4 view = m_camera->GetViewMatrix();
	    GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	    glUniform3f(glGetUniformLocation(m_shader->Program, "cameraPos"), m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

		glBindTexture(GL_TEXTURE_2D, m_tex);

		for (int i = 0; i < m_cloudNum; ++i) {
			glBindVertexArray(m_cloudMesh[i]->m_vao);
			// glDrawArraysInstanced(GL_POINTS);
			m_cloudMesh[i]->render();
			glBindVertexArray(0);
		}

		glEnable(GL_DEPTH_TEST);
	}

	void advance() {
		render();
	}
private:

	Shader * m_shader;
	Camera * m_camera;
	GLuint m_tex;

	GLuint m_vao, m_vbo;

	std::vector<Cloud*> m_cloudMesh;
	int m_cloudNum;
	std::vector<glm::vec3> m_points;
	int m_pointNum;
};