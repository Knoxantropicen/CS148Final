#pragma once

#include "ResourceManager.h"

class ModelSystem {
public:
	ModelSystem() {
		m_camera = RM::getInstance().camera;
		m_shader = new Shader("./model.vs", "./model.fs");
		m_ctrl = RM::getInstance().ctrl;
		m_model = new Model("../resources/objects/space ship .obj");

		m_shader->Use();

		int w, h;
        glfwGetFramebufferSize(RM::getInstance().window, &w, &h);
        glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
        GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void update() {

	}

	void render() const {

		m_shader->Use();

        glm::mat4 view;
        view = m_camera->GetViewMatrix();
        GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(m_ctrl->x, m_ctrl->y, m_ctrl->z));
        model = glm::rotate(model, float(m_ctrl->p), glm::vec3(-sin(m_ctrl->a + M_PI/2), 0.0f, -cos(m_ctrl->a + M_PI/2)));
        model = glm::rotate(model, m_ctrl->a, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(m_shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        GLint viewPosLoc = glGetUniformLocation(m_shader->Program, "viewPos");
        glUniform3f(viewPosLoc, m_camera->Position.x, m_camera->Position.y, m_camera->Position.z);

        m_model->Draw(*m_shader);
	}

	void advance() {
		if (!m_ctrl->system_stop) update();
		render();
	}

private:
	Camera * m_camera;
	Shader * m_shader;
	Ctrl * m_ctrl;

	Model * m_model;
};