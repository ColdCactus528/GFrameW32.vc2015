#include "StdAfx.h"
#include "GF.h"
#include "GFrameW32.h"

int gfGetWindowWidth()
{
    return theApp.GetFrameSize().x;
}

int gfGetWindowHeight()
{
    return theApp.GetFrameSize().y;
}

void gfSetWindowSize( int width, int height )
{
    theApp.SetFrameSize(width, height);
}

void gfSetPixel( int x, int y, RGBPIXEL color )
{
    theApp.SetPixel(x, y, color);
}

const RGBPIXEL gfGetPixel( int x,int y )
{
    return theApp.GetPixel(x, y);
}

void gfClearScreen( RGBPIXEL color )
{
    theApp.ClearFrame(color);
}

void gfDrawRectangle( int x0, int y0, int x1, int y1, RGBPIXEL color )
{
    theApp.DrawRectangle(x0, y0, x1, y1, color);
}

void gfDrawText( int x, int y, const TCHAR* str, RGBPIXEL color )
{
    theApp.DrawText( x, y, str, color );
}

RGBPIXEL* gfGetFrameBuffer()
{
    return theApp.GetFrameBuffer();
}


void gfDisplayMessage( const TCHAR* text )
{
    ::MessageBox(theApp.GetMainWnd(), text, theApp.GetTitle(), MB_OK);
}

unsigned int gfGetAppTime()
{
    return theApp.GetAppTime();
}

void gfCloseWindow()
{
    ::PostMessage(theApp.GetMainWnd(), WM_CLOSE, 0, 0);;
}

int gfGetMouseX()
{
    return theApp.GetMousePos().x;
}

int gfGetMouseY()
{
    return theApp.GetMousePos().y;
}
