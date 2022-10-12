//-----------------------------------------------------------------------------
// File: Box.h
// Desc: Declaration and definition of 3D bounding box template.
//
// Copyright (c) 2010 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef BOX_H
#define BOX_H

template<class TYPE>
class Box
{
public:
    Vector<TYPE> vmin;
    Vector<TYPE> vmax;

    Box()
    {}

    Box( TYPE x0, TYPE y0, TYPE z0, TYPE x1, TYPE y1, TYPE z1 ) :
    vmin(x0, y0, z0), vmax(x1, y1, z1)
    {}

    Box( const Box<TYPE>& b ) :
    vmin(b.vmin), vmax(b.vmax)
    {}

    template <class OTHER>
    explicit Box( const Box<OTHER>& b ) :
    vmin(Vector<TYPE>(b.vmin)), vmax(Vector<TYPE>(b.vmax))
    {}

    void Reset()                    { vmin = vmax = Vector<TYPE>( (TYPE)0, (TYPE)0, (TYPE)0 ); }

    void Inflate( TYPE d )
    {
        vmin -= Vector<TYPE>(d, d, d);
        vmax += Vector<TYPE>(d, d, d);
    }
    
    void Deflate( TYPE d )
    {
        vmin += Vector<TYPE>(d, d, d);
        vmax -= Vector<TYPE>(d, d, d);
    }

    void Merge( const Vector<TYPE>& v )
    {
        if( v.x < vmin.x ) vmin.x = v.x;
        if( v.y < vmin.y ) vmin.y = v.y;
        if( v.z < vmin.z ) vmin.z = v.z;
        if( v.x > vmax.x ) vmax.x = v.x;
        if( v.y > vmax.y ) vmax.y = v.y;
        if( v.z > vmax.z ) vmax.z = v.z;
    }

    bool IsInside( const Vector<TYPE>& v ) const
    {
        return (v.x >= vmin.x && v.y >= vmin.y && v.z >= vmin.z &&
                v.x <= vmax.x && v.y <= vmax.y && v.z <= vmax.z);
    }

    bool IsEmpty() const
    {
        return (vmin.x >= vmax.x || vmin.y >= vmax.y || vmin.z >= vmax.z);
    }

    static bool IsIntersection( const Box<TYPE>& b0, const Box<TYPE>& b1 )
    {
        return !(b0.vmax.x < b1.vmin.x || b0.vmax.y < b1.vmin.y || b0.vmax.z < b1.vmin.z ||
                 b0.vmin.x > b1.vmax.x || b0.vmin.y > b1.vmax.y || b0.vmin.z > b1.vmax.z);
    }

    static const Box<TYPE> Intersection( const Box<TYPE>& b0, const Box<TYPE>& b1 )
    {
        Box<TYPE> b( Max(b0.vmin.x, b1.vmin.x),
                     Max(b0.vmin.y, b1.vmin.y),
                     Max(b0.vmin.z, b1.vmin.z),
                     Min(b0.vmax.x, b1.vmax.x),
                     Min(b0.vmax.y, b1.vmax.y),
                     Min(b0.vmax.z, b1.vmax.z) );
        if( b.IsEmpty() )
            b.Reset();
        return b;
    }

    static const Box<TYPE> Union( const Box& b0, const Box& b1 )
    {
        if( !b0.IsEmpty() && !b1.IsEmpty() )
        {
            return Box<TYPE>( Min(b0.vmin.x, b1.vmin.x),
                              Min(b0.vmin.y, b1.vmin.y),
                              Min(b0.vmin.z, b1.vmin.z),
                              Max(b0.vmax.x, b1.vmax.x),
                              Max(b0.vmax.y, b1.vmax.y),
                              Max(b0.vmax.z, b1.vmax.z) );
        }
        else
        if( !b0.IsEmpty() )
            return b0;
        else
        if( !b1.IsEmpty() )
            return b1;
        else
            return Box<TYPE>( (TYPE)0, (TYPE)0, (TYPE)0, (TYPE)0, (TYPE)0, (TYPE)0 );
    }
};

typedef Box<int>    iBox;
typedef Box<float>  fBox;
typedef Box<double> dBox;

#ifndef NDEBUG
template class Box<int>;
template class Box<float>;
template class Box<double>;
#endif

#endif  // BOX_H
