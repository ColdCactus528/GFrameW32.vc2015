// Copyright (c) 2008  Konstantin Kryzhanovsky

#ifndef RGBPIXEL_H
#define RGBPIXEL_H

class RGBPIXEL
{
public:
    BYTE blue;
    BYTE green;
    BYTE red;
    BYTE reserved;

    RGBPIXEL()
    {}

    RGBPIXEL( BYTE r, BYTE g, BYTE b ) : 
    blue(b), green(g), red(r), reserved(0)
    {}

    RGBPIXEL( const RGBPIXEL& c ) :
    blue(c.blue), green(c.green), red(c.red), reserved(c.reserved)
    {}

    RGBPIXEL& operator=( const RGBPIXEL& c )
    {
        blue = c.blue; green = c.green; red = c.red;
        return (*this);
    }

    void Set( BYTE r, BYTE g, BYTE b )
    {
        blue = b; green = g; red = r;
    }

    static const RGBPIXEL Black()       { return RGBPIXEL(  0,   0,   0); }
    static const RGBPIXEL White()       { return RGBPIXEL(255, 255, 255); }
    static const RGBPIXEL DkGray()      { return RGBPIXEL(128, 128, 128); }
    static const RGBPIXEL Gray()        { return RGBPIXEL(192, 192, 192); }
    static const RGBPIXEL DkRed()       { return RGBPIXEL(128,   0,   0); }
    static const RGBPIXEL Red()         { return RGBPIXEL(255,   0,   0); }
    static const RGBPIXEL DkGreen()     { return RGBPIXEL(  0, 128,   0); }
    static const RGBPIXEL Green()       { return RGBPIXEL(  0, 255,   0); }
    static const RGBPIXEL DkBlue()      { return RGBPIXEL(  0,   0, 128); }
    static const RGBPIXEL Blue()        { return RGBPIXEL(  0,   0, 255); }
    static const RGBPIXEL DkYellow()    { return RGBPIXEL(128, 128,   0); }
    static const RGBPIXEL Yellow()      { return RGBPIXEL(255, 255,   0); }
    static const RGBPIXEL DkCyan()      { return RGBPIXEL(  0, 128, 128); }
    static const RGBPIXEL Cyan()        { return RGBPIXEL(  0, 255, 255); }
    static const RGBPIXEL DkMagenta()   { return RGBPIXEL(128,   0, 128); }
    static const RGBPIXEL Magenta()     { return RGBPIXEL(255,   0, 255); }

    static const RGBPIXEL RandomColor() { return RGBPIXEL( BYTE(rand() % 256), BYTE(rand() % 256), BYTE(rand() % 256) ); }
};

// Comparison
inline
bool operator==( const RGBPIXEL& l, const RGBPIXEL& r )
{
    return ( *(const DWORD*)&l == *(const DWORD*)&r );
}

inline
bool operator!=( const RGBPIXEL& l, const RGBPIXEL& r )
{
    return !(l == r);
}

#endif //   RGBPIXEL_H

