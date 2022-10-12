//-----------------------------------------------------------------------------
// File: DIBSection.cpp
// Desc: Definition of wrapper class for Windows DIB section.
//
// Copyright (c) 2011 Konstantin Kryzhanovsky
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "DIBSection.h"

CDIBSection::CDIBSection() :
m_nWidth(0), m_nHeight(0), m_hBitmap(NULL), m_pBits(NULL)
{}

CDIBSection::~CDIBSection()
{
    Release();
}

bool CDIBSection::Create( int w, int h )
{
    ASSERT( w != 0 && h != 0 );

    if( w == m_nWidth && h == m_nHeight )
        return true;
    else
    {
        Release();

        BITMAPINFO bi = {0};
        bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
        bi.bmiHeader.biWidth = w;
        bi.bmiHeader.biHeight = h;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        bi.bmiHeader.biSizeImage = 0;

        m_hBitmap = ::CreateDIBSection( NULL, &bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0 );
        if( m_hBitmap != NULL && m_pBits != NULL )
        {
            m_nWidth = w;
            m_nHeight = h;
            return true;
        }
        else
            return false;
    }
}

void CDIBSection::Release()
{
    if( m_hBitmap != NULL )
    {
        ::DeleteObject( m_hBitmap );
        m_hBitmap = NULL;
        m_pBits = NULL;

        m_nWidth = 0;
        m_nHeight = 0;
    }
}

bool CDIBSection::Load( const TCHAR* name )
{
    bool ret = false;

    FILE* file = _tfopen( name, _T("rb") );
    if( file != NULL )
    {
        fseek( file, 0, SEEK_END );
        long size = ftell(file);
        fseek( file, 0, SEEK_SET );
        Array<BYTE> buf;
        if( buf.SetSize(size) )
        {
            if( fread(&buf[0], 1, size, file) == (size_t)size )
            {
                const BITMAPFILEHEADER* fileHeader = (const BITMAPFILEHEADER*)&buf[0];
                BITMAPINFO*             srcInfo = (BITMAPINFO*)(fileHeader + 1);

                // Some extra check
                const char bmId[] = "BM";
                if( fileHeader->bfType == *(WORD*)bmId )    // Check file id
                {
                    HDC hDC = ::CreateCompatibleDC(NULL);
                    if( hDC != NULL )
                    {
                        int w, h;
                        if( srcInfo->bmiHeader.biSize != sizeof(BITMAPCOREHEADER) )
                        {
                            // This is native Windows bitmap of some kind
                            w = srcInfo->bmiHeader.biWidth;
                            h = srcInfo->bmiHeader.biHeight;

                            // Validate biSizeImage
                            if( srcInfo->bmiHeader.biCompression == BI_RLE4 ||
                                srcInfo->bmiHeader.biCompression == BI_RLE8 )
                                srcInfo->bmiHeader.biSizeImage = size - fileHeader->bfOffBits;
                        }
                        else
                        {
                            // This is OS/2 bitmap
                            const BITMAPCOREHEADER* os2Info = (const BITMAPCOREHEADER*)srcInfo;
                            w = os2Info->bcWidth;
                            h = os2Info->bcHeight;
                        }

                        if( Create(w, -h) )
                        {
                            if( ::SetDIBits( hDC, m_hBitmap, 0, abs(h), &buf[0] + fileHeader->bfOffBits, srcInfo, DIB_RGB_COLORS ) ==
                                abs(h) )
                                ret = true;
                        }
                        ::DeleteDC(hDC);
                    }
                }
            }
        }
        fclose(file);
    }
    return ret;
}

bool CDIBSection::Save( const TCHAR* name ) const
{
    bool ret = false;

    if( m_pBits != NULL )
    {
        int pitch = ((m_nWidth * sizeof(RGBTRIPLE) + 3) / 4) * 4;
        
        size_t size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pitch * abs(m_nHeight);
        Array<BYTE> buf;
        if( buf.SetSize((Array<BYTE>::size_type)size) )
        {
            BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*)&buf[0];
            BITMAPINFOHEADER* bi = (BITMAPINFOHEADER*)(fh + 1);

            fh->bfType = *(const WORD*)"BM";
            fh->bfSize = (DWORD)(sizeof(*fh) + sizeof(*bi) + pitch * abs(m_nHeight));
            fh->bfReserved1 = 0;
            fh->bfReserved2 = 0;
            fh->bfOffBits = (DWORD)(sizeof(*fh) + sizeof(*bi));

            bi->biSize = (DWORD)sizeof(*bi);
            bi->biWidth = m_nWidth;
            bi->biHeight = abs(m_nHeight);
            bi->biPlanes = 1;
            bi->biBitCount = 24;
            bi->biCompression = BI_RGB;
            bi->biSizeImage = (DWORD)(pitch * abs(m_nHeight));
            bi->biXPelsPerMeter = 10000 * 72 / 254;  // 72 dpi
            bi->biYPelsPerMeter = 10000 * 72 / 254;  // 72 dpi
            bi->biClrUsed = 0; 
            bi->biClrImportant = 0;

            RGBTRIPLE*     pd = (RGBTRIPLE*)(bi + 1);
            const RGBQUAD* ps = m_pBits;
            int nextLine = m_nWidth;
            if( m_nHeight < 0 )
            {
                ps += (abs(m_nHeight) - 1) * m_nWidth;
                nextLine = -m_nWidth;
            }
            for( int y = 0; y < abs(m_nHeight); ++y )
            {
                for( int x = 0; x < m_nWidth; ++x )
                {
                    pd[x].rgbtBlue  = ps[x].rgbBlue;
                    pd[x].rgbtGreen = ps[x].rgbGreen;
                    pd[x].rgbtRed   = ps[x].rgbRed;
                }
                ps += nextLine;
                pd = (RGBTRIPLE*)( (BYTE*)pd + pitch );
            }

            FILE* file = _tfopen( name, _T("wb") );
            if( file != NULL )
            {
                ret = (size == fwrite( buf, 1, size, file ));
                fclose(file);
            }
        }
    }
    return ret;
}

bool CDIBSection::SaveToClipboard() const
{
    bool ret = false;

    if( m_pBits != NULL )
    {
        int pitch = ((m_nWidth * sizeof(RGBTRIPLE) + 3) / 4) * 4;
        SIZE_T size = sizeof(BITMAPINFOHEADER) + pitch * abs(m_nHeight);
        HANDLE hMem = ::GlobalAlloc( GMEM_DDESHARE | GMEM_MOVEABLE, size );
        if( hMem != NULL )
        {
            BITMAPINFOHEADER* bi = (BITMAPINFOHEADER*)::GlobalLock(hMem);

            // Init BITMAPINFO structures
            memset( bi, 0, sizeof(*bi) );
            bi->biSize = (DWORD)sizeof(*bi);
            bi->biWidth = m_nWidth;
            bi->biHeight = abs(m_nHeight);
            bi->biPlanes = 1;
            bi->biBitCount = 24;
            bi->biCompression = BI_RGB;
            bi->biSizeImage = (DWORD)(pitch * abs(m_nHeight));
            bi->biXPelsPerMeter = 10000 * 72 / 254;  // 72 dpi
            bi->biYPelsPerMeter = 10000 * 72 / 254;  // 72 dpi
            bi->biClrUsed = 0; 
            bi->biClrImportant = 0;

            // Copy pixels
            RGBTRIPLE*     pd = (RGBTRIPLE*)(bi + 1);
            const RGBQUAD* ps = m_pBits;
            int nextLine = m_nWidth;
            if( m_nHeight < 0 )
            {
                ps += (abs(m_nHeight) - 1) * m_nWidth;
                nextLine = -m_nWidth;
            }
            for( int y = 0; y < abs(m_nHeight); ++y )
            {
                for( int x = 0; x < m_nWidth; ++x )
                {
                    pd[x].rgbtBlue  = ps[x].rgbBlue;
                    pd[x].rgbtGreen = ps[x].rgbGreen;
                    pd[x].rgbtRed   = ps[x].rgbRed;
                }
                ps += nextLine;
                pd = (RGBTRIPLE*)( (BYTE*)pd + pitch );
            }

            ::GlobalUnlock( hMem );

            ret = (SetClipboardData( CF_DIB, hMem ) != NULL);
            // Note: We do NOT need to free DIB memory.
        }
    }
    return ret;
}

void CDIBSection::Swap( CDIBSection& other )
{
    ::Swap(m_nWidth, other.m_nWidth);
    ::Swap(m_nHeight, other.m_nHeight);
    ::Swap(m_hBitmap, other.m_hBitmap);
    ::Swap(m_pBits, other.m_pBits);
}
