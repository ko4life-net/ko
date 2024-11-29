#pragma once

// TexViewer.h : header file
//
#define UM_UPDATE_INFO ((WM_USER) + 101)
/////////////////////////////////////////////////////////////////////////////
// CTexViewer window
#define MAX_IMAGETYPE 200
class CN3Texture;
class CTexViewer : public CWnd {
    // Construction
  public:
    CTexViewer();

    // Attributes
  public:
    enum eEDITMODE {
        EDITMODE_SELECT = 0,
        EDITMODE_ZOOM,
        EDITMODE_HAND,
        NUM_EDITMODE
    };
    eEDITMODE GetEditMode() const { return m_eEditMode; }
    CRect     GetSelectedRect() const { return m_rcSelectedRect; }
    CSize     GetTexSize() const { return m_TexSize; }

  protected:
    CN3Texture * m_pTex;
    CSize        m_TexSize;
    float        m_fScale;
    CPoint       m_ptLeftTopInImage;
    eEDITMODE    m_eEditMode;
    CRect        m_rcSelectedRect;
    BOOL         m_bDrag;
    CPen         m_WhiteDashPen;
    CPoint       m_ptMouseOld;
    BOOL         m_bDeselect;
    CPoint       m_ptClickOffset;

    enum eDRAGTYPE {
        DRAGTYPE_NONE = 0,
        DRAGTYPE_MOVE,
        DRAGTYPE_LEFT,
        DRAGTYPE_RIGHT,
        DRAGTYPE_TOP,
        DRAGTYPE_BOTTOM,
        DRAGTYPE_LEFTTOP,
        DRAGTYPE_RIGHTTOP,
        DRAGTYPE_LEFTBOTTOM,
        DRAGTYPE_RIGHTBOTTOM,
        DRAGTYPE_SELECT
    };
    eDRAGTYPE m_eDragType; 

    HCURSOR m_hCursorSelect;
    HCURSOR m_hCursorZoomIn;
    HCURSOR m_hCursorZoomOut;
    HCURSOR m_hCursorHand;

    HCURSOR m_hCursorSizeAll;
    HCURSOR m_hCursorSizeWE;
    HCURSOR m_hCursorSizeNS;
    HCURSOR m_hCursorSizeNWSE;
    HCURSOR m_hCursorSizeNESW;


    int   m_iImageTypeCount;
    CRect m_ImageRects[MAX_IMAGETYPE];
    int   m_iCurSelectedImage;

    // Operations
  public:
    void      Release();
    BOOL      Zoom(BOOL bZoomIn);
    BOOL      Zoom(float fScale);
    void      Render();
    void      SetTexture(LPCTSTR pszFName);
    eEDITMODE SetEditMode(eEDITMODE eMode);
    void      SetLeftTopInImage(CPoint ptLeftTop);
    BOOL      GetSelectedUVRect(struct __FLOAT_RECT * pFRect) const;
    void      SetSelectedUVRect(const struct __FLOAT_RECT * pFRect);

    void  SetImageTypeCount(int iCount) { m_iImageTypeCount = iCount; }
    BOOL  SetImageTypeIndex(int iIndex);
    CRect GetImageRect(int iIndex);
    BOOL  AutoMultiRectSelect(BOOL bHorizon, CString & strErrMsg);

  protected:
    BOOL      ScreenToImage(POINT * pPoint);
    BOOL      ScreenToImage(RECT * pRect);
    BOOL      ImageToScreen(POINT * pPoint);
    BOOL      ImageToScreen(RECT * pRect);
    eDRAGTYPE CheckDragType(CRect rcSel, CPoint point);
    void      ProcessDrag(CPoint point);

  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTexViewer)
    //}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CTexViewer();

    // Generated message map functions
  protected:
    //{{AFX_MSG(CTexViewer)
    afx_msg BOOL OnEraseBkgnd(CDC * pDC);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.