#pragma once

#include <list>
#include <deque>
#include "ExpressionDefine.h"

namespace parser
{
	class CExpressionScanner
	{
	public:
		CExpressionScanner();
		virtual ~CExpressionScanner();

	protected:
		enum { MAX_QSIZE=5, MAX_BUFF=256, MAX_TOKENLEN=256 };
		enum StateType { START, INASSIGN, INCOMMENT, INMULTI_COMMENT, OUTMULTI_COMMENT, INNUM, INID, INSTR, INDIV, INEQ, INNEQ, INOR, INAND, 
			INLTEQ, INRTEQ, INARROW, INSCOPE, DONE };

		typedef struct _tagSTokDat
		{
			std::string str;
			Tokentype tok;
		} STokDat;

		BYTE *m_pFileMem;
		int m_pCurrentMemPoint;
		int m_nMemSize;

		int m_nLineNo;
		int m_nLinePos;
		int m_nBufSize;
		char m_Buf[ MAX_BUFF];
		std::deque<STokDat> m_TokQ;
		BOOL m_bTrace;


	public:
		BOOL LoadFile( const char *szFileName, BOOL bTrace=FALSE );
		BOOL LoadPackageFile( BYTE *pFileMem, int nFileSize );

		Tokentype GetToken();
		Tokentype GetTokenQ( int nIdx );
		char* GetTokenStringQ( int nIdx );
		char* CopyTokenStringQ( int nIdx );
		int GetLineNo() { return m_nLineNo; }
		BOOL IsEnd();
		void Clear();


	protected:
		void Init();
		char GetNextChar();
		void UngetNextChar();
		Tokentype _GetToken( char *pToken );

		BOOL GetString(char *receiveBuffer, int maxBufferLength);

	};
}
