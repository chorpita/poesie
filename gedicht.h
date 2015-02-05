#ifndef __POETICS_H_
#define __POETICS_H_


class CWord 
{
public:
	virtual ~CWord() { }

	int GetNumSyllables() { return m_nNumSyllables; }
    LPSTR GetText() { return m_strText;  }
	LPSTR GetPhone() { return m_Phone;  }

protected:
	m_nNumSyllables;
	m_strText;
	m_strPhon;
};


#endif // __POETICS_H_


