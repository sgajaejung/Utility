#pragma once

#include "ExpressionScanner.h"

namespace parser
{
	class CExpressionScanner;
	class CExpressionParser
	{
	public:
		CExpressionParser();
		virtual ~CExpressionParser();

		sRmi* Parse( const char *szFileName, BOOL bTrace=FALSE );
		BOOL Parse( BYTE *pFileMem, int nFileSize , BOOL bTrace=FALSE );
		BOOL IsError() { return m_bError; }
		void SetAutoRemove(BOOL bAutoRemove) { m_bAutoRemove = bAutoRemove; }
		void Clear();


	private:
		BOOL Match( Tokentype t );
		void SyntaxError( char *szMsg, ... );

		//	assignVOut -> VOut = exp
		//	assignMOut -> MOut = exp

		//	exp -> factor +/- term
		//	term -> factor *// term
		//	factor -> (exp) | type
		//	type -> Vertex | Translate | Rotate | Scale | num

		// Vertex -> V( num, num, num )
		//	Translate -> T( num, num, num )
		//	RotateX -> Rx( num )
		//	RotateY -> Ry( num )
		//	RotateZ -> Rz( num )
		//	Scale -> S( num, num, num )

		sExpr assignVOut();
		sExpr assignMOut();
		sExpr expr();
		sExpr term();
		sExpr factor();
		sExpr type();




		// expr -> rmi_list
		// rmi_list -> (rmi)*
		// rmi -> protocol id number '{' stmt_list '}'
		// stmt_list -> (stmt)*
		// stmt -> protocol semicolon
		// protocol -> id '(' arg_list ')'
		// arg_list -> [arg (',' arg)*]
		// arg -> type
		// type -> type_sub (var)?
		// type_sub -> id '<' type_sub '>'
		//			| id::id
		//			| id
		// var -> '*' id (index)?
		//	    | '&' id (index)?
		//		| id (index)?
		//	    | '*'
		//		| '&'
		// index -> '[' (number)? ']'

		sRmi* rmi_list();
		sRmi* rmi();
		sProtocol* stmt_list();
		sProtocol* stmt();
		sProtocol* protocol();
		sArg* arg_list();
		sArg* arg();
		//sTypeVar* type();
		std::string type_sub();
		std::string var();
		std::string index();
		std::string number();
		int num();
		std::string id();

	private:
		CExpressionScanner *m_pScan;
		sRmi	 *m_pRmiList;
		char m_FileName[ MAX_PATH];
		Tokentype m_Token;
		BOOL m_bTrace;
		BOOL m_bError;
		BOOL m_bAutoRemove;

	};
}