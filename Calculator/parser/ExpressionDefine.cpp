
#include "stdafx.h"
#include "ExpressionDefine.h"
//#include "../Error/ErrorCode.h"


using namespace parser;
//using namespace marshalling;

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void parser::PrintToken( Tokentype token, char *szTokenString )
{
	switch( token )
	{
	case ID:
	case STRING:	printf( "string = %s\n", szTokenString ); break;

	case ASSIGN:	printf( "=" ); break;
	case TIMES:		printf( "*" ); break;
	case LPAREN:	printf( "(" ); break;
	case RPAREN:	printf( ")" ); break;
	case LBRACE:	printf( "{" ); break;
	case RBRACE:	printf( "}" ); break;
	case COMMA:	printf( "," ); break;
	case NUM:		printf( "number" ); break;
	case OR:		printf( "||" ); break;
	case AND:		printf( "&&" ); break;

	default: 		printf( "UnKnown token %d, %s\n", token, szTokenString ); break;
	}
}


//------------------------------------------------------------------------
// typeStr: sArg->var->type
// 스트링을 타입으로 리턴한다.
//------------------------------------------------------------------------
_variant_t parser::GetTypeStr2Type(const std::string &typeStr)
{
	if (typeStr == "std::string")
	{
		char *v=NULL;
		return _variant_t(v);
	}
	else if (typeStr == "string")
	{
		char *v=NULL;
		return _variant_t(v);
	}
	else if (typeStr == "float")
	{
		float v=0.f;
		return _variant_t(v);
	}
	else if (typeStr == "double")
	{
		double v=0.f;
		return _variant_t(v);
	}
	else if (typeStr == "int")
	{
		int v=0;
		return _variant_t(v);
	}
	else if (typeStr == "char")
	{
		char v='a';
		return _variant_t(v);
	}
	else if (typeStr == "short")
	{
		short v=0;
		return _variant_t(v);
	}
	else if (typeStr == "long")
	{
		long v=0;
		return _variant_t(v);
	}
	else if (typeStr == "bool")
	{
		bool v=true;
		return _variant_t(v);
	}
	else if (typeStr == "BOOL")
	{
		bool v=true;
		return _variant_t(v);
	}

	return _variant_t(1);
}



sExpr sExpr::operator+ (const sExpr &rhs)
{
	sExpr ret;

	if (type == rhs.type)
	{
		switch (type)
		{
		case 0: //matrix
			// error
			ret.type = 0;
			break;

		case 1: // vertex
			ret.type = 1;
			ret.v = v + rhs.v;
			break;

		case 2: // scalar
			ret.type = 2;
			ret.s = s + rhs.s;
			break;
		}
	}
	else
	{
		// error
	}

	return ret;
}


sExpr sExpr::operator- (const sExpr &rhs)
{
	sExpr ret;

	if (type == rhs.type)
	{
		switch (type)
		{
		case 0: //matrix
			// error
			ret.type = 0;
			break;

		case 1: // vertex
			ret.type = 1;
			ret.v = v - rhs.v;
			break;

		case 2: // scalar
			ret.type = 2;
			ret.s = s - rhs.s;
			break;
		}
	}
	else
	{
		// error
	}

	return ret;
}


sExpr sExpr::operator* (const sExpr &rhs)
{
	sExpr ret;

	switch (type)
	{
	case 0: // matrix
		switch (rhs.type)
		{
		case 0: // matrix * matrix
			ret.type = 0;
			ret.m = m * rhs.m;
			break;

		case 1: // matrix * vertex -> error
			break;

		case 2: // matrix * scalar -> error
			break;
		}
		break;

	case 1: // vector
		switch (rhs.type)
		{
		case 0: // vector * matrix
			ret.type = 1;
			ret.v = v * rhs.m;
			break;

		case 1: // vector * vector -> error
			break;

		case 2: // vector * scalar
			ret.type = 1;
			ret.v = v * rhs.s;
			break;
		}
		break;

	case 2: // scalar
		switch (rhs.type)
		{
		case 0: // scalar * matrix -> error				
			break;

		case 1: // scalar * vector
			ret.type = 1;
			ret.v = rhs.v * s;
			break;

		case 2: // scalar * scalar
			ret.type = 2;
			ret.s = s * rhs.s;
			break;
		}
		break;
	}

	return ret;
}


sExpr sExpr::operator/ (const sExpr &rhs)
{
	sExpr ret;

	switch (type)
	{
	case 0: // matrix
		switch (rhs.type)
		{
		case 0: // matrix / matrix -> error
			break;

		case 1: // matrix / vertex -> error
			break;

		case 2: // matrix / scalar -> error
			break;
		}
		break;

	case 1: // vector
		switch (rhs.type)
		{
		case 0: // vector / matrix -> error
			break;

		case 1: // vector / vector -> error
			break;

		case 2: // vector / scalar
			break;
		}
		break;

	case 2: // scalar
		switch (rhs.type)
		{
		case 0: // scalar / matrix -> error				
			break;

		case 1: // scalar / vector -> error
			break;

		case 2: // scalar / scalar
			ret.type = 2;
			ret.s = s / rhs.s;
			break;
		}
		break;
	}

	return ret;
}
