// CHICKEN.CPP - Advanced Compiler

#include "stdafx.h"
#include "generic.h"
#include "poetics.h"
#include "lexicon.h"
#include "compile.h"
#include "phonetics.h"
#include "syntax.h"
#include "semantic.h"
#include "chicken.h"
#include "wn/wn.h"

#define QUOTE_CHAR '\"'

void CChickenCompiler::InsertPierWord( CString & strWord, enumSyn eSyn )
{
	InsertBrainWord( strWord, eSyn );
}

void CChickenCompiler::InsertBrainWord( CString & strWord, enumSyn eSyn )
{
	CString strBase;
	switch( GetSynPOS( eSyn ) )
	{
	case VERB:
			strBase = m_LB.m_pVerbs->RandElement()->m_strKey;
			break;
	case NOUN:
			strBase = m_LB.m_pNouns->RandElement()->m_strKey;
			break;
	case ADJ:
			strBase = m_LB.m_pAdjs->RandElement()->m_strKey;
			break;
	case ADV:
			strBase = m_LB.m_pAdvs->RandElement()->m_strKey;
			break;
	}

	switch( eSyn )
	{
	case s_N:
	case s_V:
	case s_A:
	case s_R:
			strWord = strBase;
			break;				
	case s_NP:
			m_EXC.NounPlural( strBase, strWord );
			break;
	case s_VS:
			m_EXC.VerbThirdSingular( strBase, strWord );
			break;
	case s_VG:
			m_EXC.VerbGerund( strBase, strWord );
			break;
	case s_VA:
			m_EXC.VerbPast( strBase, strWord );
			break;
	case s_VP:
			m_EXC.VerbPerfect( strBase, strWord );
			break;
	case s_AC:
			m_EXC.AdjCompare( strBase, strWord );
			break;
	case s_AS:
			m_EXC.AdjSuper( strBase, strWord );
			break;
	default:
			ASSERT( FALSE );
	}
}

BOOL CChickenCompiler::InsertWord( CString & str, LPCSTR szToken, enumSyn eSyn )
{
		int nStart = str.Find(  szToken );
		BOOL bFail =  ( nStart == -1 );
		if( ! bFail )
		{
			int nEnd = str.Find( '>', nStart );
			ASSERT( nEnd > 0 );
			int nOpenQ = str.Find( QUOTE_CHAR, nStart );
			ASSERT( nOpenQ > 0 );
			int nCloseQ = str.Find( QUOTE_CHAR, nOpenQ + 1 );
			ASSERT( nCloseQ > 0 );
			CString strTag( str.Mid( nStart, nEnd - nStart + 1 ) );
			CString strWord;

			CRandPick kPlug;
			kPlug.Fill( WPLUG_POET, 2 );
			kPlug.Fill( WPLUG_BRAIN, 4 );
			kPlug.Fill( WPLUG_PIER, 3 );

			switch( kPlug.Pick() )
			{
			case WPLUG_POET:
				strWord = str.Mid( nOpenQ + 1, nCloseQ - nOpenQ - 1 );
				break;
			case WPLUG_BRAIN:
				InsertBrainWord( strWord, eSyn );
				break;
			case WPLUG_PIER:
				InsertPierWord( strWord, eSyn );
				break;
			default:
				ASSERT( FALSE );
			}
			str.Replace( strTag,  strWord );
		}
		return bFail;
}

BOOL CChickenCompiler::Run( CString & strPoem ) 
{
	CString strLocal( strPoem );

	 BOOL bRunning = strLocal.Find( "<" ) >= 0;

	 if( strLocal.Find( "<horn>" ) > 0 )
	 {
		 int nInfo = strLocal.Find( "<info>" );
		 strLocal = strLocal.Left( nInfo );
	 }

	 if( strLocal.Find( "<goat>" ) > 0 )
	 {		
		 strLocal.Replace( "<goat>", "<horn>" );
	 }

	 if( strLocal.Find( "<farmer>" ) > 0 )
	 {	
		 strLocal.Replace( "<top>", "" );
		 /*
		 if( strSyntagma.Left( 2 ) != "I " )
			{
				CString strHead = strSyntagma.Left( 1 );
				if( strLocal.Find( "<lowercase>" ) > 0 )
					strHead.MakeLower();
				else
					strHead.MakeUpper();
				strSyntagma.Delete( 0 );
				strSyntagma.Insert( 0, strHead );
			}	*/

		 strLocal.Replace( "<farmer>", "<goat>" );
	 }

	 if( strLocal.Find( "<ear>" ) > 0 )
	 {	
		int nMark = strLocal.Find( "<ia>" );
		if( nMark > 0 )
		{
			CString strTemp( strLocal.Mid( nMark + 5 ) );
			CString strWord( strTemp.Left( strTemp.FindOneOf( " .'\"?!,:" ) ) );
			strLocal.Delete( nMark, 4 );
			if( m_IPA.IndefiniteArticleAn( strWord ) )
				strLocal.Insert( nMark, "an" );
			else
				strLocal.Insert( nMark, 'a' );
		}
		else
			strLocal.Replace( "<ear>", "<farmer>" );
	 }

	 if( strLocal.Find( "<drum>" ) > 0 )
	 {
		BOOL bFail = InsertWord( strLocal, "<n ", s_N );
		if( bFail )
			 bFail = InsertWord( strLocal, "<np", s_NP );
		if( bFail )
			 bFail = InsertWord( strLocal, "<a ", s_A );
		if( bFail )
			 bFail = InsertWord( strLocal, "<ac", s_AC );
		if( bFail )
			 bFail = InsertWord( strLocal, "<as", s_AS );
		if( bFail )
			 bFail = InsertWord( strLocal, "<v ", s_V );
		if( bFail )
			 bFail = InsertWord( strLocal, "<va", s_VA );
		if( bFail )
			 bFail = InsertWord( strLocal, "<vp", s_VP );
		if( bFail )
			 bFail = InsertWord( strLocal, "<vs", s_VS );
		if( bFail )
			 bFail = InsertWord( strLocal, "<vg", s_VG );
		if( bFail )
			 bFail = InsertWord( strLocal, "<r ", s_R );
		if( bFail )
			strLocal.Replace( "<drum>", "<ear>" );
	}

	 if( strLocal.Find( "<castle>" ) > 0 )
	 {
		if( strLocal.Find( "<title>" ) > 0 )
		{
			int nIndex = RandVal( m_SF.m_Title.size() );
			CString strSyntagma = "<top>" + m_SF.m_Title[  nIndex ]->m_strKey;

			if( m_nDebugFlag == 1 )
			{
				char szCode[ 20 ];
				wsprintf( szCode, "   (%d)", nIndex );
				strSyntagma += szCode;
			}
			strLocal.Replace( "<title>",  strSyntagma );
		}
		strLocal.Replace( "<castle>", "<drum>" );
	 }

	 if( strLocal.Find( "<bear>" ) > 0 )
	 {	
		int nVStart = strLocal.Find( "<verse" );
		if( nVStart >= 0 )
		{
			int nVEnd = strLocal.Find( '>', nVStart );
			ASSERT( nVEnd > 0 );
			CString strVerse( strLocal.Mid( nVStart, nVEnd - nVStart + 1 ) );
			int nIndex = RandVal( m_SF.m_Text.size() );
			CString strSyntagma = "<top>" + m_SF.m_Text[  nIndex ]->m_strKey;

			if( m_nDebugFlag == 1 )
			{
				char szCode[ 20 ];
				wsprintf( szCode, "   (%d)", nIndex );
				strSyntagma += szCode;
			}
			strLocal.Replace( strVerse,  strSyntagma );
		}
		else
			strLocal.Replace( "<bear>", "<castle>" );
	 }

	 int i; int j;
	char s[ 200 ];	char t[ 200 ];

	if( strLocal.Find( "<apple>" ) > 0 )
	{
		CRandPick kFixedV;
		kFixedV.Fill( FALSE, 7 );
		kFixedV.Fill( TRUE, 1 );
		BOOL bFixedV = kFixedV.Pick();

		CRandPick kVersen;
		kVersen.Fill( 4, 10 );
		kVersen.Fill( 3, 1 );
		kVersen.Fill( 6, 1 );
		kVersen.Fill( 2, 1 );
		int nVerse = kVersen.Pick();

		CRandPick kSilben;
		kSilben.Fill( 6, 4 );
		kSilben.Fill( 8, 6 );
		kSilben.Fill( 10, 4 );
		int nSilb = kSilben.Pick();

		strLocal.Replace( "<apple>", "<bear>" );
		for( i = 1; i < 8; i++ )
		{

			if( ! bFixedV )
			{
				CRandPick kVersenD;
				kVersenD.Fill( 4, 60 );
				kVersenD.Fill( 3, 3 );
				kVersenD.Fill( 6, 1 );
				kVersenD.Fill( 2, 5 );
				kVersenD.Fill( 1, 5 );
				nVerse = kVersenD.Pick();
			}

			wsprintf( t, "<block%ld>", i );
			for( j = 0; j< nVerse - 1; j++ )
			{
				wsprintf( s, "%s\n<verse%ld%ld sy=%ld>", t, i, nVerse - j, nSilb );
				strLocal.Replace( t, s );
			}
			wsprintf( s, "<verse%ld1 sy=%ld>" , i, nSilb );
			strLocal.Replace( t, s );
		}

		CRandPick kLower;
		kLower.Fill( FALSE, 5 );
		kLower.Fill( TRUE, 2 );
		if( kLower.Pick() )
			strLocal.Replace( "<info>", "<info>\n<lowercase>" );
	}
		
	if( strLocal.Find( "<new>" ) > 0 )
	{
		CRandPick kPoem;
		kPoem.Fill( POEM_RANDOM, 15 );
		kPoem.Fill( POEM_SONNETT, 2 );
		kPoem.Fill( POEM_HAIKU, 1 );
		strLocal.Replace( "<new>", "<apple>" );
		 switch( kPoem.Pick()  )
		{      
		 int i;
		case POEM_RANDOM:
			{
			int nBlock = 3; //+ RandVal( 3 );
			for( i = 1; i < nBlock - 1; i++ )
			{
				 wsprintf( s,  "<body>\n\n<block%ld>", nBlock - i );
				strLocal.Replace( "<body>", s );
			}
			strLocal.Replace( "<body>", "<block1>" );
		}
		break;
		case POEM_SONNETT:
			strLocal.Replace( "<body>", "<a>\n\n<b>\n\n<c>\n\n<d>" );  
			strLocal.Replace( "<a>", "<verse11 <*>>\n<verse12 <*>>\n<verse13 <*>>\n<verse14 <*>>" );
			strLocal.Replace( "<b>", "<verse21 <*>>\n<verse22 <*>>\n<verse23 <*>>\n<verse24 <*>>" );
			strLocal.Replace( "<c>", "<verse31 <*>>\n<verse32 <*>>\n<verse33 <*>>" );
			strLocal.Replace( "<d>", "<verse41 <*>>\n<verse42 <*>>\n<verse43 <*>>" );
			strLocal.Replace( "<*>", "sy=10" );
			strLocal.Replace( "<info>", "<info>\n<sonnett>" );
			strLocal.Replace( "<apple>", "<bear>" );
		 break;
		case POEM_HAIKU:
		 strLocal.Replace( "<body>", "<a>\n<b>\n<c>" );
		 strLocal.Replace( "<a>", "<verse11 sy=5>" );
		 strLocal.Replace( "<b>", "<verse12 sy=7>" );
		 strLocal.Replace( "<c>", "<verse13 sy=5>" );
		 strLocal.Replace( "<info>", "<info>\n<haiku>" );
		 strLocal.Replace( "<apple>", "<bear>" );
		 break;
		}
	}

	if( strLocal.Find( "<poem>" ) >= 0 )
	{
		CRandPick kTitle;
		kTitle.Fill( FALSE, 2 );
		kTitle.Fill( TRUE, 97 );

		 if( kTitle.Pick()  )
		      strLocal.Replace( "<poem>", "\n<title>\n\n<body> \n\n<info>\n<new>");
		 else
			 strLocal.Replace( "<poem>", "\n<body>\n\n<info>\n<new>" );
	}

	strPoem = strLocal;

	 return bRunning;
}

// eof - CHICKEN.CPP

