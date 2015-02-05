#ifndef __POETICS_H_
#define __POETICS_H_

#define BASE_USERFILE "base.usr"
#define RESCUE_USERFILE "rescue.usr"

#define CMP_WORM 0
#define CMP_CHICKEN 1
#define CMP_DEER 2

#define POEM_FIXED 0
#define POEM_RANDOM 1
#define POEM_SONNETT 2
#define POEM_HAIKU 3

#define RHYME_CROSS 0
#define RHYME_PAIR 1
#define RHYME_HUG 2

#define STANZA_FIXEDVERSE 0
#define STANZA_RANDOMVERSE  1

#define WPLUG_POET 0
#define WPLUG_BRAIN 1
#define WPLUG_PIER 2

class CUserOptions : public CPersistent
{
public:
    CUserOptions() : 
	   m_bHasTitle( TRUE ), 
	   m_nPoem( POEM_FIXED ),
	   m_nStanzas( 4 ),
	   m_nVerses( 4 ),
	   m_cbStanza( STANZA_FIXEDVERSE ),
	   m_nCompiler( CMP_DEER )
	   { 
		   Load( BASE_USERFILE, RESCUE_USERFILE );
	   }
	   virtual ~CUserOptions() { Commit( BASE_USERFILE ); }

	 virtual void Serialize( CArchive & ar );

	int m_bHasTitle;
	int  m_nPoem;
	int m_nStanzas;
	int m_nVerses;
	int m_cbStanza;
	int m_nCompiler;
	CString m_strRootWords;
	CString m_strSyntaxPath;
	CString m_strSavePath;
};

#endif // __POETICS_H_