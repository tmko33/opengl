#pragma once
#include <GL/freeglut.h>
#include <iostream>

namespace cwc
{
    class glutWindow;

    class glApplication
    {
    public:
        glApplication() {}
        virtual ~glApplication() {}

        virtual void OnInit() {}

        static glApplication *theApp;
        static glutWindow *theWindow;

        void run();
    };
}
