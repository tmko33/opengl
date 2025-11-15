#pragma once
#include <GL/freeglut.h>

namespace cwc
{
    class glutWindow
    {
    public:
        glutWindow();
        virtual ~glutWindow();

        void Create();

        virtual void OnInit() {}
        virtual void OnRender() {}
        virtual void OnResize(int w, int h) {}
        virtual void OnClose() {}

        virtual void OnKeyDown(int key, char c) {}
        virtual void OnKeyUp(int key, char c) {}

        virtual void OnMouseDown(int b, int x, int y) {}
        virtual void OnMouseUp(int b, int x, int y) {}
        virtual void OnMouseWheel(int n, int d, int x, int y) {}

        virtual void OnIdle() {}

        void Repaint();

        int width, height;
        static glutWindow *activeWindow;
    };
}
