// DialogPart.h: interface for the CDialogPart class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CDialogPart : public CDialog {
  public:
    class CMainFrame *     m_pRefFrm;
    class CDlgEditScript * m_pRefBundle;

    CDialogPart();

    virtual ~CDialogPart();
};
