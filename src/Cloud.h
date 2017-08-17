#pragma once

#include "ResourceManager.h"
const float MINVALUE = -10000.0f;
const float MAXVALUE = 10000.0f;

struct Sphere {
	glm::vec3 c;
	float r;
	Sphere(glm::vec3 _c, float _r) {
		c = _c;
		r = _r;
	}
};

class Cloud {
public:
	Cloud(Shader * shader, GLuint tex, std::vector<glm::vec3> * f_points, int f_pointNum) {

		std::random_device rd;

		srand(rd());
		float xrange = float(rand()%400);
		srand(rd());
		float zrange = float(rand()%400);

		m_camera = RM::getInstance().camera;
		m_shader = shader;
		m_tex = tex;

		m_spheres.push_back(Sphere(glm::vec3(0.0, 50.0, 0.0), 30.0));
		m_spheres.push_back(Sphere(glm::vec3(3.0, 50.0, 1.0), 20.0));
		m_spheres.push_back(Sphere(glm::vec3(-2.0, 50.0, -2.0), 30.0));

		m_pos = glm::vec3(-200.0f + xrange, 40.0f, -200.0f + zrange);

		printf("%f, %f, %f\n", m_pos.x, m_pos.y, m_pos.z);

		float weight_sum = 0;
		float minX = MAXVALUE, minY = MAXVALUE, minZ = MAXVALUE, maxX = MINVALUE, maxY = MINVALUE, maxZ = MINVALUE;
		for (size_t i = 0; i < m_spheres.size(); ++i) {
			Sphere currS = m_spheres[i];
			float currMinX = currS.c.x - currS.r;
			float currMinY = currS.c.y - currS.r;
			float currMinZ = currS.c.z - currS.r;
			float currMaxx = currMinX + 2 * currS.r;
			float currMaxY = currMinY + 2 * currS.r;
			float currMaxZ = currMinZ + 2 * currS.r;

			if (currMinX < minX) minX = currMinX;
			if (currMinY < minY) minY = currMinY;
			if (currMinZ < minZ) minZ = currMinZ;
			if (currMaxx > maxX) maxX = currMaxx;
			if (currMaxY > maxY) maxY = currMaxY;
			if (currMaxZ > maxZ) maxZ = currMaxZ;

			weight_sum += pow(currS.r, 3);
		}

		m_center = glm::vec3(0.0, 0.0, 0.0);
		for (size_t i = 0; i < m_spheres.size(); ++i) {
			Sphere currS = m_spheres[i];
			m_center += currS.c * currS.r * currS.r * currS.r;
		}
		m_center /= weight_sum;

		for (int i = 0; i < f_pointNum; ++i) {
			glm::vec3 currPoint(minX+drand48()*(maxX-minX), minY+drand48()*(maxY-minY), minZ+drand48()*(maxZ-minZ));
			if (pointInCloud(currPoint)) f_points->push_back(currPoint);
		}

		// glm::vec3 * points = new glm::vec3[m_points.size()];
		// if (!m_points.empty()) {
		// 	memcpy(points, &m_points[0], m_points.size()*sizeof(glm::vec3)); 
		// }

		// glGenVertexArrays(1, &m_vao);
		// glGenBuffers(1, &m_vbo);
		// glBindVertexArray(m_vao);
		// glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_points.size(), points, GL_STATIC_DRAW);
		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	 //    glBindVertexArray(0);

	    m_shader->Use();

	    int w, h;
	    glfwGetFramebufferSize(RM::getInstance().window, &w, &h);
	    glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
	    
	    GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
	    
	    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	    
	    glUniform3f(glGetUniformLocation(m_shader->Program, "centerPos"), m_center.x, m_center.y, m_center.z);

	    glActiveTexture(GL_TEXTURE6);
		glUniform1i(glGetUniformLocation(m_shader->Program, "cloudTex"), 6);
		glBindTexture(GL_TEXTURE_2D, m_tex);
	}

	bool pointInCloud(glm::vec3 pPos) {
		for (size_t i = 0; i < m_spheres.size(); ++i) {
			float dist = glm::length(pPos - m_spheres[i].c);
			if (dist < m_spheres[i].r) return true;
		}
		return false;
	}

	void render() const {

		glm::mat4 model = glm::translate(glm::mat4(1.0), m_pos);
	    model = glm::scale(model, glm::vec3(3.0f, 1.0f, 2.0f));
	    GLint modeLoc  = glGetUniformLocation(m_shader->Program, "model");
	    glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));

		// glDrawArrays(GL_POINTS, 0, m_points.size());
	}

	void advance() {
		render();
	}

	GLuint m_vao, m_vbo;

private:

	Camera * m_camera;
	Shader * m_shader;
	GLuint m_tex;

	std::vector<Sphere> m_spheres;
	
	glm::vec3 m_center;
	glm::vec3 m_pos;
	// BoundingBox m_box;
};