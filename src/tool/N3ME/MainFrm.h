// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CN3EngTool;
class CMapMng;
class CDTexMng;
class CDTexGroupMng;

#include "DlgBar.h"
#include "DlgMapView.h"
#include "LyTerrainDef.h"
#include <list>

class CMainFrame : public CFrameWnd {
    friend class CDlgBar;

  protected: // create from serialization only
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    // Attributes
  public:
    CN3EngTool * m_pEng;
    CMapMng *    GetMapMng() const { return m_pMapMng; }

    CDTexMng *      GetDTexMng() const { return m_pDTexMng; }
    CDTexGroupMng * GetDTexGroupMng() const { return m_pDTexGroupMng; }

    void LoadDTexSet(const fs::path & fsFileName);

  protected:
    CMapMng * m_pMapMng;

    CDTexMng *      m_pDTexMng;
    CDTexGroupMng * m_pDTexGroupMng;

  public:
    CString       m_DTexInfoFileName;
    CDlgMapView * m_pDlgMapView;
    std::string   m_szZoneName;

    // Operations
  public:
  protected:
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
    //}}AFX_VIRTUAL

    // Implementation
  public:
    float GetFP() { return m_wndDlgBar.GetFP(); }
    int   GetTileRegion() { return m_wndDlgBar.GetTileRegion(); }
    void  UpdateTransformInfo();
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif

  protected: // control bar embedded members
    CStatusBar m_wndStatusBar;
    CToolBar   m_wndToolBar;
    CReBar     m_wndReBar;
    CDlgBar    m_wndDlgBar;

    // Generated message map functions
  protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnFileExport();
    afx_msg void OnCursorSelect();
    afx_msg void OnCursorObjBrush();
    afx_msg void OnCursorPos();
    afx_msg void OnCursorRotate();
    afx_msg void OnCursorScale();
    afx_msg void OnUpdateCursorSelect(CCmdUI * pCmdUI);
    afx_msg void OnUpdateCursorObjBrush(CCmdUI * pCmdUI);
    afx_msg void OnUpdateCursorPos(CCmdUI * pCmdUI);
    afx_msg void OnUpdateCursorRotate(CCmdUI * pCmdUI);
    afx_msg void OnUpdateCursorScale(CCmdUI * pCmdUI);
    afx_msg void OnViewOutputobject();
    afx_msg void OnUpdateViewOutputobject(CCmdUI * pCmdUI);
    afx_msg void OnViewSourceobject();
    afx_msg void OnUpdateViewSourceobject(CCmdUI * pCmdUI);
    afx_msg void OnViewSolid();
    afx_msg void OnUpdateViewSolid(CCmdUI * pCmdUI);
    afx_msg void OnViewWireframe();
    afx_msg void OnUpdateViewWireframe(CCmdUI * pCmdUI);
    afx_msg void OnEditProperty();
    afx_msg void OnUpdateEditProperty(CCmdUI * pCmdUI);
    afx_msg void OnCursorEditTerrain();
    afx_msg void OnUpdateCursorEditTerrain(CCmdUI * pCmdUI);
    afx_msg void OnTipFocusSelobj();
    afx_msg void OnUpdateTipFocusSelobj(CCmdUI * pCmdUI);
    afx_msg void OnTipFocusAll();
    afx_msg void OnUpdateTipFocusAll(CCmdUI * pCmdUI);
    afx_msg void OnTipDrop();
    afx_msg void OnUpdateTipDrop(CCmdUI * pCmdUI);
    afx_msg void OnTipDeleteObj();
    afx_msg void OnUpdateTipDeleteObj(CCmdUI * pCmdUI);
    afx_msg void OnCursorSmoothTerrain();
    afx_msg void OnUpdateCursorSmoothTerrain(CCmdUI * pCmdUI);
    afx_msg void OnFileImport();
    afx_msg void OnEditDtex();
    afx_msg void OnViewDtex();
    afx_msg void OnUpdateViewDtex(CCmdUI * pCmdUI);
    afx_msg void OnCursorDtex();
    afx_msg void OnUpdateCursorDtex(CCmdUI * pCmdUI);
    afx_msg void OnFileServerData();
    afx_msg void OnCursorEditRiver();
    afx_msg void OnUpdateCursorEditRiver(CCmdUI * pCmdUI);
    afx_msg void OnCursorEditPond();
    afx_msg void OnUpdateCursorEditPond(CCmdUI * pCmdUI);
    afx_msg void OnResourcePathSet();
    afx_msg void OnFileColormapLoad();
    afx_msg void OnFileColormapSave();
    afx_msg void OnFileGametiletexSave();
    afx_msg void OnTipGenerateColormap();
    afx_msg void OnTipGenerateColormapAll();
    afx_msg void OnUpdateTipGenerateColormap(CCmdUI * pCmdUI);
    afx_msg void OnTipTilingAll();
    afx_msg void OnUpdateTipTilingAll(CCmdUI * pCmdUI);
    afx_msg void OnTipDefaultMaterial();
    afx_msg void OnTipRemoveAlphaflag();
    afx_msg void OnUpdateTipDefaultMaterial(CCmdUI * pCmdUI);
    afx_msg void OnUpdateTipRemoveAlphaflag(CCmdUI * pCmdUI);
    afx_msg void OnCursorEditLightmap();
    afx_msg void OnUpdateCursorEditLightmap(CCmdUI * pCmdUI);
    afx_msg void OnCursorMakeNpcpath();
    afx_msg void OnUpdateCursorMakeNpcpath(CCmdUI * pCmdUI);
    afx_msg void OnViewSelectedObjectWireframe();
    afx_msg void OnUpdateViewSelectedObjectWireframe(CCmdUI * pCmdUI);
    afx_msg void OnViewHideObj();
    afx_msg void OnUpdateViewHideObj(CCmdUI * pCmdUI);
    afx_msg void OnViewAxisAndGrid();
    afx_msg void OnUpdateViewAxisAndGrid(CCmdUI * pCmdUI);
    afx_msg void OnCursorMakeWall();
    afx_msg void OnUpdateCursorMakeWall(CCmdUI * pCmdUI);
    afx_msg void OnCursorEditEvent();
    afx_msg void OnUpdateCursorEditEvent(CCmdUI * pCmdUI);
    afx_msg void OnFileImportHeight();
    afx_msg void OnTipDeleteUnusedFiles();
    afx_msg void OnTipDeleteOverlappedObjects();
    afx_msg void OnTipDeleteSelectedSourceObjects();
    afx_msg void OnViewOrth();
    afx_msg void OnCursorFlatTerrain();
    afx_msg void OnUpdateCursorFlatTerrain(CCmdUI * pCmdUI);
    afx_msg void OnViewToggleFog();
    afx_msg void OnCursorRegenUser();
    afx_msg void OnUpdateCursorRegenUser(CCmdUI * pCmdUI);
    afx_msg void OnTipSetWarpInfo();
    afx_msg void OnUpdateTipSetWarpInfo(CCmdUI * pCmdUI);
    afx_msg void OnFileSaveDivide();
    afx_msg void OnFileImportHeightBmp();
    afx_msg void OnFileExportHeightBmp();
    afx_msg void OnTipSetHeightScale();
    afx_msg void OnCursorSound();
    afx_msg void OnUpdateCursorSound(CCmdUI * pCmdUI);
    afx_msg void OnSelectMapEdit();
    afx_msg void OnCursorLightobj();
    afx_msg void OnUpdateCursorLightobj(CCmdUI * pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
