#pragma once

#include "ResourceManager.h"

class Skybox {
public:
	Skybox() {

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
	    ss << "../resources/skybox/nightsky/";
	    std::stringstream faces[6];
	    std::string jpg = ".jpg";
	    std::string png = ".png";
	    std::string tga = ".tga";
	    std::string format = tga;

	    faces[0] << ss.str() << "right" << format;
	    faces[1] << ss.str() << "left" << format;
	    faces[2] << ss.str() << "top" << format;
	    faces[3] << ss.str() << "bottom" << format;
	    faces[4] << ss.str() << "back" << format;
	    faces[5] << ss.str() << "front" << format;

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

	    m_shader->Use();

	    glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(m_shader->Program, "skybox"), 1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	    int w, h;
	    glfwGetFramebufferSize(RM::getInstance().window, &w, &h);
	    glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
		GLint projLoc  = glGetUniformLocation(m_shader->Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void render() const {
		glDisable(GL_DEPTH_TEST);

        m_shader->Use();

        glm::mat4 view = m_camera->GetViewMatrix();
	    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(m_camera->Position.x, m_camera->Position.y, m_camera->Position.z));
	    model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
	    GLint viewLoc  = glGetUniformLocation(m_shader->Program, "view");
	    GLint modeLoc  = glGetUniformLocation(m_shader->Program, "model");
	    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	    glUniformMatrix4fv(modeLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        glBindVertexArray(m_vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
	}

	void advance() {
		render();
	}
	
private:

	Shader * m_shader;
	Camera * m_camera;
	Ctrl * m_ctrl;

	GLuint m_vao;
	GLuint m_skyboxTexture;
};