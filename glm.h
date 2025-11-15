/*
 * glm.h
 * Nate Robins, 1997
 * OBJ model loader
 */

#ifndef GLM_H
#define GLM_H

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GLMmaterial {
    char* name;
    GLfloat diffuse[4];
    GLfloat ambient[4];
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shininess;
} GLMmaterial;

typedef struct _GLMtriangle {
    GLuint vindices[3];
    GLuint nindices[3];
    GLuint tindices[3];
} GLMtriangle;

typedef struct _GLMgroup {
    char* name;
    GLuint numtriangles;
    GLuint* triangles;
    GLuint material;
    struct _GLMgroup* next;
} GLMgroup;

typedef struct _GLMmodel {
    char* pathname;
    char* mtllibname;

    GLuint numvertices;
    GLfloat* vertices;

    GLuint numnormals;
    GLfloat* normals;

    GLuint numtexcoords;
    GLfloat* texcoords;

    GLuint numtriangles;
    GLMtriangle* triangles;

    GLuint nummaterials;
    GLMmaterial* materials;

    GLuint numgroups;
    GLMgroup* groups;

    GLfloat position[3];
} GLMmodel;

GLMmodel* glmReadOBJ(char* filename);
void glmUnitize(GLMmodel* model);
void glmFacetNormals(GLMmodel* model);
void glmVertexNormals(GLMmodel* model, GLfloat angle);
void glmDraw(GLMmodel* model, GLuint mode);

#define GLM_NONE     0
#define GLM_FLAT     1
#define GLM_SMOOTH   2
#define GLM_TEXTURE  4
#define GLM_COLOR    8
#define GLM_MATERIAL 16

#ifdef __cplusplus
}
#endif

#endif /* GLM_H */
