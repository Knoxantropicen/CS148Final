#pragma once

#include <assimp/cimport.h> // C importer 
#include <assimp/scene.h> // collects data 
#include <assimp/postprocess.h> // various extra operations 
#include <stdlib.h> // memory management 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


bool load_mesh(const char* file_name, GLuint* vao, int* point_count) { 
	
	/* load file with assimp and print some stats */ 
	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate); 
	if (!scene) { 
		fprintf (stderr, "ERROR: reading mesh %s\n", file_name); 
		return false; 
	} 
	printf("%i animations\n", scene->mNumAnimations); 
	printf("%i cameras\n", scene->mNumCameras); 
	printf("%i lights\n", scene->mNumLights); 
	printf("%i materials\n", scene->mNumMaterials); 
	printf("%i meshes\n", scene->mNumMeshes); 
	printf("%i textures\n", scene->mNumTextures); 

	*point_count = 0;

	int meshNum = scene->mNumMeshes;
	const aiMesh* mesh; 
	const aiMaterial* mtl;


	for (int i = 0; i < meshNum; ++i) {
		mesh = scene->mMeshes[i];
		mtl = scene->mMaterials[mesh->mMaterialIndex];
		*point_count += mesh->mNumVertices;
		printf ("%i vertices in mesh[%i]\n", mesh->mNumVertices, i); 
	}

	mesh = scene->mMeshes[0];

	/* generate a VAO, using the pass-by-reference parameter that we give to the function */ 
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	/* we really need to copy out all the data from AssImp's funny little data structures into pure contiguous arrays before we copy it into data buffers because assimp's texture coordinates are not really contiguous in memory. i allocate some dynamic memory to do this. */ 
	GLfloat* points = NULL; // array of vertex points 
	GLfloat* normals = NULL; // array of vertex normals 
	GLfloat* texcoords = NULL; // array of texture coordinates 

	if (mesh->HasPositions()) { 
		points = (GLfloat*)malloc(*point_count * 3 * sizeof(GLfloat)); 
		int c = 0;
		for (int j = 0; j < meshNum; ++j) {
			mesh = scene->mMeshes[j];
			int meshPointCount = mesh->mNumVertices;
			for (int i = 0; i < meshPointCount; i++) { 
				const aiVector3D* vp = &(mesh->mVertices[i]); 
				points[c] = (GLfloat)vp->x; 
				points[c + 1] = (GLfloat)vp->y; 
				points[c + 2] = (GLfloat)vp->z;
				c += 3;
			} 
		}
	} 
	if (mesh->HasNormals()) { 
		normals = (GLfloat*)malloc(*point_count * 3 * sizeof(GLfloat)); 
		int c = 0;
		for (int j = 0; j < meshNum; ++j) {
			mesh = scene->mMeshes[j];
			int meshPointCount = mesh->mNumVertices;
			for (int i = 0; i < meshPointCount; i++) { 
				const aiVector3D* vn = &(mesh->mNormals[i]); 
				normals[c] = (GLfloat)vn->x; 
				normals[c + 1] = (GLfloat)vn->y; 
				normals[c + 2] = (GLfloat)vn->z; 
				c += 3;
			} 
		}
	} 
	if (mesh->HasTextureCoords(0)) { 
		texcoords = (GLfloat*)malloc(*point_count * 2 * sizeof(GLfloat)); 
		int c = 0;
		for (int j = 0; j < meshNum; ++j) {
			mesh = scene->mMeshes[j];
			int meshPointCount = mesh->mNumVertices;
			for (int i = 0; i < meshPointCount; i++) { 
				const aiVector3D* vt = &(mesh->mTextureCoords[0][i]); 
				texcoords[c] = (GLfloat)vt->x; 
				texcoords[c + 1] = (GLfloat)vt->y; 
				c += 2;
			} 
		}
	}

	/* copy mesh data into VBOs */
	if (mesh->HasPositions()) { 
		GLuint vbo;
		glGenBuffers(1, &vbo); 
		glBindBuffer(GL_ARRAY_BUFFER, vbo); 
		glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), points, GL_STATIC_DRAW); 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
		glEnableVertexAttribArray(0);
		free(points); // free our temporary memory 
	} 
	if (mesh->HasNormals()) { 
		GLuint vbo; 
		glGenBuffers(1, &vbo); 
		glBindBuffer(GL_ARRAY_BUFFER, vbo); 
		glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), normals, GL_STATIC_DRAW); 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
		glEnableVertexAttribArray(1); 
		free (normals); // free our temporary memory 
	} 
	if (mesh->HasTextureCoords(0)) {
		GLuint vbo; 
		glGenBuffers(1, &vbo); 
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * *point_count * sizeof(GLfloat), texcoords, GL_STATIC_DRAW); 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2); 
		free(texcoords); // free our temporary memory 
	} 
	if (mesh->HasTangentsAndBitangents()) { 
		// NB: could store/ print tangents here 
	} 

	/* free assimp's copy of memory */ 
	aiReleaseImport(scene);

	printf("mesh loaded\n"); 
	return true;
}

