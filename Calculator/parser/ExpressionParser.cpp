

#include "stdafx.h"
#include "ExpressionParser.h"

using namespace parser;

CExpressionParser::CExpressionParser() 
{
	m_pScan = new CExpressionScanner();
	m_pRmiList = NULL;
	m_bTrace = FALSE;
	m_bError = FALSE;
	m_bAutoRemove = TRUE;

}

CExpressionParser::~CExpressionParser()
{
	SAFE_DELETE(m_pScan);

	if (m_bAutoRemove)
	{
		ReleaseRmi(m_pRmiList);
	}
}


//---------------------------------------------------------------------
// 튜토리얼 스크립트를 파싱한다.
//---------------------------------------------------------------------
sRmi* CExpressionParser::Parse( const char *szFileName, BOOL bTrace )
{
	if( !m_pScan->LoadFile(szFileName, bTrace) )
		return NULL;

	strcpy_s( m_FileName, sizeof(m_FileName), szFileName );
	printf( "%s file Compile\n", szFileName );

	m_Token = m_pScan->GetToken();
	if( ENDFILE == m_Token )
	{
		m_pScan->Clear();
		return NULL;
	}

	m_pRmiList = rmi_list();

	if( ENDFILE != m_Token )
	{
		SyntaxError( " code ends before file " );
		PrintToken( m_Token, m_pScan->GetTokenStringQ(0) );
		m_pScan->Clear();
		return NULL;
	}

	return m_pRmiList;
}


//	assignVOut -> VOut = exp
sExpr CExpressionParser::assignVOut()
{
	sExpr expr;


	return expr;
}


sExpr CExpressionParser::assignMOut()
{
	sExpr expr;

	return expr;
}


sExpr CExpressionParser::expr()
{
	sExpr expr;

	return expr;
}


sExpr CExpressionParser::term()
{
	sExpr expr;

	return expr;
}


sExpr CExpressionParser::factor()
{
	sExpr expr;

	return expr;
}


sExpr CExpressionParser::type()
{
	sExpr expr;

	return expr;
}



// rmi_list -> (rmi)*
sRmi* CExpressionParser::rmi_list()
{
	sRmi *p = rmi();
	sRmi *first = p;
	while (p && (p->next = rmi()))
	{
		p = p->next;
	}
	return first;
}

// rmi -> global id number '{' stmt_list '}'
sRmi* CExpressionParser::rmi()
{
	sRmi *p = NULL;
	
	if (PROTOCOL == m_Token)
	{
		Match(PROTOCOL);
		p = new sRmi;
		p->name = id();
		p->number = num();
		Match(LBRACE);
		p->protocol = stmt_list();
		Match(RBRACE);
	}

	return p;
}

// stmt_list -> (stmt)*
sProtocol* CExpressionParser::stmt_list()
{
	sProtocol *p = stmt();
	sProtocol *first = p;
	while (p && (p->next = stmt()))
	{
		p = p->next;
	}
	return first;
}

// stmt -> protocol semicolon
sProtocol* CExpressionParser::stmt()
{
	sProtocol *p = protocol();
	if (p)
		Match(SEMICOLON);
	return p;
}

// protocol -> id '(' arg_list ')'
sProtocol* CExpressionParser::protocol()
{
	sProtocol*p=NULL;
	if (ID == m_Token)
	{
		p = new sProtocol;
		p->name = id();
		Match(LPAREN);
		p->argList = arg_list();
		Match(RPAREN);
		p->next = NULL;
	}
	return p;
}


// arg_list -> [arg (',' arg)*]
sArg* CExpressionParser::arg_list()
{
	sArg *p = arg();
	if (!p)
		return NULL;

	sArg *first = p;
	while (COMMA == m_Token)
	{
		Match(COMMA);
		p->next = arg();
		p = p->next;
	}

	return first;
}

// arg -> type
sArg* CExpressionParser::arg()
{
	sArg *p = NULL;
	if (ID == m_Token)
	{
		p = new sArg;
		//p->var = type();
		p->next = NULL;
	}
	return p;
}

//// type -> type_sub (var)?
//sTypeVar* CExpressionParser::type()
//{
//	sTypeVar *p=NULL;
//	if (ID != m_Token)
//		return NULL;
//
//	Tokentype nextTok = m_pScan->GetTokenQ(1);
//	p = new sTypeVar;
//	p->type = type_sub();
//	p->var = var();
//
//	return p;
//}

// type_sub -> id '<' type_sub '>'
//			| id::id
//			| id
std::string CExpressionParser::type_sub()
{
	std::string str = "";

	if (ID == m_Token)
	{
		Tokentype nextTok = m_pScan->GetTokenQ(1);
		if (LT == nextTok)
		{
			str += id();
			str += "<";
			Match(LT);
			str += type_sub();
			str += ">";
			Match(RT);
		}
		else if (SCOPE == nextTok)
		{
			str += id();
			str += "::";
			Match(SCOPE);
			str += type_sub();
		}
		else
		{
			str += id();
		}
	}

	return str;
}

// var -> '*' id (index)?
//	    | '&' id (index)?
//		| id (index)?
//	    | '*'
//		| '&'
std::string CExpressionParser::var()
{
	std::string str = "";
	Tokentype nextTok = m_pScan->GetTokenQ(1);

	if (TIMES == m_Token && ID == nextTok)
	{
		Match(TIMES);
		str += "*";
		str += id();
		str += index();
	}
	else if (REF == m_Token && ID == nextTok)
	{
		Match(REF);
		str += "&";
		str += id();
		str += index();
	}
	else if (ID == m_Token)
	{
		str += id();
		str += index();
	}
	else if (TIMES == m_Token)
	{
		Match(TIMES);
		str += "*";
	}
	else if (REF == m_Token)
	{
		Match(REF);
		str += "&";
	}

	return str;
}

std::string CExpressionParser::index()
{
	std::string str = "";
	if (LBRACKET == m_Token)
	{
		Match(LBRACKET);
		str += "[";
		str += number();
		str += "]";
		Match(RBRACKET);
	}
	return str;
}

std::string CExpressionParser::number()
{
	std::string str = "";
	str = m_pScan->GetTokenStringQ(0);
	Match(NUM);
	return str;
}

int CExpressionParser::num()
{
	int n = atoi(m_pScan->GetTokenStringQ(0));
	Match(NUM);
	return n;
}

std::string CExpressionParser::id()
{
	std::string str = m_pScan->GetTokenStringQ(0);
	Match( ID );
	return str;
}

BOOL CExpressionParser::Match( Tokentype t )
{
	if( m_Token == t )
	{
		m_Token = m_pScan->GetToken();
	}
	else
	{
		SyntaxError( "unexpected token -> " );
		PrintToken( m_Token, m_pScan->GetTokenStringQ(0) );
		printf( "\n" );
	}
	return TRUE;
}


void CExpressionParser::SyntaxError( char *szMsg, ... )
{
	m_bError = TRUE;
	char buf[ 256];
	va_list marker;
	va_start(marker, szMsg);
	vsprintf_s(buf, sizeof(buf), szMsg, marker);
	va_end(marker);
 	printf( ">>>" );
 	printf( "Syntax error at line %s %d: %s", m_FileName, m_pScan->GetLineNo(), buf );
}

