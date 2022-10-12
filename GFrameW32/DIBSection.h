//-----------------------------------------------------------------------------
// File: DIBSection.h
// Desc: Declaration of wrapper class for Windows DIB section.
//
// Copyright (c) 2011 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#ifndef DIBSECTION_H
#define DIBSECTION_H

class CDIBSection
{
protected:
    int         m_nWidth;
    int         m_nHeight;
    HBITMAP     m_hBitmap;
    RGBQUAD*    m_pBits;

public:
    CDIBSection();
    ~CDIBSection();

    bool Create( int w, int h );
    void Release();

    bool Load( const TCHAR* name );
    bool Save( const TCHAR* name ) const;
    bool SaveToClipboard() const;

    HBITMAP     Bitmap() const      { return m_hBitmap; }
    RGBQUAD*    Bits()              { return m_pBits; }
    const RGBQUAD* Bits() const     { return m_pBits; }
    int         Width() const       { return m_nWidth; }
    int         Height() const      { return m_nHeight; }

    operator    HBITMAP() const     { return m_hBitmap; }
    operator    RGBQUAD*()          { return m_pBits; }
    operator    const RGBQUAD*() const { return m_pBits; }

    void Swap( CDIBSection& other );

private:
    CDIBSection& operator=( const CDIBSection& );
    CDIBSection( const CDIBSection& );
};

#endif  // DIBSECTION_H
