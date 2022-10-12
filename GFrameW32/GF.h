#ifndef GF_H
#define GF_H

// Returns app window width, in pixels
extern int  gfGetWindowWidth();

// Returns app window height, in pixels
extern int  gfGetWindowHeight();

// Adjusts app widnow width and height. The dimesions cannot be larger than desktop size
extern void gfSetWindowSize( int width, int height );


// Draws single pixel with given color
extern void gfSetPixel( int x, int y, RGBPIXEL color );

// Returns color of screen pixel
extern const RGBPIXEL gfGetPixel( int x,int y );

// Fills entire screen with given color
extern void gfClearScreen( RGBPIXEL color );

// Draws rectangle with given color
extern void gfDrawRectangle( int x0, int y0, int x1, int y1, RGBPIXEL color );

// Draws text string
extern void gfDrawText( int x, int y, const TCHAR* str, RGBPIXEL color );

// Returns pointer to screen memory buffer;
// buffer size is WindowWidth * WindowHeight
extern RGBPIXEL* gfGetFrameBuffer();


// Displays message box with given message and 'OK' button
extern void gfDisplayMessage( const TCHAR* text );

// Returns number of milliseconds passed since app start
extern unsigned int gfGetAppTime();

// Closes app window and exits app
extern void gfCloseWindow();


// Returns X position of mouse cursor
extern int  gfGetMouseX();

// Returns Y position of mouse cursor
extern int  gfGetMouseY();

#endif  // GF_H
