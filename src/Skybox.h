#pragma once

#include "ResourceManager.h"

class Skybox {
public:
	Skybox(GLFWwindow * window) {

		m_window = window;
		m_shader = new Shader("./skybox.vs", "./skybox.fs");
		m_camera = RM::getInstance().camera;
		m_ctrl = RM::getInstance().ctrl;

		GLfloat skyboxVertices[] = {
	        // Positions
	        -1.0f,  1.0f, -1.0f,
	        -1.0f, -1.0f, -1.0f,
	        1.0f, -1.0f, -1.0f,
	        1.0f, -1.0f, -1.0f,
	        1.0f,  1.0f, -1.0f,
	        -1.0f,  1.0f, -1.0f,
	        
	        -1.0f, -1.0f,  1.0f,
	        -1.0f, -1.0f, -1.0f,
	        -1.0f,  1.0f, -1.0f,
	        -1.0f,  1.0f, -1.0f,
	        -1.0f,  1.0f,  1.0f,
	        -1.0f, -1.0f,  1.0f,
	        
	        1.0f, -1.0f, -1.0f,
	        1.0f, -1.0f,  1.0f,
	        1.0f,  1.0f,  1.0f,
	        1.0f,  1.0f,  1.0f,
	        1.0f,  1.0f, -1.0f,
	        1.0f, -1.0f, -1.0f,
	        
	        -1.0f, -1.0f,  1.0f,
	        -1.0f,  1.0f,  1.0f,
	        1.0f,  1.0f,  1.0f,
	        1.0f,  1.0f,  1.0f,
	        1.0f, -1.0f,  1.0f,
	        -1.0f, -1.0f,  1.0f,
	        
	        -1.0f,  1.0f, -1.0f,
	        1.0f,  1.0f, -1.0f,
	        1.0f,  1.0f,  1.0f,
	        1.0f,  1.0f,  1.0f,
	        -1.0f,  1.0f,  1.0f,
	        -1.0f,  1.0f, -1.0f,
	        
	        -1.0f, -1.0f, -1.0f,
	        -1.0f, -1.0f,  1.0f,
	        1.0f, -1.0f, -1.0f,
	        1.0f, -1.0f, -1.0f,
	        -1.0f, -1.0f,  1.0f,
	        1.0f, -1.0f,  1.0f
	    };

	    GLuint vbo;
	    glGenVertexArrays(1, &m_vao);
	    glGenBuffers(1, &vbo);
	    glBindVertexArray(m_vao);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	    glBindVertexArray(0);

	    // skybox initialization
	    std::stringstream ss;
	    ss << "../resources/skybox/";
	    std::stringstream faces[6];
	    std::string png = ".png";
	    faces[0] << ss.str() << "right" << png;
	    faces[1] << ss.str() << "left" << png;
	    faces[2] << ss.str() << "top" << png;
	    faces[3] << ss.str() << "bottom" << png;
	    faces[4] << ss.str() << "back" << png;
	    faces[5] << ss.str() << "front" << png;

	    glGenTextures(1, &m_skyboxTexture);
	   	
	    unsigned char* image;
	    
	    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	    for(size_t i = 0; i < 6; i++)
	    {
	    	int width = 0, height = 0, nrComponents = 0;
	        image = stbi_load(faces[i].str().c_str(), &width, &height, &nrComponents, 0);
	        if (image) {
	        	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	        	stbi_image_free(image);
	        } else {
	        	std::cout << "Image failed to load at path: " << faces[i].str().c_str() << std::endl;
	        	stbi_image_free(image);
	        }
	        
	    }
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	    RM::getInstance().skyboxTexture = &m_skyboxTexture;
	}

	void advance() {
		glDisable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        m_shader->Use();

        int w, h;
	    glfwGetFramebufferSize(m_window, &w, &h);
        glm::mat4 view = m_camera->GetViewMatrix();
	    glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
	    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(m_camera->Position.x, m_camera->Position.y, m_camera->Position.z));
	    GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
	    GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
	    GLint modeLoc  = glGetUniformLocation(m_shader->Program, "model");
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	    glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
        // skybox cube
        glBindVertexArray(m_vao);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(m_shader->Program, "skybox"), 1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
//        glDepthFunc(GL_LESS); // Set depth function back to default
        glEnable(GL_DEPTH_TEST);
	}
	
private:
	GLFWwindow * m_window;
	Shader * m_shader;
	Camera * m_camera;
	Ctrl * m_ctrl;

	GLuint m_vao;
	GLuint m_skyboxTexture;
};