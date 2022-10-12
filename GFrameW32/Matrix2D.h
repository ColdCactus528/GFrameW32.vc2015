//-----------------------------------------------------------------------------
// File: Matrix2D.h
// Desc: Declaration and definition of 2D transformation matrix template.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "Point.h"

template<class TYPE>
class Matrix2D
{
public:
    TYPE m[3][3];

    Matrix2D()
    {}

    explicit Matrix2D( TYPE d )
    {
        Diagonal(d);
    }

    Matrix2D( const Matrix2D<TYPE>& mx )
    {
        memcpy( m, mx.m, sizeof(m) );
    }

    explicit Matrix2D( const TYPE* pm )
    {
        memcpy( m, pm, sizeof(m) );
    }

    template<class OTHER>
    explicit Matrix2D( const Matrix2D<OTHER>& mx )
    {
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            for( int col = 0; col < ARRAYSIZE(m[row]); ++col)
                m[row][col] = (TYPE)mx[row][col];
        }
    }

    Matrix2D( TYPE x00, TYPE x01, TYPE x02,
              TYPE x10, TYPE x11, TYPE x12,
              TYPE x20, TYPE x21, TYPE x22 )
    {
        m[0][0] = x00; m[0][1] = x01; m[0][2] = x02;
        m[1][0] = x10; m[1][1] = x11; m[1][2] = x12;
        m[2][0] = x20; m[2][1] = x21; m[2][2] = x22;
    }

    TYPE*       operator[]( int row )                       { return m[row]; }
    const TYPE* operator[]( int row ) const                 { return m[row]; }

    Matrix2D<TYPE>&  operator=( const Matrix2D<TYPE>& mx )  { memcpy( m, mx.m, sizeof(m) ); return (*this); }
    const Matrix2D<TYPE> operator+() const                  { return Matrix2D<TYPE>(*this); }
    const Matrix2D<TYPE> operator-() const
    {
        Matrix2D<TYPE> res;
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            res[row][0] = -m[row][0];
            res[row][1] = -m[row][1];
            res[row][2] = -m[row][2];
        }
        return res;
    }

    Matrix2D<TYPE>& operator+=( const Matrix2D<TYPE>& mx )
    {
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            m[row][0] += mx.m[row][0];
            m[row][1] += mx.m[row][1];
            m[row][2] += mx.m[row][2];
        }
        return (*this);
    }

    Matrix2D<TYPE>& operator-=( const Matrix2D<TYPE>& mx )
    {
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            m[row][0] -= mx.m[row][0];
            m[row][1] -= mx.m[row][1];
            m[row][2] -= mx.m[row][2];
        }
        return (*this);
    }

    Matrix2D<TYPE>& operator*=( const Matrix2D<TYPE>& mx )
    {
        *this = Matrix2D<TYPE>(*this) * mx;
        return (*this);
    }

    void Diagonal( TYPE d )
    {
        m[0][0] = d;       m[0][1] = (TYPE)0; m[0][2] = (TYPE)0;
        m[1][0] = (TYPE)0; m[1][1] = d;       m[1][2] = (TYPE)0;
        m[2][0] = (TYPE)0; m[2][1] = (TYPE)0; m[2][2] = d;
    }

    void Identity()
    {
        Diagonal( (TYPE)1 );
    }

    void InversTransfrom()
    {
        Matrix2D<TYPE> mx( (TYPE)1 );
    
        for( int i = 0; i < ARRAYSIZE(m); ++i )
        {
            TYPE d = m[i][i];
            if( d != (TYPE)1 )
            {
                mx.m[i][0] /= d;
                mx.m[i][1] /= d;
                mx.m[i][2] /= d;
                m[i][0] /= d;
                m[i][1] /= d;
                m[i][2] /= d;
            }

            for( int j = 0; j < ARRAYSIZE(m); ++j )
            {
                if( j != i )
                {
                    if( m[j][i] != 0.0 )
                    {
                        TYPE mul = m[j][i];
                        m[j][0] -= mul * m[i][0];
                        m[j][1] -= mul * m[i][1];
                        m[j][2] -= mul * m[i][2];
                        mx.m[j][0] -= mul * mx.m[i][0];
                        mx.m[j][1] -= mul * mx.m[i][1];
                        mx.m[j][2] -= mul * mx.m[i][2];
                    }
                }
            }
        }
        *this = mx;
    }

    void Transpose()
    {
        Swap( m[0][1], m[1][0] ); Swap( m[0][2], m[2][0] );
                                  Swap( m[1][2], m[2][1] );
    }

    const Point<TYPE> Transform( const Point<TYPE>& v ) const
    {
        return Point<TYPE>( v.x * m[0][0] + v.y * m[1][0] + m[2][0],
                            v.x * m[0][1] + v.y * m[1][1] + m[2][1] );
    }

    const Point<TYPE> TransformNormal( const Point<TYPE>& v ) const
    {
        Point<TYPE> res = Transform(v);
        return res.Normalize();
    }

    // Helpers
    static const Matrix2D<TYPE> RotationTransform( const Point<TYPE>& v, TYPE angle );
    static const Matrix2D<TYPE> RotationTransform( TYPE angle );
    static const Matrix2D<TYPE> TranslationTransform( const Point<TYPE>& v );
    static const Matrix2D<TYPE> ScalingTransform( const Point<TYPE>& v );
};

template <class TYPE>
const Matrix2D<TYPE> operator+( const Matrix2D<TYPE>& l, const Matrix2D<TYPE>& r )
{
    Matrix2D<TYPE> res;
    for( int row = 0; row < ARRAYSIZE(m); ++row )
    {
        res[row][0] = l.m[row][0] + r.m[row][0];
        res[row][1] = l.m[row][1] + r.m[row][1];
        res[row][2] = l.m[row][2] + r.m[row][2];
    }
    return res;
}

template <class TYPE>
const Matrix2D<TYPE> operator-( const Matrix2D<TYPE>& l, const Matrix2D<TYPE>& r )
{
    Matrix2D<TYPE> res;
    for( int row = 0; row < ARRAYSIZE(m); ++row )
    {
        res[row][0] = l.m[row][0] - r.m[row][0];
        res[row][1] = l.m[row][1] - r.m[row][1];
        res[row][2] = l.m[row][2] - r.m[row][2];
    }
    return res;
}

template <class TYPE>
const Matrix2D<TYPE> operator*( const Matrix2D<TYPE>& l, const Matrix2D<TYPE>& r )
{
    return Matrix2D<TYPE>(
        l.m[0][0] * r.m[0][0] + l.m[0][1] * r.m[1][0] + l.m[0][2] * r.m[2][0],
        l.m[0][0] * r.m[0][1] + l.m[0][1] * r.m[1][1] + l.m[0][2] * r.m[2][1],
        l.m[0][0] * r.m[0][2] + l.m[0][1] * r.m[1][2] + l.m[0][2] * r.m[2][2],
        l.m[1][0] * r.m[0][0] + l.m[1][1] * r.m[1][0] + l.m[1][2] * r.m[2][0],
        l.m[1][0] * r.m[0][1] + l.m[1][1] * r.m[1][1] + l.m[1][2] * r.m[2][1],
        l.m[1][0] * r.m[0][2] + l.m[1][1] * r.m[1][2] + l.m[1][2] * r.m[2][2],
        l.m[2][0] * r.m[0][0] + l.m[2][1] * r.m[1][0] + l.m[2][2] * r.m[2][0],
        l.m[2][0] * r.m[0][1] + l.m[2][1] * r.m[1][1] + l.m[2][2] * r.m[2][1],
        l.m[2][0] * r.m[0][2] + l.m[2][1] * r.m[1][2] + l.m[2][2] * r.m[2][2] );
}

template <class TYPE>
bool operator==( const Matrix2D<TYPE>& l, const Matrix2D<TYPE>& r )
{
    return (memcmp( l.m, r.m, sizeof(l.m) ) == 0);
}

template <class TYPE>
bool operator!=( const Matrix2D<TYPE>& l, const Matrix2D<TYPE>& r )
{
    return !(l == mm1);
}

//-----------------------------------------------------------------------------
// Name: RotationTransform
// Desc: Builds matrix of rotation transformation around given point by given
//       angle (angle in radians).
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix2D<TYPE> Matrix2D<TYPE>::RotationTransform( const Point<TYPE>& v, TYPE angle )
{
    Matrix2D<TYPE> mx((TYPE)1);
    TYPE x = v.x,
         y = v.y,
         c = (TYPE)cos(angle),
         s = (TYPE)sin(angle);

    mx[0][0] = c;                   mx[0][1] = s;
    mx[1][0] = -s;                  mx[1][1] = c;
    mx[2][0] = -x * c + y * s + x;  mx[2][1] = -x * s - y * c + y;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: RotationTransform
// Desc: Builds matrix of rotation transformation around coordinate system origin
//       given angle (angle in radians).
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix2D<TYPE> Matrix2D<TYPE>::RotationTransform( TYPE angle )
{
    Matrix2D<TYPE> mx((TYPE)1);
    TYPE c = (TYPE)cos(angle),
         s = (TYPE)sin(angle);

    mx[0][0] = c;  mx[0][1] = s;
    mx[1][0] = -s; mx[1][1] = c;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: MovementTransform
// Desc: Builds matrix of translation transformation along given vector.
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix2D<TYPE> Matrix2D<TYPE>::TranslationTransform( const Point<TYPE>& v )
{
    Matrix2D<TYPE> mx( (TYPE)1 );

    mx[2][0] = v.x;
    mx[2][1] = v.y;

    return mx;
}


//-----------------------------------------------------------------------------
// Name: ScalingTransform
// Desc: Builds matrix of scaling transformation. Each component of parameter
//       vector describes scaling coefficient along corresponding axis.
//-----------------------------------------------------------------------------
template <class TYPE> inline 
const Matrix2D<TYPE> Matrix2D<TYPE>::ScalingTransform( const Point<TYPE>& v )
{
    Matrix2D<TYPE> mx( (TYPE)1 );
    
    mx[0][0] = v.x;
    mx[1][1] = v.y;
    
    return mx;
}

typedef Matrix2D<float>    fMatrix2D;
typedef Matrix2D<double>   dMatrix2D;

#ifndef NDEBUG
// Instantiate templates for validation purposes
template class Matrix2D<float>;
template class Matrix2D<double>;
#endif

#endif  //  MATRIX2D_H
