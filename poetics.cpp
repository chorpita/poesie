// poesieDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "poetics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CUserOptions::Serialize( CArchive & ar )
{
	if( ar.IsLoading() )
	{
		ar >> m_bHasTitle >> m_nPoem >> m_nStanzas >> m_nVerses >> m_cbStanza;
		ar >> m_nCompiler >> m_strRootWords >> m_strSyntaxPath >> m_strSavePath;
	}
	else
	{
		ar << m_bHasTitle << m_nPoem << m_nStanzas << m_nVerses << m_cbStanza;
		ar << m_nCompiler << m_strRootWords << m_strSyntaxPath << m_strSavePath;
	}
}

// eof - POETICS.CPP

