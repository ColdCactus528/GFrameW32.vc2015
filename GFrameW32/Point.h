//-----------------------------------------------------------------------------
// File: Point.h
// Desc: Declaration and definition of 2D point template.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef POINT_H
#define POINT_H

template <class TYPE>
class Point
{
public:
    union
    {
        TYPE x, u;
    };
    union
    {
        TYPE y, v;
    };

    Point()
    {}

    Point( TYPE X, TYPE Y ) : 
    x(X), y(Y)
    {}

    Point( const Point<TYPE>& pt ) :
    x(pt.x), y(pt.y)
    {}

    template <class OTHER>
    explicit Point( const Point<OTHER>& pt ) :
    x((TYPE)pt.x), y((TYPE)pt.y)
    {}

    explicit Point( const POINT& pt ) :
    x( (TYPE)pt.x ), y( (TYPE)pt.y )
    {}

    // Element access
    TYPE operator[]( int index ) const                  { return (&x)[index]; }
    TYPE& operator[]( int index )                       { return (&x)[index]; }

    TYPE         LengthSquared() const                  { return (x * x + y * y); }
    TYPE         Length() const                         { return (TYPE)sqrt((double)LengthSquared()); }
    const Point<TYPE> Orth() const                      { return Point( -y, x ); }
    const Point<TYPE>& Normalize()                      { *this /= Length(); return (*this); }

    // Arithmetic operators
    Point<TYPE>&      operator=( const Point<TYPE>& pt ){ x = pt.x; y = pt.y; return (*this); }
    const Point<TYPE> operator+() const                 { return Point<TYPE>( *this ); }
    const Point<TYPE> operator-() const                 { return Point<TYPE>( -x, -y ); }
    Point<TYPE>&      operator+=( const Point<TYPE>& pt )   { x += pt.x; y += pt.y; return (*this); }
    Point<TYPE>&      operator-=( const Point<TYPE>& pt )   { x -= pt.x; y -= pt.y; return (*this); }
    Point<TYPE>&      operator*=( const TYPE& k )       { x *= k; y *= k; return (*this); }
    Point<TYPE>&      operator/=( const TYPE& k )       { x /= k; y /= k; return (*this); }

    // Comparison
    bool IsEqual( const Point<TYPE>& pt ) const
    {
        TYPE dx = x - pt.x,
             dy = y - pt.y;
        if( dx < (TYPE)0 ) dx = -dx;
        if( dy < (TYPE)0 ) dy = -dy;
        const TYPE epsilon = (TYPE)1 / 100000000;
        return (dx <= epsilon) && (dy <= epsilon);
    }

    Point<int> Round() const                            { return Point<int>( IntRound(x), IntRound(y) ); }

    enum ClassifyResult { BEYOND, BETWEEN, BEHIND, RIGHT, LEFT, ORG, DST };
    ClassifyResult Classify( const Point<TYPE>& org, const Point<TYPE>& dst ) const;
};

template <> inline
Point<int> Point<int>::Round() const
{
    return *this;
}

// Arithmetic operators
template <class TYPE> inline
const Point<TYPE> operator+( const Point<TYPE>& l, const Point<TYPE>& r )
{
    return Point<TYPE>( l.x + r.x, l.y + r.y );
}

template <class TYPE> inline
const Point<TYPE> operator-( const Point<TYPE>& l, const Point<TYPE>& r )
{
    return Point<TYPE>( l.x - r.x, l.y - r.y );
}

template <class TYPE> inline
TYPE operator*( const Point<TYPE>& l, const Point<TYPE>& r )
{
    return (l.x * r.x + l.y * r.y);
}

template <class TYPE> inline
const Point<TYPE> operator*( const Point<TYPE>& l, const TYPE& r )
{
    return Point<TYPE>( l.x * r, l.y * r );
}

template <class TYPE> inline
const Point<TYPE> operator*( const TYPE& l, const Point<TYPE>& r )
{
    return Point<TYPE>( l * r.x, l * r.y );
}

template <class TYPE> inline
const Point<TYPE> operator/( const Point<TYPE>& l, const TYPE& r )
{
    return Point<TYPE>( l.x / r, l.y / r );
}

template <class TYPE> inline
const Point<TYPE> operator/( const TYPE& l, const Point<TYPE>& r )
{
    return Point<TYPE>( l / r.x, l / r.y );
}

// Comparison
template <class TYPE> inline
bool operator==( const Point<TYPE>& l, const Point<TYPE>& r )
{ 
    return (l.x == r.x && l.y == r.y);
}

template <class TYPE> inline
bool operator!=( const Point<TYPE>& l, const Point<TYPE>& r )
{
    return !(l == r);
}

// Helper functions
template <class TYPE>
typename Point<TYPE>::ClassifyResult Point<TYPE>::Classify( const Point<TYPE>& org, const Point<TYPE>& dst ) const
{
    const TYPE epsilon = (TYPE)1 / 1000000;

    const Point<TYPE> a = dst - org,
                      b = (*this) - org;
    TYPE s = a.x * (-b.y) + a.y * b.x;
    if( Abs(s) < epsilon )    // kludge
    {
        if( a.x * b.x < (TYPE)0 || a.y * b.y < (TYPE)0 )
            return BEHIND;
        TYPE la = a.LengthSquared(),
             lb = b.LengthSquared();
        if( lb > la )
            return BEYOND;
        if( Abs(lb) < epsilon )
            return ORG;
        if( Abs(lb - la) < epsilon )
            return DST;
        return BETWEEN;
    }
    if( s > (TYPE)0 )
        return LEFT;
    else // s < 0
        return RIGHT;
}

inline
POINT& AsPOINT( Point<int>& pt )
{
    return *(POINT*)&pt;
}

typedef Point<int>      iPoint;
typedef Point<float>    fPoint;
typedef Point<double>   dPoint;

// Linear interpolator version with extended internal value range
inline
const iPoint Lerp( const iPoint& org, const iPoint& dst, int num, int denom )
{
    const iPoint d = dst - org;
    return org + iPoint( (int)(((__int64)d.x * num) / denom),
        (int)(((__int64)d.y * num) / denom) );
}

#ifndef NDEBUG
// Instantiate templates for validation purposes
template class Point<int>;
template class Point<float>;
template class Point<double>;
#endif

#endif  // POINT_H
