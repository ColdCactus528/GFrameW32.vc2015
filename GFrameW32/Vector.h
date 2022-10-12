//-----------------------------------------------------------------------------
// File: Vector.h
// Desc: Declaration and definition of 3D vector templates.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef VECTOR_H
#define VECTOR_H

template<class TYPE>
class Vector
{
public:
    TYPE x;
    TYPE y;
    TYPE z;

    Vector()
    {}

    Vector(TYPE X, TYPE Y, TYPE Z) :
    x(X), y(Y), z(Z)
    {}

    Vector(const Vector<TYPE>& v) :
    x(v.x), y(v.y), z(v.z)
    {}

    template<class OTHER>
    explicit Vector( const Vector<OTHER>& v ) :
    x((TYPE)v.x), y((TYPE)v.y), z((TYPE)v.z)
    {}

    TYPE&      operator[](int i)                        { return (&x)[i]; }
    TYPE       operator[](int i) const                  { return (&x)[i]; }

    TYPE               LengthSquared() const            { return (x*x + y*y + z*z); }
    TYPE               Length() const                   { return (TYPE)sqrt((double)LengthSquared()); }
    Vector<TYPE>&      Normalize()                      { *this /= Length(); return (*this); }
    const Vector<TYPE> Dir() const                      { return Vector<TYPE>(*this).Normalize(); }

    Vector<TYPE>&      operator=(const Vector<TYPE>& v) { x = v.x; y = v.y; z = v.z; return (*this); }
    const Vector<TYPE> operator+() const                { return Vector<TYPE>(*this); }
    const Vector<TYPE> operator-() const                { return Vector<TYPE>(-x, -y, -z); }
    Vector<TYPE>&      operator+=(const Vector<TYPE>& v){ x += v.x; y += v.y; z += v.z; return (*this); }
    Vector<TYPE>&      operator-=(const Vector<TYPE>& v){ x -= v.x; y -= v.y; z -= v.z; return (*this); }
    Vector<TYPE>&      operator*=(const TYPE& k)        { x *= k; y *= k; z *= k; return (*this); }
    Vector<TYPE>&      operator/=(const TYPE& k)        { x /= k; y /= k; z /= k; return (*this); }
    Vector<TYPE>&      operator^=(const Vector<TYPE>& b){ const Vector<TYPE> a(*this);
                                                          x = a.y * b.z - a.z * b.y;
                                                          y = a.z * b.x - a.x * b.z;
                                                          z = a.x * b.y - a.y * b.x;
                                                          return (*this); }

    bool IsEqual( const Vector<TYPE>& v ) const
    {
        const TYPE epsilon = (TYPE)1 / 100000000;
        return ((*this) - v).LengthSquared() <= epsilon;
    }
};

// Arithmetics operators
template<class TYPE>
const Vector<TYPE> operator+(const Vector<TYPE>& l, const Vector<TYPE>& r)
{
    return Vector<TYPE>(l.x + r.x, l.y + r.y, l.z + r.z);
}

template<class TYPE>
const Vector<TYPE> operator-(const Vector<TYPE>& l, const Vector<TYPE>& r)
{
    return Vector<TYPE>(l.x - r.x, l.y - r.y, l.z - r.z);
}

template<class TYPE>
TYPE operator*(const Vector<TYPE>& l, const Vector<TYPE>& r)
{
    return (l.x*r.x + l.y*r.y + l.z*r.z);
}

template<class TYPE>
const Vector<TYPE> operator*(const Vector<TYPE>& l, const TYPE& r)
{
    return Vector<TYPE>(l.x * r, l.y * r, l.z * r);
}

template<class TYPE>
const Vector<TYPE> operator*(const TYPE& l, const Vector<TYPE>& r)
{
    return Vector<TYPE>(l * r.x, l * r.y, l * r.z);
}

template<class TYPE>
const Vector<TYPE> operator/(const Vector<TYPE>& l, const TYPE& r)
{
    return Vector<TYPE>(l.x / r, l.y / r, l.z / r);
}

template<class TYPE>
const Vector<TYPE> operator^(const Vector<TYPE>& l, const Vector<TYPE>& r)
{
    return Vector<TYPE>(l.y * r.z - l.z * r.y, 
                        l.z * r.x - l.x * r.z,
                        l.x * r.y - l.y * r.x);
}

// Comparison operators
template<class TYPE>
bool operator==(const Vector<TYPE>& l, const Vector<TYPE>& r)
{
    return (l.x == r.x && l.y == r.y && l.z == r.z);
}

template<class TYPE>
bool operator!=(const Vector<TYPE>& l, const Vector<TYPE>& r)
{
    return !(l == r);
}


typedef Vector<int>     iVector;
typedef Vector<float>   fVector;
typedef Vector<double>  dVector;

#ifndef NDEBUG
// Instantiate templates for validation purposes
template class Vector<int>;
template class Vector<float>;
template class Vector<double>;
#endif

// Class for homogeneous vector
template<class TYPE>
class Vector4
{
public:
    TYPE x;
    TYPE y;
    TYPE z;
    TYPE w;

    Vector4()
    {}

    Vector4(TYPE X, TYPE Y, TYPE Z, TYPE W) :
    x(X), y(Y), z(Z), w(W)
    {}

    Vector4(const Vector4<TYPE>& v) :
    x(v.x), y(v.y), z(v.z), w(v.w)
    {}

    template<class OTHER>
    explicit Vector4( const Vector4<OTHER>& v ) :
    x((TYPE)v.x), y((TYPE)v.y), z((TYPE)v.z), w((TYPE)v.w)
    {}

    ~Vector4()
    {}

    TYPE&          operator[](int i)                    { return (&x)[i]; }
    TYPE           operator[](int i) const              { return (&x)[i]; }

    Vector<TYPE>   NormalizeHom() const
    {
        if( w != (TYPE)0 )
        {
            if( w != (TYPE)1 )
                return Vector<TYPE>(x / w, y / w, z / w);
            else
                return Vector<TYPE>(x, y, z);
        }
        else
        {
            Vector<TYPE> v( (TYPE)0, (TYPE)0, (TYPE)0 );
            if( x > (TYPE)0 ) v.x = Traits<TYPE>::PosInfinity();
            else
            if( x < (TYPE)0 ) v.x = Traits<TYPE>::NegInfinity();
            
            if( y > (TYPE)0 ) v.y = Traits<TYPE>::PosInfinity();
            else
            if( y < (TYPE)0 ) v.y = Traits<TYPE>::NegInfinity();

            if( z > (TYPE)0 ) v.z = Traits<TYPE>::PosInfinity();
            else
            if( z < (TYPE)0 ) v.z = Traits<TYPE>::NegInfinity();

            return v;
        }
    }

    Vector4<TYPE>&  operator=(const Vector4<TYPE>& v)   { x = v.x; y = v.y; z = v.z; w = v.w; return (*this); }

    template<class TYPE>
    struct Traits
    {
        static TYPE PosInfinity();
        static TYPE NegInfinity();
    };

    template<> struct Traits<float>
    {
        static float PosInfinity()      { return FLT_MAX; }
        static float NegInfinity()      { return -FLT_MAX; }
    };

    template<> struct Traits<double>
    {
        static double PosInfinity()     { return DBL_MAX; }
        static double NegInfinity()     { return -DBL_MAX; }
    };
};

typedef Vector4<float>  fVector4;
typedef Vector4<double> dVector4;

#ifndef NDEBUG
// Instantiate templates for validation purposes
//template class Vector4<float>;
//template class Vector4<double>;
#endif

#endif VECTOR_H
