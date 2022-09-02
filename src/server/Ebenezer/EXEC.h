// EXEC.h: interface for the EXEC class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class EXEC {
  public:
    void Init();
    BYTE m_Exec;
    int  m_ExecInt[MAX_EXEC_INT];

    void Parse(char * pBuf);
    EXEC();
    virtual ~EXEC();
};
