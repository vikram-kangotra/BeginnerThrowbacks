#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

HGLRC hRc = NULL;
HDC hDc = NULL;
HWND hWnd = NULL;
HINSTANCE hInstance=NULL;

bool keys[256];
bool active=TRUE;
bool fullscreen=FALSE;

GLfloat rtri = 0;
GLfloat rquad = 0;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

GLvoid ReSizeGlScene(GLsizei width,GLsizei height){
    if(height == 0){
        height = 1;
    }

    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool InitGl(GLvoid){
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    return TRUE;
}

void DrawCube(GLfloat x,GLfloat y,GLfloat z){
    glTranslatef(x,y,z);
    glBegin(GL_QUADS);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(-1.0,1.0,1);
        glVertex3f(1.0,1.0,1);
        glVertex3f(1.0,-1.0,1);
        glVertex3f(-1.0,-1.0,1);

        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(-1.0,1.0,1);
        glVertex3f(-1.0,-1.0,1);
        glVertex3f(-1.0,-1.0,-1);
        glVertex3f(-1.0,1.0,-1);

        glColor3f(1.0f,0.5f,0.0f);
        glVertex3f(-1.0,1.0,-1);
        glVertex3f(1.0,1.0,-1);
        glVertex3f(1.0,-1.0,-1);
        glVertex3f(-1.0,-1.0,-1);

        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(1.0,1.0,1);
        glVertex3f(1.0,-1.0,1);
        glVertex3f(1.0,-1.0,-1);
        glVertex3f(1.0,1.0,-1);

        glColor3f(1.0,1.0,0.0);
        glVertex3f(-1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,1.0);
        glVertex3f(1.0,-1.0,-1.0);
        glVertex3f(-1.0,-1.0,-1.0);

        glColor3f(1.0,1.0,1.0);
        glVertex3f(-1.0,1.0,1.0);
        glVertex3f(1.0,1.0,1.0);
        glVertex3f(1.0,1.0,-1.0);
        glVertex3f(-1.0,1.0,-1.0);
    glEnd();
}

void DrawSlice(GLfloat angle,GLfloat x,GLfloat y,GLfloat z){
    glPushMatrix();
    glRotatef(angle,0.0,0.0,1.0);
    glTranslatef(x,y,z);
    DrawCube(0,0,0);
    DrawCube(-2,0,0);
    DrawCube(0,-2,0);
    DrawCube(2,0,0);
    DrawCube(2,0,0);
    DrawCube(0,2,0);
    DrawCube(0,2,0);
    DrawCube(-2,0,0);
    DrawCube(-2,0,0);
    glPopMatrix();
}

int DrawGlScene(GLvoid){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-15.0f);
    glRotated(rtri,1.0f,0.0,0.0);
    glRotated(rquad,0.0f,1.0,0.0);

    DrawSlice(0,0,0,-2);
    DrawSlice(0,0,0,0);
    DrawSlice(0,0,0,2);

    if(keys[VK_LEFT]){
        rquad-=0.5;
    }else if(keys[VK_RIGHT]){
        rquad+=0.5;
    }else if(keys[VK_UP]){
        rtri+=0.5;
    }else if(keys[VK_DOWN]){
        rtri-=0.5;
    }

    return TRUE;
}

GLvoid KillGlWindow(GLvoid){
    if(fullscreen){
        ChangeDisplaySettings(NULL,0);
        ShowCursor(TRUE);
    }
    if(hRc){
        if(!wglMakeCurrent(NULL,NULL)){
            MessageBox(NULL,"Release of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        }
        if(!wglDeleteContext(hRc)){
            MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
            hRc = NULL;
        }
    }
    if(hDc && !ReleaseDC(hWnd,hDc)){
        MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hDc = NULL;
    }
    if(hWnd && !DestroyWindow(hWnd)){
        MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;
    }
    if(!UnregisterClass("OpenGl",hInstance)){
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;
    }
}

BOOL CreateGlWindow(const char* title,int width,int height,int bits,bool fullscreenflag){
    GLuint PixelFormat;
    WNDCLASS wc;

    DWORD dwExStyle;
    DWORD dwStyle;

    RECT WindowRect;
    WindowRect.left = (long)0;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    WindowRect.bottom = (long)height;

    fullscreen = fullscreenflag;

    hInstance = GetModuleHandle(NULL);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OpenGL";

    if(!RegisterClass(&wc)){
        MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(fullscreen){
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL){
            if(MessageBox(NULL,"The Requested FullScreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","Graphics",MB_YESNO|MB_ICONEXCLAMATION)==IDYES){
                fullscreen = FALSE;
            }else{
                MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
                return FALSE;
            }
        }
    }

    if(fullscreen){
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor(FALSE);
    }else{
        dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
        dwStyle=WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&WindowRect,dwStyle,FALSE,dwExStyle);

    if(!(hWnd=CreateWindowEx(dwExStyle,
                             "OpenGL",
                             title,
                             WS_CLIPSIBLINGS |
                             WS_CLIPCHILDREN |
                             dwStyle,
                             0, 0,
                             WindowRect.right-WindowRect.left,
                             WindowRect.bottom-WindowRect.top,
                             NULL,
                             NULL,
                             hInstance,
                             NULL))){

        KillGlWindow();
        MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        bits,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };

    if(!(hDc = GetDC(hWnd))){
        KillGlWindow();
        MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(!(PixelFormat=ChoosePixelFormat(hDc,&pfd))){
        KillGlWindow();
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(!SetPixelFormat(hDc,PixelFormat,&pfd)){
        KillGlWindow();
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(!(hRc=wglCreateContext(hDc))){
        KillGlWindow();
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if(!wglMakeCurrent(hDc,hRc)){
        KillGlWindow();
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    ShowWindow(hWnd,SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    ReSizeGlScene(width,height);

    if(!InitGl()){
        KillGlWindow();
        MessageBox(NULL,"initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,
                         WPARAM wParam,LPARAM lParam){
    switch(uMsg){
    case WM_ACTIVATE:
        if(!HIWORD(wParam)){
            active = TRUE;
        }else{
            active=FALSE;
        }
        return 0;
    case WM_SYSCOMMAND:
        switch(wParam){
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
            return 0;
        }
        break;
    case WM_KEYDOWN:
        keys[wParam] = TRUE;
        return 0;
    case WM_KEYUP:
        keys[wParam] = FALSE;
        return 0;
    case WM_SIZE:
        ReSizeGlScene(LOWORD(lParam),HIWORD(lParam));
        return 0;
    }

    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lCmdLine,
                   int nCmdShow){

    MSG msg;
    BOOL done = FALSE;

    if(!CreateGlWindow("Graphics openGL FrameWork",640,480,16,fullscreen)){
        return 0;
    }

    while(done == FALSE){
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            if(msg.message == WM_QUIT){
                done = TRUE;
            }else{
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }else{
            if(active){
                if(keys[VK_ESCAPE]){
                    done=TRUE;
                }if(keys[VK_F1]){
                    keys[VK_F1]=FALSE;
                    KillGlWindow();
                    fullscreen=!fullscreen;
                    if(!CreateGlWindow("Graphics openGL FrameWork",640,480,16,fullscreen)){
                        return 0;
                    }
                }else{
                    DrawGlScene();
                    SwapBuffers(hDc);
                }
            }
        }
    }

    KillGlWindow();
    return (msg.wParam);
}
