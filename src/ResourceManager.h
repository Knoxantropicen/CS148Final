#pragma once

#include <GL/glew.h>

class RM {
private:
	RM() {

		// GLfloat vertices[] = {
		// 	0.0, 0.0, 0.0
		// };

		// glGenVertexArrays(1, &particleVAO);
  //       glGenBuffers(1, &particleVBO);

  //       glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
  //       glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //       glBindVertexArray(particleVAO);

  //       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  //       glEnableVertexAttribArray(0);
  //       glBindVertexArray(0);

		// particle data initialization
		int hori = 10, vert = 20;
        float delta_h = M_PI / hori, delta_v = 2 * M_PI / vert;
        float phi0, phi1, theta0, theta1;
        float x00, x01, x10, x11, y0, y1, z00, z01, z10, z11;

        GLfloat * vertices = new GLfloat[36 * 8 * hori * vert];

        int c = 0;

        for (int i=0; i<hori; ++i) {

            phi0 = M_PI/2 + i * delta_h;
            phi1 = M_PI/2 + (i+1) * delta_h;

            y0 = cos(phi0);
            y1 = cos(phi1);

            for (int j=0; j<2*vert; ++j) {

                theta0 = j * delta_v;
                theta1 = (j+1) * delta_v;

                x00 = cos(theta0) * sin(phi0);
                x10 = cos(theta0) * sin(phi1);
                x01 = cos(theta1) * sin(phi0);
                x11 = cos(theta1) * sin(phi1);
                z00 = sin(theta0) * sin(phi0);
                z10 = sin(theta0) * sin(phi1);
                z01 = sin(theta1) * sin(phi0);
                z11 = sin(theta1) * sin(phi1);

                vertices[c] = vertices[c+3] = x00;
                vertices[c+1] = vertices[c+4] = y0;
                vertices[c+2] = vertices[c+5] = z00;

                vertices[c+6] = vertices[c+9] = x11;
                vertices[c+7] = vertices[c+10] = y1;
                vertices[c+8] = vertices[c+11] = z11;

                vertices[c+12] = vertices[c+15] = x10;
                vertices[c+13] = vertices[c+16] = y1;
                vertices[c+14] = vertices[c+17] = z10;

                vertices[c+18] = vertices[c+21] = x00;
                vertices[c+19] = vertices[c+22] = y0;
                vertices[c+20] = vertices[c+23] = z00;

                vertices[c+24] = vertices[c+27] = x01;
                vertices[c+25] = vertices[c+28] = y0;
                vertices[c+26] = vertices[c+29] = z01;

                vertices[c+30] = vertices[c+33] = x11;
                vertices[c+31] = vertices[c+34] = y1;
                vertices[c+32] = vertices[c+35] = z11;

                c += 36;
            }

            y0 = cos(M_PI - phi0);
            y1 = cos(M_PI - phi1);
            for (int j=0; j<2*vert; ++j) {
                for (int k=0; k<36; ++k) {
                    vertices[c+k] = vertices[c+k-2*vert*36];
                    vertices[c+1] = vertices[c+4] = vertices[c+19] = vertices[c+22] = vertices[c+25] = vertices[c+28] = y0;
                    vertices[c+7] = vertices[c+10] = vertices[c+13] = vertices[c+16] = vertices[c+31] = vertices[c+34] = y1;
                }
                c += 36;
            }
        }

        glGenVertexArrays(1, &particleVAO);
        glGenBuffers(1, &particleVBO);

        glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
        glBufferData(GL_ARRAY_BUFFER, 36 * 8 * hori * vert, vertices, GL_STATIC_DRAW);

        glBindVertexArray(particleVAO);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

	}
	~RM() {}
	RM(RM const& copy);
	RM& operator=(RM const& copy);
public:
	static RM& getInstance() {
		static RM instance;
		return instance;
	}

	GLuint particleVAO, particleVBO;
	bool particleSettled;
};