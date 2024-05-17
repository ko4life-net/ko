#pragma once

// DlgSceneGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSceneGraph dialog
#include "Resource.h"
#include "N3Base/N3Base.h"

class CN3Scene;
class CMapMng;
class CDlgSceneGraph : public CDialog {
  public:
    CN3Scene * m_pSceneRef;
    BOOL       m_IsSourceObj; // Is it for display? (List of objects in the tool)
    CMapMng *  m_pMapMng;

    CImageList m_ImgList;

  protected:
    void  UpdateTreeItem(HTREEITEM hParent, CN3Base * pBase);
    DWORD m_dwFlag; // Flag indicating what to display in the list

  public:
    void SelectObject(HTREEITEM hItem, void * pItemData);

    void ExpandTree(HTREEITEM hItem);
    void UpdateTree(CN3Scene * pScene);
    // standard constructor
    CDlgSceneGraph(CWnd * pParent = NULL, DWORD dwFlag = OBJ_CAMERA | OBJ_LIGHT | OBJ_SHAPE | OBJ_CHARACTER | OBJ_MESH |
                                                         OBJ_MESH_PROGRESSIVE | OBJ_MESH_INDEXED | OBJ_JOINT |
                                                         OBJ_SKIN);

    // Dialog Data
    //{{AFX_DATA(CDlgSceneGraph)
    enum {
        IDD = IDD_SCENE_GRAPH
    };
    CTreeCtrl m_Tree;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSceneGraph)
  public:
    virtual BOOL PreTranslateMessage(MSG * pMsg);

  protected:
    virtual void DoDataExchange(CDataExchange * pDX); // DDX/DDV support
                                                      //}}AFX_VIRTUAL

    // Implementation
  protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgSceneGraph)
    afx_msg void OnSelchangedTreeObj(NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnDblclkTreeObj(NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnRclickTreeObj(NMHDR * pNMHDR, LRESULT * pResult);
    afx_msg void OnPaint();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
