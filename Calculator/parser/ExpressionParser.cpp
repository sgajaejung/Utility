

#include "stdafx.h"
#include "ExpressionParser.h"

using namespace parser;

CExpressionParser::CExpressionParser() 
{
	m_pScan = new CExpressionScanner();
	m_pRmiList = NULL;
	m_bTrace = FALSE;
	m_bError = FALSE;

}

CExpressionParser::~CExpressionParser()
{
	SAFE_DELETE(m_pScan);
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

	if (VOUT == m_Token)
	{
		m_vector = assignVOut().v;
		m_mat = assignMOut().m;
	}
	else if (MOUT == m_Token)
	{
		m_mat = assignMOut().m;
		m_vector = assignVOut().v;
	}


	if( ENDFILE != m_Token )
	{
		SyntaxError( " code ends before file " );
		PrintToken( m_Token, m_pScan->GetTokenStringQ(0) );
		m_pScan->Clear();
		return NULL;
	}

	return m_pRmiList;
}


//---------------------------------------------------------------------
// 튜토리얼 스크립트를 파싱한다.
//---------------------------------------------------------------------
BOOL CExpressionParser::Parse( BYTE *pFileMem, int nFileSize , BOOL bTrace )//bTrace=FALSE
{
	if( !m_pScan->LoadPackageFile(pFileMem, nFileSize) )
		return FALSE;

	m_Token = m_pScan->GetToken();
	if( ENDFILE == m_Token )
	{
		m_pScan->Clear();
		return FALSE;
	}

	if (VOUT == m_Token)
	{
		m_vector = assignVOut().v;
		m_mat = assignMOut().m;
	}
	else if (MOUT == m_Token)
	{
		m_mat = assignMOut().m;
		m_vector = assignVOut().v;
	}


	if( ENDFILE != m_Token )
	{
		SyntaxError( " code ends before file " );
		PrintToken( m_Token, m_pScan->GetTokenStringQ(0) );
		m_pScan->Clear();
		return FALSE;
	}

	return TRUE;
}


//	assignVOut -> VOut = exp
sExpr CExpressionParser::assignVOut()
{
	Match(VOUT);
	Match(ASSIGN);

	sExpr exp = expr();
	return exp;
}


//	assignMOut -> MOut = exp
sExpr CExpressionParser::assignMOut()
{
	Match(MOUT);
	Match(ASSIGN);

	sExpr exp = expr();
	return exp;
}


//	expr -> term +/- expr | term
sExpr CExpressionParser::expr()
{
	sExpr exp1 = term();

	const Tokentype tok = m_Token;
	if ((PLUS == m_Token) || (MINUS == m_Token))
	{
		Match(m_Token);
	}
	else
	{
		return exp1;
	}

	sExpr exp2 = expr();

	sExpr ret;
	if (PLUS == tok)
	{
		ret = exp1 + exp2;
	}
	else if (MINUS == tok)
	{
		ret = exp1 - exp2;
	}

	return ret;
}


//	term -> factor *// term | term
sExpr CExpressionParser::term()
{
	sExpr exp1 = factor();

	const Tokentype tok = m_Token;
	if ((TIMES == m_Token) || (DIV == m_Token))
	{
		Match(m_Token);
	}
	else
	{
		return exp1;
	}

	sExpr exp2 = term();

	sExpr ret;
	if (TIMES == tok)
	{
		ret = exp1 * exp2;
	}
	else
	{
		ret = exp1 / exp2;
	}

	return ret;
}


//	factor -> (expr) | type
sExpr CExpressionParser::factor()
{
	sExpr ret;

	if (LPAREN == m_Token)
	{
		Match(LPAREN);
		ret = expr();
		Match(RPAREN);
	}
	else
	{
		ret = type();
	}
	
	return ret;
}


//	type -> Vertex | Translate | Rotate | Scale | num
sExpr CExpressionParser::type()
{
	sExpr ret;

	if (ID == m_Token)
	{
		string str = m_pScan->GetTokenStringQ(0);
		if (str == "V")
		{
			const Vector3 v = num3();
			ret.type = 1;
			ret.v = v;
		}
		else if (str == "T")
		{
			const Vector3 v = num3();
			ret.type = 0;
			ret.m.SetTranslate(v);
		}
		else if (str == "S")
		{
			const Vector3 v = num3();
			ret.type = 0;
			ret.m.SetScale(v);
		}
		else if (str == "Rx")
		{
			Match(ID);
			Match(LPAREN);
			ret.type = 0;
			const float v = fnum();
			ret.m.SetRotationX(v);
			Match(RPAREN);
		}
		else if (str == "Ry")
		{
			Match(ID);
			Match(LPAREN);
			ret.type = 0;
			const float v = fnum();
			ret.m.SetRotationY(v);
			Match(RPAREN);
		}
		else if (str == "Rz")
		{
			Match(ID);
			Match(LPAREN);
			ret.type = 0;
			const float v = fnum();
			ret.m.SetRotationZ(v);
			Match(RPAREN);
		}

	}
	else if (NUM == m_Token)
	{
		const string str = m_pScan->GetTokenStringQ(0);
		ret.type = 2;
		ret.s = (float)atof(str.c_str());
	}

	return ret;
}


// num3 -> id( num, num, num )
Vector3 CExpressionParser::num3()
{
	Vector3 ret;

	Match(ID);
	Match(LPAREN);
	ret.x = fnum();
	Match(COMMA);
	ret.y = fnum();
	Match(COMMA);
	ret.z  = fnum();
	Match(RPAREN);

	return ret;
}



int CExpressionParser::num()
{
	int n = atoi(m_pScan->GetTokenStringQ(0));
	Match(NUM);
	return n;
}

float CExpressionParser::fnum()
{
	float f = (float)atof(m_pScan->GetTokenStringQ(0));
	Match(NUM);
	return f;
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

