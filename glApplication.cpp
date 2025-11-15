#include "glApplication.h"
#include "glutWindow.h"

namespace cwc
{
    glApplication *glApplication::theApp = nullptr;
    glutWindow *glApplication::theWindow = nullptr;

    void glApplication::run()
    {
        int argc = 1;
        char *argv[1] = {(char *)"App"};

        theApp = this;

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

        OnInit();

        if (theWindow)
            theWindow->Create();  // Llama a la ventana

        glutMainLoop();
    }
}
