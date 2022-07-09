// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3EA7AF1D_2BD0_44CA_A957_189BFB0D0B38__INCLUDED_)
#define AFX_MAINFRM_H__3EA7AF1D_2BD0_44CA_A957_189BFB0D0B38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../N3Base/N3EngTool.h"
#include "../N3Base/N3Camera.h"
#include "../N3Base/N3Light.h"
#include "../N3Base/N3Shape.h"

#include "PortalVolume.h"

#include "PosDummy.h"
#include "RotDummy.h"
#include "ScaleDummy.h"
#include "SwappedDummy.h"
#include <string>

class CDlgBase;
class CTotalToolSheet;
class CDlgShapeList;
class CN3Scene;
class CN3IndoorView;
class COrganizeView;

enum e_State { STATE_EDIT = 0, STATE_COMPILE, STATE_EXECUTE };                               
enum e_SelectState { SELECT_STATE_PVOLUMN = 0, SELECT_STATE_SHAPE };
enum e_ShapeManage { SHAPEMANAGE_STATE_ONLYONE = 0, SHAPEMANAGE_STATE_PERPVOLUMN, SHAPEMANAGE_STATE_COMPLEX };

const DWORD dw_Render_None = 0x00000000;
const DWORD dw_Render_Add_Shape = 0x00000001;
const DWORD dw_Render_Add_Collision = 0x0000002;

enum e_EditMode { 	EDIT_SELECT = 0,	EDIT_MOVE, EDIT_ROTATE, EDIT_SCALE	 };

const __Vector3 cvInitEyeOffset(0, 100, -40);
const int ciVersion = 1;

class CMainFrame : public CFrameWnd, public CN3Base
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// States..
public:
	e_State						m_eState;								// 상태 변수들..
	e_SelectState			m_eSelectState;
	DWORD					m_dwRenderingOption;
	e_EditMode				m_eEditState;

	std::string					m_strResourcePath;
	std::string					m_strFileName;

	bool						m_bViewSelectedTotalShape;
	bool						m_bViewSelectedFloor;

// Attributes
public:
	CSplitterWnd			m_wndSplitter;
	
// Operations
public:
	CN3EngTool				m_Eng;
	CN3Camera				m_Camera;
	CN3Light					m_Light;
	CDlgBase*				m_pDlgBase;						// 객체 등록정보 편집 대화상자..
	CTotalToolSheet*	m_pTotalSheet;
	CDlgShapeList*		m_pDlgSourceList;				// Object 목록을 보여줄 다이알로그
	CDlgShapeList*		m_pDlgOutputList;				 // 맵에 배치한 Object를 보여줄 다이알로그
	CN3Scene*			m_pSceneSource;				   // source object 목록에 보여줄 것들을 담은 Scene
	SelectElement		m_LastSelectedElement;		 // 가장 마지막으로 선택된 객체..	포커스 맞출때 쓴다..

	//.................
	CArray<CPortalVolume*, CPortalVolume*> m_SelVolArray;
	std::list<FloorInfo>	m_FloorList;
	CTransDummy*	   m_pDummy;						  // 물체를 이동 회전 확대/축소 하는 기능을 담당하는 클래스
	CPosDummy			 m_PosDummy;						 // 이동
	CRotDummy			  m_RotDummy;							// 회전
	CScaleDummy			m_ScaleDummy;					// 확대/축소
	CSwappedDummy	m_SwappedDummy;
	//..................

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	FindMinMaxTotalShape(__Vector3 &vecMin, __Vector3 &vecMax);
	void	OutputDlgRefresh();	// 소스목록에서 선택한 Object를 넣으면 OutputScene으로 복사해서 넣어준다.
	CN3Transform* AddChr(CN3Scene* pDestScene, const std::string& szFN, BOOL bGenerateChainNumber);		// 특정Scene에 캐릭터 객체를 복사해 추가
	CN3Transform* AddShape(CN3Scene* pDestScene, const std::string& szFN, BOOL bGenerateChainNumber);	// 특정Scene에 Shape 객체를 복사해 추가

	void	UpdateShapeInfoDisplay();
	void	RenderObjectToWindow(CN3TransformCollision* pObj, HWND hWnd);	// 특정 윈도우에 Object를 그려준다.
	ShapeInfo* GetShapeForDisplay();
	void	TotalValidateCheckAfterDelete();

	// Scene Source..
	void	LoadSourceObjects();						// Folder 에서 Source Object 를 읽어온다.
	void	RefreshSourceObjects();

	void SetFPSString(LPCTSTR pStr);
	COrganizeView* GetOrganizeView();
	CN3IndoorView* GetIndoorView();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnFileNew();
	afx_msg void OnFileWorkshopOpen();
	afx_msg void OnFileOpenGamedata();
	afx_msg void OnFileSaveWorkshop();
	afx_msg void OnFileSaveGamedata();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnViewSourceobject();
	afx_msg void OnUpdateViewSourceobject(CCmdUI* pCmdUI);
	afx_msg void OnCursorSelect();
	afx_msg void OnCursorPos();
	afx_msg void OnCursorRotate();
	afx_msg void OnCursorScale();
	afx_msg void OnTipFocusSelobj();
	afx_msg void OnTipFocusAll();
	afx_msg void OnTipDeleteObj();
	afx_msg void OnUpdateCursorSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCursorPos(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCursorRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCursorScale(CCmdUI* pCmdUI);
	afx_msg void OnTipNextShape();
	afx_msg void OnFileMruFile1();
	afx_msg void OnFileMruFile2();
	afx_msg void OnFileMruFile3();
	afx_msg void OnFileMruFile4();
	afx_msg void OnViewOutputobject();
	afx_msg void OnUpdateViewOutputobject(CCmdUI* pCmdUI);
	afx_msg void OnEditProperty();
	afx_msg void OnUpdateEditProperty(CCmdUI* pCmdUI);
	afx_msg void OnTipDeleteUnusedFiles();
	afx_msg void OnTotalPropertyTool();
	afx_msg void OnUpdateTotalPropertyTool(CCmdUI* pCmdUI);
	afx_msg void OnFileServerData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3EA7AF1D_2BD0_44CA_A957_189BFB0D0B38__INCLUDED_)
