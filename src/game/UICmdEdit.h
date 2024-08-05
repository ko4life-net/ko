#pragma once
#include "GameDef.h"
#include "N3Base/N3UIBase.h"
#include "N3Base/N3UIString.h"
#include "N3Base/N3UIEdit.h"

//////////////////////////////////////////////////////////////////////

class CUICmdEdit : public CN3UIBase {
  public:
    CN3UIString * m_pText_Title;
    CN3UIButton * m_pBtn_Ok;
    CN3UIButton * m_pBtn_Cancel;
    CN3UIEdit *   m_pEdit_Box;
    std::string   m_szArg1;

  public:
    void SetVisible(bool bVisible) override;
    void Open(std::string msg);

    bool Load(HANDLE hFile);
    bool ReceiveMessage(CN3UIBase * pSender, DWORD dwMsg);

    CUICmdEdit();
    virtual ~CUICmdEdit();
};
