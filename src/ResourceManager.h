#pragma once

#include <GL/glew.h>

class RM {
private:
	RM() {

		// GLfloat p_vertices[] = {
		// 	0.0, 0.0, 0.0
		// };

		// glGenp_vertexArrays(1, &pVAO);
  //       glGenBuffers(1, &pVBO);

  //       glBindBuffer(GL_ARRAY_BUFFER, pVBO);
  //       glBufferData(GL_ARRAY_BUFFER, sizeof(p_vertices), p_vertices, GL_STATIC_DRAW);

  //       glBindp_vertexArray(pVAO);

  //       glp_vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  //       glEnablep_vertexAttribArray(0);
  //       glBindp_vertexArray(0);

		// p data initialization
		p_hori = 20, p_vert = 40;
        float delta_h = M_PI / p_hori, delta_v = 2 * M_PI / p_vert;
        float phi0, phi1, theta0, theta1;
        float x00, x01, x10, x11, y0, y1, z00, z01, z10, z11;

        GLfloat * p_vertices = new GLfloat[36 * 8 * p_hori * p_vert];

        int c = 0;

        for (int i=0; i<p_hori; ++i) {

            phi0 = M_PI/2 + i * delta_h;
            phi1 = M_PI/2 + (i+1) * delta_h;

            y0 = cos(phi0);
            y1 = cos(phi1);

            for (int j=0; j<2*p_vert; ++j) {

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

                p_vertices[c] = p_vertices[c+3] = x00;
                p_vertices[c+1] = p_vertices[c+4] = y0;
                p_vertices[c+2] = p_vertices[c+5] = z00;

                p_vertices[c+6] = p_vertices[c+9] = x11;
                p_vertices[c+7] = p_vertices[c+10] = y1;
                p_vertices[c+8] = p_vertices[c+11] = z11;

                p_vertices[c+12] = p_vertices[c+15] = x10;
                p_vertices[c+13] = p_vertices[c+16] = y1;
                p_vertices[c+14] = p_vertices[c+17] = z10;

                p_vertices[c+18] = p_vertices[c+21] = x00;
                p_vertices[c+19] = p_vertices[c+22] = y0;
                p_vertices[c+20] = p_vertices[c+23] = z00;

                p_vertices[c+24] = p_vertices[c+27] = x01;
                p_vertices[c+25] = p_vertices[c+28] = y0;
                p_vertices[c+26] = p_vertices[c+29] = z01;

                p_vertices[c+30] = p_vertices[c+33] = x11;
                p_vertices[c+31] = p_vertices[c+34] = y1;
                p_vertices[c+32] = p_vertices[c+35] = z11;

                c += 36;
            }

            y0 = cos(M_PI - phi0);
            y1 = cos(M_PI - phi1);
            for (int j=0; j<2*p_vert; ++j) {
                for (int k=0; k<36; ++k) {
                    p_vertices[c+k] = p_vertices[c+k-2*p_vert*36];
                    p_vertices[c+1] = p_vertices[c+4] = p_vertices[c+19] = p_vertices[c+22] = p_vertices[c+25] = p_vertices[c+28] = y0;
                    p_vertices[c+7] = p_vertices[c+10] = p_vertices[c+13] = p_vertices[c+16] = p_vertices[c+31] = p_vertices[c+34] = y1;
                }
                c += 36;
            }
        }

        glGenVertexArrays(1, &p_VAO);
        glGenBuffers(1, &p_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, p_VBO);
        glBufferData(GL_ARRAY_BUFFER, 36 * 8 * p_hori * p_vert, p_vertices, GL_STATIC_DRAW);

        glBindVertexArray(p_VAO);

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

	GLuint p_VAO, p_VBO;
	int p_hori, p_vert;
};