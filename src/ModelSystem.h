#pragma once

#include "ResourceManager.h"

class ModelSystem {
public:
	ModelSystem(GLFWwindow * window) {
		m_window = window;
		m_camera = RM::getInstance().camera;
		m_shader = new Shader("./model.vs", "./model.fs");
		m_ctrl = RM::getInstance().ctrl;
		m_model = new Model("../resources/objects/space ship .obj");
	}

	void update() {

	}

	void render() const {

		m_shader->Use();

        int w, h;
        glfwGetFramebufferSize(m_window, &w, &h);
        glm::mat4 view;
        view = m_camera->GetViewMatrix();
        glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
        GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
        GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // glm::mat4 model = glm::rotate(glm::mat4(1.0), m_ctrl->a, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(m_ctrl->x, 0, m_ctrl->z));
        model = glm::rotate(model, m_ctrl->a, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(m_shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        GLint viewPosLoc = glGetUniformLocation(m_shader->Program, "viewPos");
        glUniform3f(viewPosLoc, m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

        glActiveTexture(GL_TEXTURE3);
        glUniform1i(glGetUniformLocation(m_shader->Program, "skybox"), 3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *(RM::getInstance().skyboxTexture));
        m_model->Draw(*m_shader);
	}

	void advance() {
		if (!m_ctrl->system_stop) update();
		render();
	}

private:
	GLFWwindow * m_window;
	Camera * m_camera;
	Shader * m_shader;
	Ctrl * m_ctrl;

	Model * m_model;
};