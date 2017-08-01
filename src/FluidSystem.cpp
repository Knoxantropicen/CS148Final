#include "FluidSystem.h"

FluidSystem::FluidSystem(GLFWwindow * window) {
	m_window = window;
	m_particle_shader = new Shader("./particle.vs", "./particle.frag");
	m_advect_shader = new Shader("./advect.vs", 0);

	m_total_time = 0;
	m_max_p_num = 200;

	m_camera = new Camera(glm::vec3(0.0, 0.0, 10.0));
	
	m_particles = new Particle[m_max_p_num];

	// glGenVertexArrays(1, &m_advectVAO);
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

	// glBindVertexArray(0);

	m_isFirst = true;
	m_currVB = 0;
	m_currTFB = 1;
}

void FluidSystem::update(float time_step) {

	// for (int i=0; i<200; ++i) {
	// 	double random_degree = drand48() * 2 * M_PI;
	// 	m_particles.push_back(new Particle(m_shader, glm::vec3(0, 1, 0), glm::vec3(sin(random_degree), 1, cos(random_degree))));
	// }

	// for (size_t i=0; i<m_particles.size(); ++i) {
	// 	m_particles[i]->advance(time_step);
	// }

	// while (m_particles.size() > m_max_p_num) {
	// 	Particle * curr_p = m_particles.front();
	// 	m_particles.pop_front();
	// 	delete curr_p;
	// }

	m_total_time += time_step;

	glEnable(GL_RASTERIZER_DISCARD);

	m_advect_shader->Use();
	glUniform1f(glGetUniformLocation(m_advect_shader->Program, "dt"), time_step);
	glUniform3f(glGetUniformLocation(m_advect_shader->Program, "a"), 0.0f, -0.98f, 0.0f);

	glBindVertexArray(m_particleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
	
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

void FluidSystem::render() const {
	glClearColor(0.08f, 0.08f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_particle_shader->Use();

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    glm::mat4 view;
    view = m_camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(m_camera->Zoom, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    GLint viewLoc  = glGetUniformLocation(m_particle_shader->Program, "view");
    GLint projLoc  = glGetUniformLocation(m_particle_shader->Program, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniformMatrix4fv(glGetUniformLocation(m_particle_shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

    

    glBindVertexArray(m_particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

 //    GLuint VAO, VBO;
 //    GLfloat vert[] = {0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f};
 //    glGenVertexArrays(1, &VAO);
	// glGenBuffers(1, &VBO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	// glBindVertexArray(VAO);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// glEnableVertexAttribArray(0);
	// glDrawArrays(GL_POINTS, 0, 2);
	// glBindVertexArray(0);

	glfwSwapBuffers(m_window);
}

void FluidSystem::advance(float time_step) {

	update(time_step);
	render();

	m_currVB = m_currTFB;
	m_currTFB = (m_currTFB + 1) & 0x1;
}


