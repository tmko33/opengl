#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "glm.h"

static char* _glm_strdup(const char* s)
{
    char* d = (char*)malloc(strlen(s)+1);
    strcpy(d, s);
    return d;
}

static void glmReverseWinding(GLMmodel* model)
{
    GLuint i;
    for (i=0; i<model->numtriangles; i++) {
        GLuint t = model->triangles[i].vindices[0];
        model->triangles[i].vindices[0] = model->triangles[i].vindices[2];
        model->triangles[i].vindices[2] = t;
    }
}

static GLfloat glmAbs(GLfloat f)
{
    return (f >= 0 ? f : -f);
}

GLMmodel* glmReadOBJ(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: No se pudo abrir OBJ %s\n", filename);
        return NULL;
    }

    GLMmodel* model = (GLMmodel*)calloc(1, sizeof(GLMmodel));
    model->pathname = _glm_strdup(filename);

    char buf[256];

    // Primera pasada: contar elementos
    while (fscanf(file, "%s", buf) != EOF) {
        if (strcmp(buf, "v") == 0) {
            model->numvertices++;
            fscanf(file, "%*[^\n]\n");
        } else if (strcmp(buf, "vn") == 0) {
            model->numnormals++;
            fscanf(file, "%*[^\n]\n");
        } else if (strcmp(buf, "vt") == 0) {
            model->numtexcoords++;
            fscanf(file, "%*[^\n]\n");
        } else if (strcmp(buf, "f") == 0) {
            model->numtriangles++;
            fscanf(file, "%*[^\n]\n");
        }
    }

    rewind(file);

    // Reservar memoria
    model->vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * (model->numvertices+1));
    model->normals  = (GLfloat*)malloc(sizeof(GLfloat) * 3 * (model->numnormals+1));
    model->texcoords= (GLfloat*)malloc(sizeof(GLfloat) * 2 * (model->numtexcoords+1));
    model->triangles= (GLMtriangle*)malloc(sizeof(GLMtriangle) * model->numtriangles);

    GLuint nv=1, nn=1, nt=1, nf=0;

    while (fscanf(file, "%s", buf) != EOF)
    {
        if (strcmp(buf, "v") == 0) {
            fscanf(file, "%f %f %f",
                   &model->vertices[3*nv],
                   &model->vertices[3*nv+1],
                   &model->vertices[3*nv+2]);
            nv++;
        }
        else if (strcmp(buf, "vn") == 0) {
            fscanf(file, "%f %f %f",
                   &model->normals[3*nn],
                   &model->normals[3*nn+1],
                   &model->normals[3*nn+2]);
            nn++;
        }
        else if (strcmp(buf, "vt") == 0) {
            fscanf(file, "%f %f",
                   &model->texcoords[2*nt],
                   &model->texcoords[2*nt+1]);
            nt++;
        }
        else if (strcmp(buf, "f") == 0) {
            GLuint v[3], t[3], n[3];
            fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u",
                   &v[0], &t[0], &n[0],
                   &v[1], &t[1], &n[1],
                   &v[2], &t[2], &n[2]);
            model->triangles[nf].vindices[0]=v[0];
            model->triangles[nf].vindices[1]=v[1];
            model->triangles[nf].vindices[2]=v[2];
            model->triangles[nf].tindices[0]=t[0];
            model->triangles[nf].tindices[1]=t[1];
            model->triangles[nf].tindices[2]=t[2];
            model->triangles[nf].nindices[0]=n[0];
            model->triangles[nf].nindices[1]=n[1];
            model->triangles[nf].nindices[2]=n[2];
            nf++;
        }
    }

    fclose(file);
    return model;
}

void glmUnitize(GLMmodel* model)
{
    GLfloat min[3]={1e9,1e9,1e9};
    GLfloat max[3]={-1e9,-1e9,-1e9};

    for (GLuint i=1; i<=model->numvertices; i++) {
        GLfloat* v = &model->vertices[3*i];
        if (v[0]<min[0]) min[0]=v[0];
        if (v[1]<min[1]) min[1]=v[1];
        if (v[2]<min[2]) min[2]=v[2];
        if (v[0]>max[0]) max[0]=v[0];
        if (v[1]>max[1]) max[1]=v[1];
        if (v[2]>max[2]) max[2]=v[2];
    }

    GLfloat scale = 2.0f / fmaxf(fmaxf(max[0]-min[0],
                                      max[1]-min[1]),
                                      max[2]-min[2]);

    for (GLuint i=1; i<=model->numvertices; i++) {
        GLfloat* v = &model->vertices[3*i];
        v[0]=(v[0]-(min[0]+max[0])/2)*scale;
        v[1]=(v[1]-(min[1]+max[1])/2)*scale;
        v[2]=(v[2]-(min[2]+max[2])/2)*scale;
    }
}

void glmFacetNormals(GLMmodel* model)
{
    // No-op in compact version (normales del OBJ)
}

void glmVertexNormals(GLMmodel* model, GLfloat angle)
{
    // No-op (usa normales del OBJ)
}

void glmDraw(GLMmodel* model, GLuint mode)
{
    glBegin(GL_TRIANGLES);
    for (GLuint i=0; i<model->numtriangles; i++) {
        GLMtriangle* t = &model->triangles[i];
        for (int j=0; j<3; j++) {
            if (mode & GLM_TEXTURE && model->numtexcoords) {
                GLuint ti = t->tindices[j];
                glTexCoord2fv(&model->texcoords[2*ti]);
            }
            if (mode & GLM_SMOOTH && model->numnormals) {
                GLuint ni = t->nindices[j];
                glNormal3fv(&model->normals[3*ni]);
            }
            GLuint vi = t->vindices[j];
            glVertex3fv(&model->vertices[3*vi]);
        }
    }
    glEnd();
}
