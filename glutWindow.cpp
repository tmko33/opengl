#include "glutWindow.h"
#include "glApplication.h"

namespace cwc
{
    glutWindow *glutWindow::activeWindow = nullptr;

    // --- Callbacks estáticos usados por GLUT ---
    static void displayCallback()
    {
        if (glutWindow::activeWindow)
            glutWindow::activeWindow->OnRender();
    }

    static void reshapeCallback(int w, int h)
    {
        if (glutWindow::activeWindow)
            glutWindow::activeWindow->OnResize(w, h);
    }

    static void idleCallback()
    {
        if (glutWindow::activeWindow)
            glutWindow::activeWindow->OnIdle();
    }

    static void keyboardDownCallback(unsigned char key, int x, int y)
    {
        if (glutWindow::activeWindow)
            glutWindow::activeWindow->OnKeyDown((int)key, key);
    }

    static void keyboardUpCallback(unsigned char key, int x, int y)
    {
        if (glutWindow::activeWindow)
            glutWindow::activeWindow->OnKeyUp((int)key, key);
    }

    glutWindow::glutWindow()
    {
        width = 800;
        height = 600;
        glApplication::theWindow = this;
    }

    glutWindow::~glutWindow() {}

    void glutWindow::Create()
    {
        glutInitWindowSize(width, height);
        glutCreateWindow("Proyecto 3D Phanor");

        activeWindow = this;
        OnInit();

        glutDisplayFunc(displayCallback);
        glutReshapeFunc(reshapeCallback);
        glutIdleFunc(idleCallback);
        glutKeyboardFunc(keyboardDownCallback);
        glutKeyboardUpFunc(keyboardUpCallback);
    }

    void glutWindow::Repaint()
    {
        glutPostRedisplay();
    }
}
