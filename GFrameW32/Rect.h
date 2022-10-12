//-----------------------------------------------------------------------------
// File: Rect.h
// Desc: Declaration and definition of rectangle template.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef RECT_H
#define RECT_H

#include "point.h"

template<class TYPE>
class Rect
{
public:
    union
    {
        TYPE x0, left;
    };
    union
    {
        TYPE y0, top;
    };
    union
    {
        TYPE x1, right;
    };
    union
    {
        TYPE y1, bottom;
    };

    // Constructors
    Rect()
    {}
    
    Rect( TYPE X0, TYPE Y0, TYPE X1, TYPE Y1 ) :
    x0(X0), y0(Y0), x1(X1), y1(Y1)
    {}
   
    Rect( const Rect<TYPE>& rc ) :
    x0(rc.x0), y0(rc.y0), x1(rc.x1), y1(rc.y1)
    {}

    Rect( const Point<TYPE>& leftTop, const Point<TYPE>& rightBottom ) :
    x0(leftTop.x), y0(leftTop.y), x1(rightBottom.x), y1(rightBottom.y)
    {}

    template<class OTHER>
    explicit Rect( const Rect<OTHER>& rc ) :
    x0((TYPE)rc.x0), y0((TYPE)rc.y0), x1((TYPE)rc.x1), y1((TYPE)rc.y1)
    {}

    explicit Rect( const RECT& rc ) :
    x0( (TYPE)rc.left ), y0( (TYPE)rc.top ), x1( (TYPE)rc.right ), y1( (TYPE)rc.bottom )
    {}

    // Element access
    TYPE operator[]( int index ) const                  { return (&x0)[index]; }
    TYPE& operator[]( int index )                       { return (&x0)[index]; }

    TYPE                Width() const                   { return (x1 - x0); }
    TYPE                Height() const                  { return (y1 - y0); }
    const Point<TYPE>   TopLeft() const                 { return Point<TYPE>(x0, y0); }
    const Point<TYPE>   RightBottom() const             { return Point<TYPE>(x1, y1); }
    const Point<TYPE>   Center() const                  { return Point<TYPE>((x0 + x1) / 2, (y0 + y1) / 2); }

    bool  IsEmpty() const                               { return ((x0 == x1) || (y0 == x1)); }
    bool  PtInRect( const Point<TYPE>& pt ) const       { return ((pt.x >= x0) && (pt.x < x1) && (pt.y >= y0) && (pt.y < y1)); }
    void  SetEmpty()                                    { x0 = y0 = x1 = y1 = (TYPE)0; }

    void  Normalize()                                   { if( x0 > x1 ) Swap(x0, x1); if( y0 > y1 ) Swap(y0, y1); }
    void  Inflate( TYPE l, TYPE t, TYPE r, TYPE b )     { x0 -= l; y0 -= t; x1 += r; y1 += b; }
    void  Inflate( TYPE dx, TYPE dy )                   { Inflate(dx, dy, dx, dy); }
    void  Inflate( TYPE d )                             { Inflate(d, d); }

    void  Deflate( TYPE l, TYPE t, TYPE r, TYPE b )     { x0 += l; y0 += t; x1 -= r; y1 -= b; }
    void  Deflate( TYPE dx, TYPE dy )                   { Deflate(dx, dy, dx, dy); }
    void  Deflate( TYPE d )                             { Deflate(d, d); }

    void  Offset( TYPE x, TYPE y )                      { x0 += x; y0 += y; x1 += x; y1 += y; }
    void  Offset( const Point<TYPE>& d )                { Offset(d.x, d.y); }
    void  MoveTo( TYPE x, TYPE y )                      { x1 += x - x0; y1 += y - y0; x0 = x; y0 = y; }
    void  MoveTo( const Point<TYPE>& pos )              { MoveTo(pos.x, pos.y); }

    Rect<TYPE>& operator=( const Rect<TYPE>& rc )       { x0 = rc.x0; y0 = rc.y0; x1 = rc.x1; y1 = rc.y1; return (*this); }

    static const Rect<TYPE> Intersection( const Rect<TYPE>& rc0, const Rect<TYPE>& rc1 )
    {
        Rect<TYPE> rc( Max(rc0.x0, rc1.x0), Max(rc0.y0, rc1.y0),
                       Min(rc0.x1, rc1.x1), Min(rc0.y1, rc1.y1) );
        if( rc.Width() <= 0 || rc.Height() <= 0 )
            rc.SetEmpty();
        return rc;
    }

    static const Rect<TYPE> Union( const Rect<TYPE>& rc0, const Rect<TYPE>& rc1 )
    {
        if( !rc0.IsEmpty() && !rc1.IsEmpty() )
        {
            return Rect<TYPE>( Min(rc0.x0, rc1.x0), Min(rc0.y0, rc1.y0),
                               Max(rc0.x1, rc1.x1), Max(rc0.y1, rc1.y1) );
        }
        else
        {
            if( !rc0.IsEmpty() )
                return rc0;
            else
            if( !rc1.IsEmpty() )
                return rc1;
            else
                return Rect<TYPE>((TYPE)0, (TYPE)0, (TYPE)0, (TYPE)0);
        }
    }
};

// Comparison operators
template <class TYPE>
bool operator==( const Rect<TYPE>& l, const Rect<TYPE>& r )
{
    return (l.x0 == r.x0 && l.y0 == r.y0 && l.x1 == r.x1 && l.y1 == r.y1);
}

template <class TYPE>
bool operator!=( const Rect<TYPE>& l, const Rect<TYPE>& r )
{
    return !(l == r);
}

inline
RECT& AsRECT( Rect<int>& rc )
{
    return *(RECT*)&rc;
}

inline
void MergePoint( Rect<int>& rc, const Point<int>& pt )
{
    if( !rc.IsEmpty() )
        rc = Rect<int>(pt.x, pt.y, pt.x + 1, pt.y + 1);
    else
    {
        if( rc.x0 > pt.x ) rc.x0 = pt.x;
        if( rc.y0 > pt.y ) rc.y0 = pt.y;
        if( rc.x1 <= pt.x ) rc.x1 = pt.x + 1;
        if( rc.y1 <= pt.y ) rc.y1 = pt.y + 1;
    }
}

typedef Rect<int>       iRect;
typedef Rect<float>     fRect;
typedef Rect<double>    dRect;

#ifndef NDEBUG
// Instantiate templates for validation purposes
template Rect<int>;
template Rect<float>;
template Rect<double>;
#endif

#endif  // RECT_H
