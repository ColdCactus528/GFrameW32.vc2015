//-----------------------------------------------------------------------------
// File: Array.h
// Desc: Declaration and definition of dynamic array template.
//
// Copyright (c) 2011 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef ARRAY_H
#define ARRAY_H

template<class TYPE>
class Array
{
public:
    typedef int size_type;

    // Construction/Destruction
    Array() :
    m_pData(NULL), m_nSize(0), m_nMaxSize(0)
    {}
    
    Array( const Array<TYPE>& arr ) :
    m_pData(NULL), m_nSize(0), m_nMaxSize(0)
    {
        if( Reserve(arr.m_nSize) )
        {
            for( size_type i = 0; i < arr.m_nSize; ++i )
                m_pData[i] = arr.m_pData[i];
            m_nSize = arr.m_nSize;
        }
    }

    explicit Array( size_type size ) :
    m_pData(NULL), m_nSize(0), m_nMaxSize(0)
    {
        if( Reserve(size) )
        {
            m_nSize = size;
        }
    }

    explicit Array( size_type size, const TYPE& d ) :
    m_pData(NULL), m_nSize(0), m_nMaxSize(0)
    {
        if( Reserve(size) )
        {
            for( size_type i = 0; i < size; ++i )
                m_pData[i] = d;
            m_nSize = size;
        }
    }

    explicit Array( size_type size, const TYPE* src ) :
    m_pData(NULL), m_nSize(0), m_nMaxSize(0)
    {
        if( Reserve(size) )
        {
            if( src != NULL )
            {
                for( size_type i = 0; i < size; ++i )
                    m_pData[i] = src[i];
            }
            m_nSize = size;
        }
    }

    ~Array()
    {
        Empty();
    }

    // Assignment operator
    Array<TYPE>& operator=( const Array<TYPE>& arr )
    {
        Array<TYPE> tmp(arr);
        Swap(tmp);
        return *this;
    }

    // Assign number of same elements to array
    bool Assign( size_type size, const TYPE& d )
    {
        Array<TYPE> tmp(size, d);
        Swap(tmp);
        return (m_nSize == size);
    }

    // Assign number of elements to array
    bool Assign( size_type size, const TYPE* src )
    {
        Array<TYPE> tmp( size, src );
        Swap(tmp);
        return (m_nSize == size);
    }

    void Fill( const TYPE& d )
    {
        for( size_type i = 0; i < m_nSize; ++i )
            m_pData[i] = d;
    }

    size_type GetSize() const                       { return m_nSize; }
    bool    IsEmpty() const                         { return (m_nSize == 0); }
    bool    IsValidIndex( size_type index ) const   { return (index >= 0 && index < m_nSize); }

    // Data access
    template <class INDEX>
    TYPE& operator[]( INDEX index )                 { ASSERT(index >= 0 && (size_type)index < m_nSize); return m_pData[index]; }
    template <class INDEX>
    const TYPE& operator[]( INDEX index ) const     { ASSERT(index >= 0 && (size_type)index < m_nSize); return m_pData[index]; }
    TYPE& Back()                                    { ASSERT(m_nSize > 0); return m_pData[m_nSize - 1]; }
    const TYPE& Back() const                        { ASSERT(m_nSize > 0); return m_pData[m_nSize - 1]; }
    template <class INDEX>
    TYPE& Back( INDEX index )                       { ASSERT(index >= 0 && (size_type)index < m_nSize); return m_pData[m_nSize - index - 1]; }
    template <class INDEX>
    const TYPE& Back( INDEX index ) const           { ASSERT(index >= 0 && (size_type)index < m_nSize); return m_pData[m_nSize - index - 1]; }
    operator TYPE*()                                { return m_pData; }
    operator const TYPE*() const                    { return m_pData; }

    // Change array size
    bool SetSize( size_type size )
    {
        if( Reserve(size) )
        {
            m_nSize = size;
            return true;
        }
        return false;
    }

    // Add space for single element
    bool Add()
    {
        if( Reserve(m_nSize + 1) )
        {
            ++m_nSize;
            return true;
        }
        return false;
    }

    // Add single element
    bool Add( const TYPE& d )
    {
        if( Reserve(m_nSize + 1) )
        {
            m_pData[m_nSize++] = d;
            return true;
        }
        return false;
    }

    // Add another array
    bool Add( const Array<TYPE>& arr )
    {
        return Add( arr.m_nSize, arr.m_pData );
    }

    // Add number of elements with the same value
    bool Add( size_type size, const TYPE& d )
    {
        if( Reserve(m_nSize + size) )
        {
            for( size_type i = 0; i < size; ++i )
                m_pData[m_nSize++] = d;
            return true;
        }
        return false;
    }

    // Add number of elements
    bool Add( size_type size, const TYPE* src )
    {
        if( Reserve(m_nSize + size) )
        {
            if( src != NULL )
            {
                for( size_type i = 0; i < size; ++i )
                    m_pData[m_nSize + i] = src[i];
            }
            m_nSize += size;
            return true;
        }
        return false;
    }

    // Insert element at given position
    bool Insert( size_type index, const TYPE& d )
    {
        if( index >= 0 && index <= m_nSize )
        {
            if( Reserve(m_nSize + 1) )
            {
                // Shift elements after inserted one
                for( size_type i = m_nSize; i > index; --i )
                    m_pData[i] = m_pData[i - 1];
                m_pData[index] = d;
                ++m_nSize;
                return true;
            }
        }
        return false;
    }

    // Insert another array at given position
    bool Insert( size_type index, const Array<TYPE>& arr )
    {
        return Insert( index, arr.m_nSize, arr.m_pData );
    }

    // Insert number of same elements at given position
    bool Insert( size_type index, size_type size, const TYPE& d )
    {
        if( index >= 0 && index <= m_nSize )
        {
            if( Reserve(m_nSize + size) )
            {
                // Shift elements after inserted ones
                size_type i;
                for( i = m_nSize + size - 1; i > index; --i )
                    m_pData[i] = m_pData[i - size];

                // Copy new elements
                for( i = 0; i < size; ++i )
                    m_pData[index++] = d;
                m_nSize += size;
                return true;
            }
        }
        return false;
    }

    // Insert number of elements at given position
    bool Insert( size_type index, size_type size, const TYPE* src )
    {
        if( index >= 0 && index <= m_nSize )
        {
            if( Reserve(m_nSize + size) )
            {
                // Shift elements after inserted ones
                size_type i;
                for( i = m_nSize - 1; i >= index; --i )
                    m_pData[i + size] = m_pData[i];

                if( src != NULL )
                {
                    // Copy new elements
                    for( i = 0; i < size; ++i )
                        m_pData[index++] = src[i];
                }
                else
                {
                    // Set inserted elements to defaults
                    for( i = 0; i < size; ++i )
                        m_pData[index++] = TYPE();
                }
                m_nSize += size;
                return true;
            }
        }
        return false;
    }

    // Set element at given position to given value, more safe then element assignment
    // as index checking is performed
    bool SetAt( size_type index, const TYPE& d )
    {
        if( IsValidIndex(index) )
        {
            m_pData[index] = d;
            return true;
        }
        return false;
    }

    // Remove element with given idex
    void Remove( size_type index )
    {
        Remove( index, 1 );
    }

    // Remove number of elements starting from given index
    void Remove( size_type index, size_type size )
    {
        if( IsValidIndex(index) )
        {
            if( m_nSize > size )
            {
                m_nSize -= size;
                for( ; index < m_nSize; ++index )
                    m_pData[index] = m_pData[index + size];
            }
            else
                Empty();
        }
    }

    // Remove all elements equal to specified value
    void RemoveValue( const TYPE& d )
    {
        int di = 0;
        for( int si = 0; si < m_nSize; ++si )
        {
            if( m_pData[si] != d )
            {
                if( di != si )
                    m_pData[di] = m_pData[si];
                ++di;
            }
        }
        m_nSize = di + 1;
    }

    // Destroy all array contents and storage
    void Empty()
    {
        if( m_pData != NULL )
        {
            delete[] m_pData;
            m_pData = NULL;
            m_nSize = 0;
            m_nMaxSize = 0;
        }
    }

    // Swap contents of two arrays
    void Swap( Array<TYPE>& arr )
    {
        TYPE*     tmp_data  = m_pData;
        size_type tmp_size  = m_nSize;
        size_type tmp_max   = m_nMaxSize;
        m_pData             = arr.m_pData;
        m_nSize             = arr.m_nSize;
        m_nMaxSize          = arr.m_nMaxSize;
        arr.m_pData         = tmp_data;
        arr.m_nSize         = tmp_size;
        arr.m_nMaxSize      = tmp_max;
    }
    
    bool Reserve( size_type size )
    {
        if( size <= m_nMaxSize )
            return true;
        if( Realloc( Max( size, (size_type)(m_nMaxSize * 1.7) + 1 ) ) )
            return true;
        return false;
    }

    size_type Find( const TYPE& d ) const
    {
        for( size_type i = 0; i < m_nSize; ++i )
            if( m_pData[i] == d )
                return i;
        return -1;
    }

    template<class EQ>
    size_type Find( const TYPE& d, const EQ eq )
    {
        for( size_type i = 0; i < m_nSize; ++i )
            if( eq(m_pData[i], d) )
                return i;
        return -1;
    }

    size_type FindSorted( const TYPE& d ) const
    {
       size_type low = 0,
                 high = m_nSize;
       while( low < high )
       {
           size_type mid = (low + high) / 2;
           if( m_pData[mid] < d )
               low = mid + 1;
           else
           if( d < m_pData[mid] )
               high = mid;
           else
               return mid;
       }
       return -(low + 1); // not found
    }

    template<class CMP>
    size_type FindSorted( const TYPE& d, CMP cmp ) const
    {
       size_type low = 0,
                 high = m_nSize;
       while( low < high )
       {
           size_type mid = (low + high) / 2;
           if( cmp(m_pData[mid], d) )
               low = mid + 1;
           else
           if( cmp(d, m_pData[mid]) )
               high = mid;
           else
               return mid;
       }
       return -(low + 1); // not found
    }

    size_type InsertSorted( const TYPE& d )
    {
        size_type index = FindSorted( d );
        if( index < 0 )
            index = ~index;
        Insert(index, d);
        return index;
    }

    template<class CMP>
    size_type InsertSorted( const TYPE& d, CMP cmp )
    {
        size_type index = FindSorted( d, cmp );
        if( index < 0 )
            index = ~index;
        Insert(index, d);
        return index;
    }

    size_type AssignSorted( size_type count, const TYPE* src )
    {
        Array<TYPE> tmp;
        tmp.Reserve(count);
        for( size_type i = 0; i < count; ++i )
            tmp.InsertSorted(src[i]);
        Swap(tmp);
        return (m_nSize == count);
    }

    template <class CMP>
    size_type AssignSorted( size_type count, const TYPE* src, CMP cmp )
    {
        Array<TYPE> tmp;
        tmp.Reserve(count);
        for( size_type i = 0; i < count; ++i )
            tmp.InsertSorted(src[i], cmp);
        Swap(tmp);
        return (m_nSize == count);
    }

private:    
    TYPE*       m_pData;            // Pointer to array data
    size_type   m_nSize;            // Number of valid data units
    size_type   m_nMaxSize;         // Number of data units memory allocated for

    bool Realloc( size_type size )
    {
        try
        {
            TYPE* p = new TYPE[size];
            if( p != NULL )
            {
                size_type copy = Min( m_nSize, size );

                for( size_type i = 0; i < copy; ++i )
                    p[i] = m_pData[i];
                if( m_pData != NULL )
                    delete[] m_pData;
                m_pData = p;
                m_nSize = copy;
                m_nMaxSize = size;
                return true;
            }
        }
        catch( const std::bad_alloc& )
        {}
        return false;
    }
};

// Comparison operators
template<class TYPE>
bool operator==(const Array<TYPE>& l, const Array<TYPE>& r)
{
    if( l.GetSize() != r.GetSize() )
        return false;
    for( Array<TYPE>::size_type i = 0; i < l.GetSize(); ++i )
    {
        if( l[i] != r[i] )
            return false;
    }
    return true;
}

template<class TYPE>
bool operator!=(const Array<TYPE>& l, const Array<TYPE>& r)
{
    return !(l == r);
}

#ifdef _DEBUG
template class Array<int>;
#endif

#endif  //  ARRAY_H
