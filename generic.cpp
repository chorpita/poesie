// poesieDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CPersistent::Load( LPCSTR szMainFile, LPCSTR szRescueFile )
{
	if ( CFileFind().FindFile( szMainFile ) )
	{
    	CStdioFile file( szMainFile,  CFile::modeRead | CFile::typeBinary );
	    CArchive ar( & file, CArchive::load );
		Serialize( ar );
		ar.Close();	
		file.Close();

		// make a backup of successfully read MAIN in RESCUE
		if( szRescueFile )
		{
			if( CFileFind().FindFile( szRescueFile ) )
				CFile::Remove( szRescueFile );
			CFile::Rename( szMainFile, szRescueFile );
		}
	}
	else
	{
		// MAIN FILE lost because of crash or because user deleted it
		if( szRescueFile )
			if( CFileFind().FindFile( szRescueFile ) ) 
			{
				//  use rescue file instead
				CStdioFile file( szRescueFile,  CFile::modeRead | CFile::typeBinary );
				CArchive ar( & file, CArchive::load );
				Serialize( ar );
				ar.Close();	
				file.Close();
			}
	}
}

void CPersistent::Commit( LPCSTR szMainFile )
{
	CStdioFile file( szMainFile,  CFile::modeWrite | CFile::modeCreate | CFile::typeBinary );
	CArchive ar( & file, CArchive::store );
	Serialize( ar );
	ar.Close();
	file.Close();	
}


void CRandPick::Fill( int nVal, int nCount )
{
	ASSERT( m_nSize + nCount <= MAX_R_DSTRB );
	for( int i = m_nSize; i < m_nSize + nCount; i++ )
		m_nArray[ i ] = nVal;
	m_nSize += nCount;
}

int CRandPick::Pick()
{
	return m_nArray[ RandVal( m_nSize ) ];
}

void CSimpleChunker::Chunk( LPCSTR szBuffer )
{
	char szToken[ 200 ];
	BOOL bDone = FALSE;	
	int i = 0;
	int j = 0;
	while( szBuffer[ i ] != '\0' )
	{
		InterruptPoint( szBuffer, i );

		if( IsValidChar( szBuffer[ i ] ) )
		{
			szToken[ j ] = szBuffer[ i ];
			j++;
		}
		else
		{
			szToken[ j ] = '\0';
			if( j > 0 )
				Add( szToken );
			j = 0;
		}
		i++;
	}
	szToken[ j ] = '\0';
	if( j > 0 )
		Add( szToken );
}


void RandInitialize()
{
	SYSTEMTIME st;
	GetSystemTime( & st );
	srand( (UINT) st.wSecond * 637 * st.wMinute );  
	rand(); rand(); rand();
}

int RandVal( int nRange )
{
	return rand() % nRange;
}

void ScrambleVector( int nRange, int * pVector )
{
	if( nRange <= 1 )
	{
		if( nRange == 1 )
			pVector[ 0 ] = 0;
	}
	else
	{
		for( int i = 0; i < nRange; i++ )
			pVector[ i ] = -1;
		int nLocalRange = nRange;
		while( nLocalRange != 0 )
		{
			int nTry = RandVal( nLocalRange );
			i = 0;
			int j = 0;
			while( i <= nTry )
				if( pVector[ j++ ] == - 1 )
					i++;
			nLocalRange--;		
			pVector[ j - 1 ] = nLocalRange;
		}
	}
}

BOOL SimpleScanf(LPCTSTR lpszText, LPCTSTR lpszFormat, va_list pData)
{
	ASSERT(lpszText != NULL);
	ASSERT(lpszFormat != NULL);

	ASSERT(*lpszFormat == '%');
	lpszFormat++;        // skip '%'

	BOOL bLong = FALSE;
	BOOL bShort = FALSE;
	if (*lpszFormat == 'l')
	{
		bLong = TRUE;
		lpszFormat++;
	}
	else if (*lpszFormat == 's')
	{
		bShort = TRUE;
		lpszFormat++;
	}

	ASSERT(*lpszFormat == 'd' || *lpszFormat == 'u');
	ASSERT(lpszFormat[1] == '\0');

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	TCHAR chFirst = lpszText[0];
	long l, l2;
	if (*lpszFormat == 'd')
	{
		// signed
		l = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (int)l;
	}
	else
	{
		// unsigned
		if (*lpszText == '-')
			return FALSE;
		l = (long)_tcstoul(lpszText, (LPTSTR*)&lpszText, 10);
		l2 = (unsigned int)l;
	}
	if (l == 0 && chFirst != '0')
		return FALSE;   // could not convert

	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;
	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	if (bShort)
	{
		if ((short)l != l)
			return FALSE;   // too big for short
		*va_arg(pData, short*) = (short)l;
	}
	else
	{
		ASSERT(sizeof(long) == sizeof(int));
		ASSERT(l == l2);
		*va_arg(pData, long*) = l;
	}

	// all ok
	return TRUE;
}

void ParseInteger( TCHAR * szText, UINT nIDPrompt, ... )
{	
	va_list pData;
	va_start( pData, nIDPrompt );
	// the following works for %d, %u, %ld, %lu
	SimpleScanf( szText, "%d", pData );
	va_end( pData );
}

void ParseNumericControl( CWnd * pCtrl, int * nVal )
{	
	ASSERT( pCtrl );
	TCHAR szT[ 32 ];
	pCtrl->GetWindowText( szT, 32 );
	ParseInteger( szT, AFX_IDP_PARSE_INT, nVal );
}

BOOL IsEnglishVowel( TCHAR c )
{
	switch( c )
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return TRUE;
	default:
		return FALSE;
	}
}

void InflectS( CString & strBase, CString & strResult )
{
	CString str( strBase );
	int nLen = str.GetLength();
	char cLast = str.GetAt( nLen - 1 );
	char cNLast = (nLen > 1) ? str.GetAt( nLen - 2 ) : 'x';
	char cNNLast = ( nLen > 2 ) ? str.GetAt( nLen - 3 ) : 'b';
	switch( cLast )
	{	
	case 'h':
		if( cNLast == 's' )
			str += 'e';
		if( ( cNNLast == 't' ) && (cNLast == 'c' ) )
			str += 'e';
		break;
	case 'f':
		if( cNLast != 'f' )
		{
			// still makes errors with words like "golf", "beef", but hard to correct...
			str.Delete( nLen - 1 );
			str += "ve";
		}
		break;
	case 'y':
		if( ! IsEnglishVowel( cNLast ) )
		{
			str.Delete( nLen - 1 );
			str += "ie";
		}
		break;
	case 's':
	case 'x':
		str += 'e';
		break;
	case 'o':
		if( ! IsEnglishVowel( cNLast ) )
			str += 'e';
		break;
	case 'e':
		if( ( cNLast == 'f' ) && (cNNLast != 'f' ) )
			str.Replace( "fe", "ve" );
		break;
	default:
		break;
	}
	strResult = str + 's';
}

void InflectED( CString & strBase, CString & strResult )
{
	if( strBase.Right( 1 ) == "e" ) // improve this !!!
		strResult = strBase + "d";
	else
		strResult = strBase + "ed";
}

// end - generic.cpp

