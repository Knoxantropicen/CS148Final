#pragma once

#include "ResourceManager.h"

class Building {
public:

	Building(float l, float h, float w, glm::vec3 pos, float scale, int type) {
		m_shader = new Shader("./building.vs", "./building.fs");
		m_camera = RM::getInstance().camera;
		std::stringstream ss;
		ss << "../resources/textures/building" << type << ".jpg";
		m_tex = loadTexture(ss.str().c_str());

		GLfloat vertices[] = {
	        // pos 		// tex
	        0, h, 0,    0, h,	
	        0, 0, 0,    0, 0,
	        l, 0, 0,    l, 0,
	        l, 0, 0,    l, 0,
	        l, h, 0,    l, h,
	        0, h, 0,    0, h,
	        
	        0, 0, w,    w, 0,
	        0, 0, 0,    0, 0,
	        0, h, 0,    0, h,
	        0, h, 0,    0, h,
	        0, h, w,    w, h,
	        0, 0, w,    w, 0,
	        
	        l, 0, 0,    l, 0,
	        l, 0, w,    0, 0,
	        l, h, w,    0, h,
	        l, h, w,    0, h,
	        l, h, 0,    l, h,
	        l, 0, 0,    l, 0,
	        
	        0, 0, w,    w, 0,
	        0, h, w,    w, h,
	        l, h, w,    0, h,
	        l, h, w,    0, h,
	        l, 0, w,    0, 0,
	        0, 0, w,    w, 0,
	        
	        0, h, 0,    0, 0,
	        l, h, 0,    0, 0,
	        l, h, w,    0, 0,
	        l, h, w,    0, 0,
	        0, h, w,    0, 0,
	        0, h, 0,    0, 0,
	        
	        0, 0, 0,    0, 0,
	        0, 0, w,    0, 0,
	        l, 0, 0,    0, 0,
	        l, 0, 0,    0, 0,
	        0, 0, w,    0, 0,
	        l, 0, w,	0, 0
	    };

	    GLuint vbo;
	    glGenVertexArrays(1, &m_vao);
	    glGenBuffers(1, &vbo);
	    glBindVertexArray(m_vao);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	    glBindVertexArray(0);

	    m_shader->Use();

	    glActiveTexture(GL_TEXTURE6);
		glUniform1i(glGetUniformLocation(m_shader->Program, "tex"), 6);
		glBindTexture(GL_TEXTURE_2D, m_tex);

		int fw, fh;
	    glfwGetFramebufferSize(RM::getInstance().window, &fw, &fh);
	    glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)fw / (GLfloat)fh, 0.1f, 1000.0f);
		GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::translate(glm::mat4(1.0), pos);
	    model = glm::scale(model, glm::vec3(scale, scale, scale));
	    GLint modeLoc  = glGetUniformLocation(m_shader->Program, "model");
	    glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
	}

	void render() const {

		m_shader->Use();

        glm::mat4 view = m_camera->GetViewMatrix();
	    GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        glBindVertexArray(m_vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
	}

	void advance() {
		render();
	}

private:
	Shader * m_shader;
	Camera * m_camera;

	GLuint m_vao;
	GLuint m_tex;

};