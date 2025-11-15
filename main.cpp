#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include <FreeImage.h>

#define NT 5
#define NO 10

//-----------------------------------------------------------------------------

class myWindow : public cwc::glutWindow
{
protected:
    cwc::glShaderManager SM;
    cwc::glShader *shader;
    cwc::glShader *shader1;
    GLuint ProgramObject;
    clock_t time0, time1;
    float timer010;
    bool bUp;

    // Modelos
    GLMmodel *objmodel_ptr;
    GLMmodel *objmodel_ptr1;
    GLMmodel *objmodel_ptr2;
    GLMmodel *objmodel_ptr3;
    GLMmodel *objmodel_ptr4;
    GLMmodel *objmodel_ptr5;
    GLMmodel *objmodel_ptr6;
    GLMmodel *objmodel_ptr7;
    GLMmodel *objmodel_ptr8;

    GLuint texid[NT];

public:
    myWindow() {}

    // Cargar textura JPG
    void initialize_textures(int nt, char *nombre)
    {
        glGenTextures(1, &texid[nt]);
        glBindTexture(GL_TEXTURE_2D, texid[nt]);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        FIBITMAP *bitmap = FreeImage_Load(
            FreeImage_GetFileType(nombre, 0), nombre);
        FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);

        int w = FreeImage_GetWidth(pImage);
        int h = FreeImage_GetHeight(pImage);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h,
                     0, GL_BGRA, GL_UNSIGNED_BYTE,
                     FreeImage_GetBits(pImage));

        FreeImage_Unload(pImage);
        glEnable(GL_TEXTURE_2D);
    }

    // Cargar malla
    void openMesh(GLMmodel **ptr, float n, char *file)
    {
        if (!(*ptr))
        {
            *ptr = glmReadOBJ(file);
            glmUnitize(*ptr);
            glmFacetNormals(*ptr);
            glmVertexNormals(*ptr, n);
        }
    }

    // Dibujar con textura y escala
    void openShaderTexScaled(float xt, float yt, float zt,
                             float xs, float ys, float zs,
                             GLuint t, GLMmodel **ptr)
    {
        glPushMatrix();
        glTranslatef(xt, yt, zt);
        glBindTexture(GL_TEXTURE_2D, t);
        glScalef(xs, ys, zs);
        glmDraw(*ptr, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
        glPopMatrix();
    }

    // Dibujar sin textura
    void openShaderNotTex(float xt, float yt, float zt,
                          GLfloat rA, GLfloat xr, GLfloat yr, GLfloat zr,
                          GLMmodel **ptr)
    {
        glPushMatrix();
        glTranslatef(xt, yt, zt);
        glRotatef(rA, xr, yr, zr);
        glmDraw(*ptr, GLM_SMOOTH | GLM_MATERIAL);
        glPopMatrix();
    }

    // ---------------- RENDER ---------------------

    virtual void OnRender(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
        glTranslatef(0.0, -0.3, 1.8);

        if (shader)
            shader->begin();

        // Tierra (más abajo y atrás)
        openShaderNotTex(0.0f, -0.20f, -0.10f, 0, 0, 0, 0, &objmodel_ptr);

        // Río (más atrás, más plano)
        openShaderNotTex(0.0f, -0.18f, -0.45f, 180, 0, 0, 0, &objmodel_ptr1);

        // PALMERAS (arco)
        openShaderNotTex(-0.40f, 0.05f, -0.15f, 0, 0, 1, 0, &objmodel_ptr3);
        openShaderNotTex(-0.25f, 0.07f, -0.23f, 0, 0, 1, 0, &objmodel_ptr3);
        openShaderNotTex(-0.10f, 0.08f, -0.28f, 0, 0, 1, 0, &objmodel_ptr3);
        openShaderNotTex(0.10f, 0.08f, -0.28f, 0, 0, 1, 0, &objmodel_ptr3);
        openShaderNotTex(0.25f, 0.07f, -0.23f, 0, 0, 1, 0, &objmodel_ptr3);
        openShaderNotTex(0.40f, 0.05f, -0.15f, 0, 0, 1, 0, &objmodel_ptr3);

        if (shader)
            shader->end();

        // TEXTURAS (casa)
        if (shader1)
            shader1->begin();

        // Cimientos (casa movida a la izquierda)
        openShaderTexScaled(-0.25f, -0.05f, -0.05f,
                            0.22f, 0.22f, 0.22f,
                            texid[0], &objmodel_ptr2);

        // Piso casa
        openShaderTexScaled(-0.27f, 0.0f, -0.03f,
                            0.22f, 0.22f, 0.22f,
                            texid[1], &objmodel_ptr4);

        // Fachada
        openShaderTexScaled(-0.29f, 0.065f, -0.14f,
                            0.09f, 0.09f, 0.09f,
                            texid[2], &objmodel_ptr5);

        // Techo
        openShaderTexScaled(-0.29f, 0.135f, -0.14f,
                            0.11f, 0.11f, 0.11f,
                            texid[3], &objmodel_ptr6);

        // Ventanas
        openShaderTexScaled(-0.29f, -0.01f, -0.12f,
                            0.09f, 0.09f, 0.11f,
                            texid[4], &objmodel_ptr8);

        if (shader1)
            shader1->end();

        glutSwapBuffers();
        glPopMatrix();

        UpdateTimer();
        Repaint();
    }

    // ---------------- INIT ---------------------

    virtual void OnInit()
    {
        glClearColor(0.9f, 0.55f, 0.3f, 1.0f); // Fondo atardecer
        glEnable(GL_DEPTH_TEST);

        shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt");
        shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt");

        time0 = clock();
        timer010 = 0.0f;
        bUp = true;

        // Cargar mallas
        objmodel_ptr = NULL;  openMesh(&objmodel_ptr, 90, "./Mallas/tierra.obj");
        objmodel_ptr1 = NULL; openMesh(&objmodel_ptr1, 90, "./Mallas/rio.obj");
        objmodel_ptr2 = NULL; openMesh(&objmodel_ptr2, 90, "./Mallas/cimientosCasa.obj");
        objmodel_ptr3 = NULL; openMesh(&objmodel_ptr3, 90, "./Mallas/palmeras.obj");
        objmodel_ptr4 = NULL; openMesh(&objmodel_ptr4, 90, "./Mallas/pisoCasa.obj");
        objmodel_ptr5 = NULL; openMesh(&objmodel_ptr5, 90, "./Mallas/estructuraCasa.obj");
        objmodel_ptr6 = NULL; openMesh(&objmodel_ptr6, 90, "./Mallas/techo.obj");
        objmodel_ptr7 = NULL; openMesh(&objmodel_ptr7, 90, "./Mallas/ventanas.obj");
        objmodel_ptr8 = NULL; openMesh(&objmodel_ptr8, 90, "./Mallas/pisoDentroCasa.obj");

        // Texturas
        initialize_textures(0, "./Mallas/cimientosTextura.jpg");
        initialize_textures(1, "./Mallas/pisoTextura.jpg");
        initialize_textures(2, "./Mallas/fachadaCasa.jpg");
        initialize_textures(3, "./Mallas/techoTextura.jpg");
        initialize_textures(4, "./Mallas/pisoDentroTextura.jpg");

        DemoLight();
    }

    // ---------------- LUZ ---------------------

    void DemoLight()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);

        GLfloat light_pos[] = {0.5f, 1.5f, 2.0f, 1.0f};

        GLfloat light_Ka[] = {0.9f, 0.5f, 0.2f, 1.0f};
        GLfloat light_Kd[] = {1.0f, 0.8f, 0.6f, 1.0f};
        GLfloat light_Ks[] = {1.0f, 1.0f, 1.0f, 1.0f};

        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
    }

    // ---------------- Cámara ---------------------

    virtual void OnResize(int w, int h)
    {
        if (h == 0)
            h = 1;

        float ratio = 1.0f * w / h;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glViewport(0, 0, w, h);
        gluPerspective(45, ratio, 1, 100);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(0.6f, 0.5f, 4.5f,     // cámara nueva
                  0.0, 0.0, -1.0,
                  0.0f, 1.0f, 0.0f);
    }

    // ---------------- Timer ---------------------

    void UpdateTimer()
    {
        time1 = clock();
        float delta = (float)(time1 - time0) / CLOCKS_PER_SEC / 4;

        if (delta > 0.00005f)
        {
            time0 = clock();
            if (bUp)
            {
                timer010 += delta;
                if (timer010 >= 1.0f) { timer010 = 1.0f; bUp = false; }
            }
            else
            {
                timer010 -= delta;
                if (timer010 <= 0.0f) { timer010 = 0.0f; bUp = true; }
            }
        }
    }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
    virtual void OnInit() { std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
    myApplication *pApp = new myApplication;
    myWindow *myWin = new myWindow();

    pApp->run();
    delete pApp;

    return 0;
}

//-----------------------------------------------------------------------------
