// GFrameW32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GFrameW32.h"

#include "Graphics.h"

CGFrameApp theApp;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Perform application initialization:
    if( !theApp.InitInstance(hInstance, nCmdShow) )
        return 0;

    return theApp.Run();
}


CGFrameApp::CGFrameApp() :
m_hInstance(NULL), m_hWnd(NULL), m_bNeedUpdate(false), m_dwStartTime(0),
m_bShowFrameRate(false)
{
    memset(m_strTitle, 0, sizeof(m_strTitle));
    memset(m_strWindowClass, 0, sizeof(m_strWindowClass));
}

CGFrameApp::~CGFrameApp()
{}

BOOL CGFrameApp::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_hInstance = hInstance;

    ::LoadString(m_hInstance, IDS_APP_TITLE, m_strTitle, ARRAYSIZE(m_strTitle));
    ::LoadString(m_hInstance, IDC_GFRAMEW32, m_strWindowClass, ARRAYSIZE(m_strWindowClass));

    if( !m_Bitmap.Create(400, -300) )
        return FALSE;

    RegisterClass();

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_VISIBLE | WS_SYSMENU,
          styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    iRect rc = iRect( 0, 0, m_Bitmap.Width(), -m_Bitmap.Height() );
    ::AdjustWindowRectEx( (RECT*)&rc, style, TRUE, styleEx );

    m_hWnd = ::CreateWindowEx(styleEx,
                              m_strWindowClass,
                              m_strTitle, 
                              style,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              rc.Width(), rc.Height(),
                              NULL,
                              NULL,
                              m_hInstance,
                              this);
    if( m_hWnd == NULL )
        return FALSE;

    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);

    CheckMenuItem(ID_VIEW_SHOWFRAMERATE, m_bShowFrameRate);

    return TRUE;
}

int CGFrameApp::ExitInstance()
{
    return 0;
}

int CGFrameApp::Run()
{
    HACCEL hAccelTable = ::LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_GFRAMEW32));

    for( ; ; )
    {
        //  Process all pending messages 
        MSG msg;
        while( ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) ) 
        {
            if( ::GetMessage(&msg, NULL, 0, 0) )
            {
                if( !::TranslateAccelerator(msg.hwnd, hAccelTable, &msg) )
                {
                    ::TranslateMessage(&msg); 
                    ::DispatchMessage(&msg);
                }
            }
            else    // WM_QUIT message is received
                return ExitInstance();
        } 
        OnIdle(); 
    }
}

ATOM CGFrameApp::RegisterClass()
{
    WNDCLASSEX wcex = {0};

    wcex.cbSize = sizeof(wcex);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= sizeof(LONG_PTR);
    wcex.hInstance		= m_hInstance;
    wcex.hIcon			= ::LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_GFRAMEW32));
    wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)::GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GFRAMEW32);
    wcex.lpszClassName	= m_strWindowClass;
    wcex.hIconSm		= ::LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

void CGFrameApp::CheckMenuItem(UINT id, bool bCheck)
{
    HMENU hMenu = GetMenu(m_hWnd);
    UINT flags = MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED);
    ::CheckMenuItem(hMenu, id, flags);
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
LRESULT CALLBACK CGFrameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CGFrameApp* app = (CGFrameApp*)::GetWindowLongPtr(hWnd, 0);

    switch( message )
    {
    case WM_COMMAND:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            int wmId    = LOWORD(wParam);
                //wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch(wmId)
            {
            case IDM_ABOUT:
                ::DialogBox(app->m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutProc);
                break;
            case IDM_EXIT:
                ::DestroyWindow(hWnd);
                break;
            case ID_FILE_SAVE_AS:
                app->OnFileSave();
                break;
            case ID_EDIT_COPY:
                app->OnEditCopy();
                break;
            case ID_VIEW_SHOWFRAMERATE:
                app->OnShowFramerate();
            default:
                return ::DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_CREATE:
        {
            const CREATESTRUCT* cs = (const CREATESTRUCT*)lParam;
            ::SetWindowLongPtr(hWnd, 0, (LONG_PTR)cs->lpCreateParams);

            app = (CGFrameApp*)cs->lpCreateParams;

            app->m_hWnd = hWnd;
            app->m_dwStartTime = ::GetTickCount();
            if( !::gfInitScene() )
                return -1;
        }
        break;

    case WM_PAINT:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            PAINTSTRUCT ps;
            HDC hDC = ::BeginPaint(hWnd, &ps);
                
            app->DrawBitmap(hDC);

            ::EndPaint(hWnd, &ps);
        }
        break;

    case WM_ERASEBKGND:
        return TRUE;

    case WM_SIZE:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            int cx = LOWORD(lParam),
                cy = HIWORD(lParam);
	        if( cx != 0 && cy != 0 )
            {
                if( cx != app->m_Bitmap.Width() ||
                    cy != -app->m_Bitmap.Height() )
                    app->m_Bitmap.Create( cx, -cy );
                app->m_bNeedUpdate = true;
            }
        }

    case WM_KEYDOWN:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            UINT charCode = (UINT)wParam;
            UINT repCnt = (UINT)(lParam & 0x0000FFFF);
            UINT flags = (UINT)(lParam >> 16);
            app->OnKeyDown(charCode, repCnt, flags);
        }
        break;

    case WM_KEYUP:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            UINT charCode = (UINT)wParam;
            UINT repCnt = (UINT)(lParam & 0x0000FFFF);
            UINT flags = (UINT)(lParam >> 16);
            app->OnKeyUp(charCode, repCnt, flags);
        }
        break;

    case WM_LBUTTONDOWN:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            UINT flags = (UINT)wParam;
            int  x = LOWORD(lParam),
                 y = HIWORD(lParam);
            app->OnLButtonDown(flags, iPoint(x, y));
        }
        break;

    case WM_RBUTTONDOWN:
        {
            ASSERT(app != NULL);
            ASSERT(app->m_hWnd == hWnd);

            UINT flags = (UINT)wParam;
            int  x = LOWORD(lParam),
                 y = HIWORD(lParam);
            app->OnRButtonDown(flags, iPoint(x, y));
        }
        break;

    case WM_DESTROY:
        ::gfCleanupScene();
        ::PostQuitMessage(0);
        break;

    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK CGFrameApp::AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        {
            // Center dialog on the parent window
            HWND hWnd = ::GetParent(hDlg);
            iRect rcWnd;
            ::GetWindowRect(hWnd, &AsRECT(rcWnd));

            iRect rcDlg;
            ::GetWindowRect(hDlg, &AsRECT(rcDlg));

            iPoint pos(rcWnd.left + (rcWnd.Width() - rcDlg.Width()) / 2,
                       rcWnd.left + (rcWnd.Height() - rcDlg.Height()) / 2);
            //::ScreenToClient(hWnd, (POINT*)&pos);
            ::SetWindowPos(hDlg, NULL, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CGFrameApp::DrawBitmap( HDC hDC )
{
    if( m_Bitmap.Bitmap() != NULL )
    {
        HDC hMemDC = ::CreateCompatibleDC(NULL);
        HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, m_Bitmap.Bitmap());

        if( m_bShowFrameRate )
            DrawFPS(hMemDC);

        ::BitBlt(hDC, 0, 0, m_Bitmap.Width(), abs(m_Bitmap.Height()), hMemDC, 0, 0, SRCCOPY );

        ::SelectObject( hMemDC, hOldBitmap );
        ::DeleteDC(hMemDC);
    }
}

void CGFrameApp::DrawFPS( HDC hDC )
{
    static int   frames = 0,
                 prevFrames = 0;
    static float fps = 0.0;

    static clock_t prevTime = clock();

    ++frames;
    
    clock_t time = clock();
    if( time - prevTime > CLOCKS_PER_SEC )
    {
        fps = (float)(frames - prevFrames) * CLOCKS_PER_SEC / (time - prevTime);
        prevTime = time;
        prevFrames = frames;
    }

    TCHAR str[32];
    _stprintf( str, _T("Frame Rate: %5.1f"), (double)fps );

    int oldBkMode = ::SetBkMode(hDC, TRANSPARENT);
    COLORREF oldBkColor = ::SetTextColor(hDC, RGB(128, 0, 0));
    ::TextOut(hDC, 1, 1, str, (int)_tcslen(str));
    ::SetTextColor(hDC, RGB(255, 0, 0));
    ::TextOut(hDC, 0, 0, str, (int)_tcslen(str) );
    ::SetBkMode(hDC, oldBkMode);
    ::SetTextColor(hDC, oldBkColor);
}

void CGFrameApp::DrawFrame()
{
    ::gfDrawScene();
    if( m_bNeedUpdate )
    {
        ASSERT(m_hWnd != NULL);
        HDC hDC = ::GetDC(m_hWnd);
        ASSERT(hDC != NULL);

        DrawBitmap(hDC);

        ::ReleaseDC(m_hWnd, hDC);

        m_bNeedUpdate = false;
    }
}

const iPoint CGFrameApp::GetFrameSize() const
{
    return iPoint(m_Bitmap.Width(), abs(m_Bitmap.Height()));
}

void CGFrameApp::SetFrameSize( int width, int height )
{
    if( m_hWnd != NULL )
    {
        iRect rc = iRect( 0, 0, width, height );
        ::AdjustWindowRectEx( &AsRECT(rc), ::GetWindowLong(m_hWnd, GWL_STYLE), ::GetMenu(m_hWnd) != NULL, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) );
        ::SetWindowPos(m_hWnd, NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOZORDER );
    }
}

void CGFrameApp::SetPixel( int x, int y, RGBPIXEL color )
{
    if( m_Bitmap.Bits() != NULL )
    {
        if( x >= 0 && x < m_Bitmap.Width() &&
            y >= 0 && y < abs(m_Bitmap.Height()) )
        {
            m_Bitmap.Bits()[x + y * m_Bitmap.Width()] = *((const RGBQUAD*)&color);
            m_bNeedUpdate = true;
        }
    }
}

const RGBPIXEL CGFrameApp::GetPixel( int x,int y ) const
{
    if( m_Bitmap.Bits() != NULL )
    {
        if( x >= 0 && x < m_Bitmap.Width() &&
            y >= 0 && y < abs(m_Bitmap.Height()) )
            return *(const RGBPIXEL*)&m_Bitmap.Bits()[x + y * m_Bitmap.Width()];
    }
    return RGBPIXEL::Black();
}

void CGFrameApp::ClearFrame( RGBPIXEL color )
{
    DrawRectangle(0, 0, m_Bitmap.Width(), abs(m_Bitmap.Height()), color);
}

void CGFrameApp::DrawRectangle( int x0, int y0, int x1, int y1, RGBPIXEL color )
{
    if( m_Bitmap.Bits() != NULL )
    {
        iRect rcBm = iRect( 0, 0, m_Bitmap.Width(), abs(m_Bitmap.Height()) );
        iRect rc = iRect(x0, y0, x1, y1);
        rc = iRect::Intersection( rc, rcBm );
        if( !rc.IsEmpty() )
        {
            RGBPIXEL* bits = (RGBPIXEL*)m_Bitmap.Bits() + m_Bitmap.Width() * rc.top;
            for( int y = rc.top; y < rc.bottom; y++ )
            {
                for( int x = rc.left; x < rc.right; x++ )
                    bits[x] = color;
                bits += m_Bitmap.Width();
            }
            m_bNeedUpdate = true;
        }
    }
}

void CGFrameApp::DrawText( int x, int y, const TCHAR* str, RGBPIXEL color )
{
    if( m_Bitmap.Bitmap() != NULL )
    {
        HDC dc = ::CreateCompatibleDC(NULL);
        HBITMAP oldBitmap = (HBITMAP)::SelectObject(dc, m_Bitmap.Bitmap());
        ::SetTextColor(dc, RGB(color.red, color.green, color.blue));
        SetBkMode(dc, TRANSPARENT);
        ::SetTextAlign(dc, TA_TOP | TA_LEFT);
        ::TextOut(dc, x, y, str, (int)_tcslen(str));
        ::SelectObject(dc, oldBitmap);
        ::DeleteDC(dc);

        m_bNeedUpdate = true;
    }
}

RGBPIXEL* CGFrameApp::GetFrameBuffer()
{
    if( m_Bitmap.Bits() != NULL )
    {
        m_bNeedUpdate = true;
        return (RGBPIXEL*)m_Bitmap.Bits();
    }
    return NULL;
}

unsigned int CGFrameApp::GetAppTime() const
{
    return (unsigned int)(::GetTickCount() - m_dwStartTime);
}

const iPoint CGFrameApp::GetMousePos() const
{
    POINT pt;
    ::GetCursorPos(&pt);
    ::ScreenToClient(m_hWnd, &pt);
    return iPoint(pt.x, pt.y);
}

void CGFrameApp::OnIdle()
{
    DrawFrame();
}

void CGFrameApp::OnEditCopy()
{
    if( ::OpenClipboard(m_hWnd) )
    {
        ::EmptyClipboard();

	    if( !m_Bitmap.SaveToClipboard() )
            ::MessageBox( m_hWnd, _T("Cannot put data to clipboard!"), _T("Error"), MB_OK | MB_ICONERROR);

        ::CloseClipboard();
    }
}

inline bool IsFirstKeyDown( UINT nFlags )
{
    return ((nFlags & (1l << 14)) == 0);
}

void CGFrameApp::OnKeyDown(UINT charCode, UINT repCnt, UINT flags)
{
    UNREFERENCED_PARAMETER(repCnt);

	if( charCode == VK_ESCAPE )
        ::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
    else
    if( IsFirstKeyDown(flags) )
        ::gfOnKeyDown(charCode);
}

void CGFrameApp::OnKeyUp(UINT charCode, UINT repCnt, UINT flags)
{
    UNREFERENCED_PARAMETER(repCnt);
    UNREFERENCED_PARAMETER(flags);

    ::gfOnKeyUp(charCode);
}

void CGFrameApp::OnLButtonDown(UINT flags, iPoint point)
{
    UNREFERENCED_PARAMETER(flags);
    ::gfOnLMouseClick(point.x, point.y);
}

void CGFrameApp::OnRButtonDown(UINT flags, iPoint point)
{
    UNREFERENCED_PARAMETER(flags);
    ::gfOnRMouseClick(point.x, point.y);
}

void CGFrameApp::OnShowFramerate()
{
	m_bShowFrameRate = !m_bShowFrameRate;
    m_bNeedUpdate = true;

    CheckMenuItem(ID_VIEW_SHOWFRAMERATE, m_bShowFrameRate);
}

void CGFrameApp::OnFileSave()
{
    TCHAR strFileName[MAX_PATH] = _T("Screenshot.bmp");
    TCHAR strFileTitle[MAX_PATH] = _T("Screenshot.bmp");

    OPENFILENAME ofn = {0};
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = m_hWnd;
    ofn.lpstrFilter     = _T("BMP Files (*.bmp)\0*.bmp\0All files (*.*)\0*.*\0\0"); 
    ofn.lpstrFile       = strFileName; 
    ofn.nMaxFile        = ARRAYSIZE(strFileName); 
    ofn.lpstrFileTitle  = strFileTitle; 
    ofn.nMaxFileTitle   = ARRAYSIZE(strFileTitle); 
    ofn.lpstrInitialDir = NULL; 
    ofn.Flags           = OFN_ENABLESIZING | OFN_EXPLORER | OFN_NOREADONLYRETURN | OFN_NOTESTFILECREATE | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt     = _T("bmp");
    ofn.lpstrTitle      = _T("Save Screenshot As"); 

    if( ::GetSaveFileName(&ofn) )
    {
        if( !m_Bitmap.Save(ofn.lpstrFile) )
            ::MessageBox( m_hWnd, _T("Cannot save file!"), _T("Error"), MB_OK | MB_ICONERROR);
    }
}
