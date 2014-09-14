#pragma once

#include <string>

namespace parser 
{
	enum
	{
		MAX_STRING = 256,
		MAX_ARGUMENT = 10,
	};

	enum Tokentype
	{
		_ERROR, ENDFILE, ID, NUM, FNUM, STRING, ASSIGN, LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, COMMA, COLON, SEMICOLON,
		PLUS, MINUS, TIMES, DIV, REMAINDER, REF, ARROW,
		LT/* < */, RT/* > */, LTEQ/* <= */, RTEQ/* >= */, NEQ/* != */, EQ/* == */, OR/* || */, AND/* && */, NEG/* ! */, SCOPE/*::*/,
		VOUT, MOUT,
	};

	enum NodeKind { Stmt, Exp };
	enum Kind
	{
		ProgramK, TutorialK, RoleTutorialK, TrainingK, PracticeK, StatementK, AIK, EventK, UIEventK, 
		CustomEventK, SuccessK, FailK, AssignK,
		ScriptWaitK, ScriptEndK, ScriptExitK, // statement
		TypeK, ConditionOpK, ConditionTreeK, OpK, IdK, FuncK, ParamK, ConstIntK, ConstFloatK, ConstStrK, CallK, // exp
	};

	enum { MAXCHILD=8, };

	enum eCONDITION_OP { OP_NONE, OP_AND, OP_OR, OP_NEG, OP_LT, OP_RT, OP_LTEQ, OP_RTEQ, OP_EQ, OP_NEQ };


	struct sExpr
	{
		int type; // 0=matrix, 1=vector, 2=scalar
		float s;
		Matrix44 m;
		Vector3 v;

		sExpr operator+ (const sExpr &rhs);
		sExpr operator- (const sExpr &rhs);
		sExpr operator* (const sExpr &rhs);
		sExpr operator/ (const sExpr &rhs);
	};



	// Functions
	void PrintToken( Tokentype token, char *szTokenString );

	_variant_t	GetTypeStr2Type(const std::string &typeStr); // sArg->var->type

}

