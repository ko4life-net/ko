// ProgressBar.cpp : implementation file
//

#include "StdAfx.h"
#include "n3me.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressBar

CProgressBar::CProgressBar() {
    m_nSize = 0;
}

CProgressBar::~CProgressBar() {
    Clear();
}

BEGIN_MESSAGE_MAP(CProgressBar, CProgressCtrl)
//{{AFX_MSG_MAP(CProgressBar)
ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressBar message handlers

BOOL CProgressBar::Create(LPCTSTR strMessage, int nSize, int MaxValue) {
    if (NULL == m_hWnd) {
        // get the status bar
        CStatusBar * pStatusBar = GetStatusBar();
        if (pStatusBar == NULL) {
            return FALSE;
        }

        // Create a progress control on the status bar
        if (!CProgressCtrl::Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pStatusBar, 1)) {
            return FALSE;
        }
    }

    // Set the range and steps of progress control
    SetRange(0, MaxValue);
    SetStep(1);

    m_strMessage = strMessage;
    m_nSize = nSize;

    // Adjust position and size
    Resize();
    return TRUE;
}

CStatusBar * CProgressBar::GetStatusBar() {
    CFrameWnd * pFrame = (CFrameWnd *)AfxGetMainWnd();
    if (pFrame == NULL || pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)) == FALSE) {
        return NULL;
    }

    CStatusBar * pBar = (CStatusBar *)pFrame->GetMessageBar();
    if (pBar == NULL || pBar->IsKindOf(RUNTIME_CLASS(CStatusBar)) == FALSE) {
        return NULL;
    }

    return pBar;
}

void CProgressBar::Resize() {
    CStatusBar * pStatusBar = GetStatusBar();
    if (pStatusBar == NULL) {
        return;
    }

    // text output
    if (::IsWindow(m_hWnd) && IsWindowVisible()) {
        pStatusBar->SetWindowText(m_strMessage);
        pStatusBar->UpdateWindow();
    }

    // Calculate the area occupied by the text
    CClientDC dc(pStatusBar);
    CFont *   pOldFont = dc.SelectObject(pStatusBar->GetFont());
    CSize     size = dc.GetTextExtent(m_strMessage);
    int       margin = dc.GetTextExtent(_T(" ")).cx * 2;
    dc.SelectObject(pOldFont);

    // Calculate the area where the progress control will be drawn
    CRect rc;
    pStatusBar->GetItemRect(0, rc);
    rc.left = size.cx + 2 * margin;
    rc.right = rc.left + (rc.right - rc.left) * m_nSize / 100;
    if (rc.right < rc.left) {
        rc.right = rc.left;
    }

    // Leave 10% margin at the top and bottom of the status bar
    int Height = rc.bottom - rc.top;
    rc.bottom -= Height / 10;
    rc.top += Height / 10;

    // Readjust the position and size of the progress control
    if (::IsWindow(m_hWnd) && (rc != m_Rect)) {
        MoveWindow(&rc);
    }
    m_Rect = rc;
}

void CProgressBar::Clear() {
    ModifyStyle(WS_VISIBLE, 0);
    CString str;
    str.LoadString(AFX_IDS_IDLEMESSAGE);
    CStatusBar * pStatusBar = GetStatusBar();
    if (pStatusBar) {
        pStatusBar->SetWindowText(str);
    }
}

BOOL CProgressBar::OnEraseBkgnd(CDC * pDC) {
    Resize();

    return CProgressCtrl::OnEraseBkgnd(pDC);
}
