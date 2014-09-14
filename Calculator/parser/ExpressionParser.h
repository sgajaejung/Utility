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

		BOOL Parse( BYTE *pFileMem, int nFileSize , BOOL bTrace=FALSE );
		BOOL IsError() { return m_bError; }
		void Clear();

		Matrix44 m_mat;
		Vector3 m_vector;


	private:
		BOOL Match( Tokentype t );
		void SyntaxError( char *szMsg, ... );

		//	assignVOut -> VOut = exp
		//	assignMOut -> MOut = exp

		//	expr -> term +/- expr | term
		//	term -> factor *// term | factor
		//	factor -> (expr) | type
		//	type -> Vertex | Translate | Rotate | Scale | num

		// Vertex -> V( num, num, num )
		//	Translate -> T( num, num, num )
		//	RotateX -> Rx( num )
		//	RotateY -> Ry( num )
		//	RotateZ -> Rz( num )
		//	Scale -> S( num, num, num )
		// num3 -> id( num, num, num )

		sExpr assignVOut();
		sExpr assignMOut();
		sExpr expr();
		sExpr term();
		sExpr factor();
		sExpr type();
		Vector3 num3();

		int num();
		float fnum();
		std::string id();


	private:
		CExpressionScanner *m_pScan;
		char m_FileName[ MAX_PATH];
		Tokentype m_Token;
		BOOL m_bTrace;
		BOOL m_bError;
	};
}