#pragma once

#include "resource.h"

#include "DIBSection.h"

class CGFrameApp
{
public:
    CGFrameApp();
    ~CGFrameApp();

    BOOL            InitInstance(HINSTANCE hInstance, int nCmdShow);
    int             ExitInstance();
    int             Run();

    void            DrawFrame();

    const iPoint    GetFrameSize() const;
    void            SetFrameSize( int width, int height );

    void            SetPixel( int x, int y, RGBPIXEL color );
    const RGBPIXEL  GetPixel( int x,int y ) const;
    void            ClearFrame( RGBPIXEL color );
    void            DrawRectangle( int x0, int y0, int x1, int y1, RGBPIXEL color );
    void            DrawText( int x, int y, const TCHAR* str, RGBPIXEL color );

    RGBPIXEL*       GetFrameBuffer();

    unsigned int    GetAppTime() const; 

    const iPoint    GetMousePos() const;
    HWND            GetMainWnd() const      { return m_hWnd; }
    const TCHAR*    GetTitle() const        { return m_strTitle; }

private:
    HINSTANCE       m_hInstance;                      // current instance
    TCHAR           m_strTitle[128];                  // The title bar text
    TCHAR           m_strWindowClass[128];            // The main window class name
    HWND            m_hWnd;

    CDIBSection     m_Bitmap;
    bool            m_bNeedUpdate;

    DWORD           m_dwStartTime;

    bool            m_bShowFrameRate;

    ATOM            RegisterClass();
    void            CheckMenuItem(UINT id, bool bCheck);

    void            DrawBitmap( HDC hDC );
    void            DrawFPS( HDC hDC );

    void            OnIdle();
    void            OnEditCopy();
    void            OnKeyDown(UINT charCode, UINT repCnt, UINT flags);
    void            OnKeyUp(UINT charCode, UINT repCnt, UINT flags);
    void            OnLButtonDown(UINT flags, iPoint point);
    void            OnRButtonDown(UINT flags, iPoint point);
    void            OnShowFramerate();
    void            OnFileSave();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

extern CGFrameApp theApp;
