//-----------------------------------------------------------------------------
// File: Matrix.h
// Desc: Declaration and definition of 3D transformation matrix template.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

template<class TYPE>
class Matrix
{
public:
    TYPE m[4][4];

    Matrix()
    {}

    explicit Matrix( TYPE d )
    {
        Diagonal(d);
    }

    Matrix( const Matrix<TYPE>& mx )
    {
        memcpy( m, mx.m, sizeof(m) );
    }

    template<class OTHER>
    explicit Matrix( const Matrix<OTHER>& mx )
    {
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            for( int col = 0; col < ARRAYSIZE(m[row]); ++col)
                m[row][col] = (TYPE)mx[row][col];
        }
    }

    explicit Matrix( const TYPE* pm )
    {
        memcpy( m, pm, sizeof(m) );
    }

    Matrix( TYPE x00, TYPE x01, TYPE x02, TYPE x03,
            TYPE x10, TYPE x11, TYPE x12, TYPE x13,
            TYPE x20, TYPE x21, TYPE x22, TYPE x23,
            TYPE x30, TYPE x31, TYPE x32, TYPE x33 )
    {
        m[0][0] = x00; m[0][1] = x01; m[0][2] = x02; m[0][3] = x03;
        m[1][0] = x10; m[1][1] = x11; m[1][2] = x12; m[1][3] = x13;
        m[2][0] = x20; m[2][1] = x21; m[2][2] = x22; m[2][3] = x23;
        m[3][0] = x30; m[3][1] = x31; m[3][2] = x32; m[3][3] = x33;
    }

    TYPE*          operator[]( int row )                { return m[row]; }
    const TYPE*    operator[]( int row ) const          { return m[row]; }

    Matrix<TYPE>&  operator=( const Matrix<TYPE>& mx )  { memcpy( m, mx.m, sizeof(m) ); return (*this); }
    const Matrix<TYPE> operator+() const                { return Matrix<TYPE>(*this); }
    const Matrix<TYPE> operator-() const
    {
        Matrix<TYPE> res;
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            res[row][0] = -m[row][0];
            res[row][1] = -m[row][1];
            res[row][2] = -m[row][2];
            res[row][3] = -m[row][3];
        }
        return res;
    }

    Matrix<TYPE>& operator+=( const Matrix<TYPE>& mx )
    {
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            m[row][0] += mx.m[row][0];
            m[row][1] += mx.m[row][1];
            m[row][2] += mx.m[row][2];
            m[row][3] += mx.m[row][3];
        }
        return (*this);
    }

    Matrix<TYPE>& operator-=( const Matrix<TYPE>& mx )
    {
        for( int row = 0; row < ARRAYSIZE(m); ++row )
        {
            m[row][0] -= mx.m[row][0];
            m[row][1] -= mx.m[row][1];
            m[row][2] -= mx.m[row][2];
            m[row][3] -= mx.m[row][3];
        }
        return (*this);
    }

    Matrix<TYPE>& operator*=( const Matrix<TYPE>& mx )
    {
        *this = Matrix<TYPE>(*this) * mx;
        return (*this);
    }

    void Diagonal( TYPE d )
    {
        m[0][0] = d;       m[0][1] = (TYPE)0; m[0][2] = (TYPE)0; m[0][3] = (TYPE)0;
        m[1][0] = (TYPE)0; m[1][1] = d;       m[1][2] = (TYPE)0; m[1][3] = (TYPE)0;
        m[2][0] = (TYPE)0; m[2][1] = (TYPE)0; m[2][2] = d;       m[2][3] = (TYPE)0;
        m[3][0] = (TYPE)0; m[3][1] = (TYPE)0; m[3][2] = (TYPE)0; m[3][3] = d;
    }

    void Identity()
    {
        Diagonal( (TYPE)1 );
    }

    void InversTransfrom()
    {
        Matrix<TYPE> mx(1.0f);
    
        for( int i = 0; i < ARRAYSIZE(m); ++i )
        {
            TYPE d = m[i][i];
            if( d != (TYPE)1 )
            {
                mx.m[i][0] /= d;
                mx.m[i][1] /= d;
                mx.m[i][2] /= d;
                mx.m[i][3] /= d;
                m[i][0] /= d;
                m[i][1] /= d;
                m[i][2] /= d;
                m[i][3] /= d;
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
                        m[j][3] -= mul * m[i][3];
                        mx.m[j][0] -= mul * mx.m[i][0];
                        mx.m[j][1] -= mul * mx.m[i][1];
                        mx.m[j][2] -= mul * mx.m[i][2];
                        mx.m[j][3] -= mul * mx.m[i][3];
                    }
                }
            }
        }
        *this = mx;
    }

    void Transpose()
    {
        Swap( m[0][1], m[1][0] ); Swap( m[0][2], m[2][0] ); Swap( m[0][3], m[3][0] );
                                  Swap( m[1][2], m[2][1] ); Swap( m[1][3], m[3][1] );
                                                            Swap( m[2][3], m[3][2] );
    }

    const Vector<TYPE> TransformAffine( const Vector<TYPE>& v ) const
    {
        return Vector<TYPE>( v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
                             v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
                             v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2] );
    }

    const Vector4<TYPE> Transform( const Vector<TYPE>& v ) const
    {
        Vector<TYPE> vec = TransformAffine(v);
        return Vector4<TYPE>(vec.x, vec.y, vec.z, v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3]);
    }

    const Vector<TYPE> TransformNormal( const Vector<TYPE>& v ) const
    {
        Vector<TYPE> res = TransformAffine(v);
        return res.Normalize();
    }

    // Helpers
    static const Matrix<TYPE> ViewTransform( const Vector<TYPE>& pos, const Vector<TYPE>& dir, const Vector<TYPE>& up );
    static const Matrix<TYPE> ProjectionTransform( TYPE width, TYPE height, TYPE near_z, TYPE far_z );
    static const Matrix<TYPE> RotationTransform( const Vector<TYPE>& v, TYPE angle );
    static const Matrix<TYPE> RotationXTransform( TYPE angle );
    static const Matrix<TYPE> RotationYTransform( TYPE angle );
    static const Matrix<TYPE> RotationZTransform( TYPE angle );
    static const Matrix<TYPE> MovementTransform( const Vector<TYPE>& v );
    static const Matrix<TYPE> ScalingTransform( const Vector<TYPE>& v );
};

template <class TYPE>
const Matrix<TYPE> operator+( const Matrix<TYPE>& l, const Matrix<TYPE>& r )
{
    Matrix<TYPE> res;
    for( int row = 0; row < ARRAYSIZE(m); ++row )
    {
        res[row][0] = l.m[row][0] + r.m[row][0];
        res[row][1] = l.m[row][1] + r.m[row][1];
        res[row][2] = l.m[row][2] + r.m[row][2];
        res[row][3] = l.m[row][3] + r.m[row][3];
    }
    return res;
}

template <class TYPE>
const Matrix<TYPE> operator-( const Matrix<TYPE>& l, const Matrix<TYPE>& r )
{
    Matrix<TYPE> res;
    for( int row = 0; row < ARRAYSIZE(m); ++row )
    {
        res[row][0] = l.m[row][0] - r.m[row][0];
        res[row][1] = l.m[row][1] - r.m[row][1];
        res[row][2] = l.m[row][2] - r.m[row][2];
        res[row][3] = l.m[row][3] - r.m[row][3];
    }
    return res;
}

template <class TYPE>
const Matrix<TYPE> operator*( const Matrix<TYPE>& l, const Matrix<TYPE>& r )
{
    return Matrix<TYPE>(
        l.m[0][0] * r.m[0][0] + l.m[0][1] * r.m[1][0] + l.m[0][2] * r.m[2][0] + l.m[0][3] * r.m[3][0],
        l.m[0][0] * r.m[0][1] + l.m[0][1] * r.m[1][1] + l.m[0][2] * r.m[2][1] + l.m[0][3] * r.m[3][1],
        l.m[0][0] * r.m[0][2] + l.m[0][1] * r.m[1][2] + l.m[0][2] * r.m[2][2] + l.m[0][3] * r.m[3][2],
        l.m[0][0] * r.m[0][3] + l.m[0][1] * r.m[1][3] + l.m[0][2] * r.m[2][3] + l.m[0][3] * r.m[3][3],
        l.m[1][0] * r.m[0][0] + l.m[1][1] * r.m[1][0] + l.m[1][2] * r.m[2][0] + l.m[1][3] * r.m[3][0],
        l.m[1][0] * r.m[0][1] + l.m[1][1] * r.m[1][1] + l.m[1][2] * r.m[2][1] + l.m[1][3] * r.m[3][1],
        l.m[1][0] * r.m[0][2] + l.m[1][1] * r.m[1][2] + l.m[1][2] * r.m[2][2] + l.m[1][3] * r.m[3][2],
        l.m[1][0] * r.m[0][3] + l.m[1][1] * r.m[1][3] + l.m[1][2] * r.m[2][3] + l.m[1][3] * r.m[3][3],
        l.m[2][0] * r.m[0][0] + l.m[2][1] * r.m[1][0] + l.m[2][2] * r.m[2][0] + l.m[2][3] * r.m[3][0],
        l.m[2][0] * r.m[0][1] + l.m[2][1] * r.m[1][1] + l.m[2][2] * r.m[2][1] + l.m[2][3] * r.m[3][1],
        l.m[2][0] * r.m[0][2] + l.m[2][1] * r.m[1][2] + l.m[2][2] * r.m[2][2] + l.m[2][3] * r.m[3][2],
        l.m[2][0] * r.m[0][3] + l.m[2][1] * r.m[1][3] + l.m[2][2] * r.m[2][3] + l.m[2][3] * r.m[3][3],
        l.m[3][0] * r.m[0][0] + l.m[3][1] * r.m[1][0] + l.m[3][2] * r.m[2][0] + l.m[3][3] * r.m[3][0],
        l.m[3][0] * r.m[0][1] + l.m[3][1] * r.m[1][1] + l.m[3][2] * r.m[2][1] + l.m[3][3] * r.m[3][1],
        l.m[3][0] * r.m[0][2] + l.m[3][1] * r.m[1][2] + l.m[3][2] * r.m[2][2] + l.m[3][3] * r.m[3][2],
        l.m[3][0] * r.m[0][3] + l.m[3][1] * r.m[1][3] + l.m[3][2] * r.m[2][3] + l.m[3][3] * r.m[3][3] );
}

template <class TYPE>
bool operator==( const Matrix<TYPE>& l, const Matrix<TYPE>& r )
{
    return (memcmp( l.m, r.m, sizeof(l.m) ) == 0);
}

template <class TYPE>
bool operator!=( const Matrix<TYPE>& l, const Matrix<TYPE>& r )
{
    return !(l == r);
}

//-----------------------------------------------------------------------------
// Name: ViewTransform
// Desc: Builds matrix of view transformation, described by position, direction
//       of view and up vector.
//-----------------------------------------------------------------------------
template<class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::ViewTransform( const Vector<TYPE>& pos, const Vector<TYPE>& dir, const Vector<TYPE>& up )
{
    Vector<TYPE> Dir = dir.Dir(),          // Normalize the direction vector
                 Right = (up ^ Dir).Dir(), // Create right vector as normal to plane defined by Dir and Up
                 Up = (Dir ^ Right).Dir(); // Create Up vector as normal to Dir-Right plane

    return Matrix<TYPE>( Right.x,      Up.x,      Dir.x,     (TYPE)0,
                         Right.y,      Up.y,      Dir.y,     (TYPE)0,
                         Right.z,      Up.z,      Dir.z,     (TYPE)0,
                        -Right * pos, -Up * pos, -Dir * pos, (TYPE)1 );
}

//-----------------------------------------------------------------------------
// Name: ProjectionTransform
// Desc: Builds matrix of projection transformation, using given viwport info.
//-----------------------------------------------------------------------------
template<class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::ProjectionTransform( TYPE width, TYPE height, TYPE near_z, TYPE far_z )
{
    Matrix<TYPE> mx( (TYPE)0 );
    TYPE Q = far_z / (far_z - near_z);
   
    mx[0][0] = (TYPE)width;
    mx[1][1] = (TYPE)height;
    mx[2][2] = Q;
    mx[2][3] = (TYPE)1;
    mx[3][2] = -Q * near_z;
    //mx[3][3] = (TYPE)1;*/

    return mx;
}

//-----------------------------------------------------------------------------
// Name: RotationTransform
// Desc: Builds matrix of rotation transformation around given axis by given
//       angle (angle in radians).
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::RotationTransform( const Vector<TYPE>& v, TYPE angle )
{
    Matrix<TYPE> mx((TYPE)1);
    TYPE x = v.x,
         y = v.y,
         z = v.z,
         Cos = (TYPE)cos(angle),
         Sin = (TYPE)sin(angle);

    mx[0][0] = x * x + ( (TYPE)1 - x * x ) * Cos;
    mx[0][1] = x * y * ( (TYPE)1 - Cos ) + z * Sin;
    mx[0][2] = z * x * ( (TYPE)1 - Cos ) - y * Sin;

    mx[1][0] = x * y * ( (TYPE)1 - Cos ) - z * Sin;
    mx[1][1] = y * y + ( (TYPE)1 - y * y ) * Cos;
    mx[1][2] = y * z * ( (TYPE)1 - Cos ) + x * Sin;
    
    mx[2][0] = z * x * ( (TYPE)1 - Cos ) + y * Sin;
    mx[2][1] = y * z * ( (TYPE)1 - Cos ) - x * Sin;
    mx[2][2] = z * z + ( (TYPE)1 - z * z ) * Cos;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: RotationXTransform
// Desc: Builds matrix of rotation transformation around X-axis by given
//       angle (angle in radians).
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::RotationXTransform( TYPE angle )
{
    Matrix<TYPE> mx((TYPE)1);
    TYPE Cos = (TYPE)cos(angle),
         Sin = (TYPE)sin(angle);

    mx[1][1] = Cos;  mx[1][2] = Sin;
    mx[2][1] = -Sin; mx[2][2] = Cos;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: RotationYTransform
// Desc: Builds matrix of rotation transformation around Y-axis by given
//       angle (angle in radians).
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::RotationYTransform( TYPE angle )
{
    Matrix<TYPE> mx((TYPE)1);
    TYPE Cos = (TYPE)cos(angle),
         Sin = (TYPE)sin(angle);

    mx[0][0] = Cos;  mx[0][2] = Sin;
    mx[2][0] = -Sin; mx[2][2] = Cos;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: RotationZTransform
// Desc: Builds matrix of rotation transformation around Z-axis by given
//       angle (angle in radians).
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::RotationZTransform( TYPE angle )
{
    Matrix<TYPE> mx((TYPE)1);
    TYPE Cos = (TYPE)cos(angle),
         Sin = (TYPE)sin(angle);

    mx[0][0] = Cos;  mx[0][1] = Sin;
    mx[1][0] = -Sin; mx[1][1] = Cos;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: MovementTransform
// Desc: Builds matrix of movement transformation along given vector.
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::MovementTransform( const Vector<TYPE>& v )
{
    Matrix<TYPE> mx( (TYPE)1 );

    mx[3][0] = v.x;
    mx[3][1] = v.y;
    mx[3][2] = v.z;

    return mx;
}

//-----------------------------------------------------------------------------
// Name: ScalingTransform
// Desc: Builds matrix of scaling transformation. Each component of parameter
//       vector describes scaling coefficient along corresponding axis.
//-----------------------------------------------------------------------------
template <class TYPE> inline
const Matrix<TYPE> Matrix<TYPE>::ScalingTransform( const Vector<TYPE>& v )
{
    Matrix<TYPE> mx( (TYPE)1 );
    
    mx[0][0] = v.x;
    mx[1][1] = v.y;
    mx[2][2] = v.z;
    
    return mx;
}

typedef Matrix<float>    fMatrix;
typedef Matrix<double>   dMatrix;

#ifndef NDEBUG
// Instantiate templates for validation purposes
template class Matrix<float>;
template class Matrix<double>;
#endif

#endif  //  MATRIX_H
