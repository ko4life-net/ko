#if !defined(AFX_ORGANIZEVIEW_H__57A9D1DE_2B50_41D4_9B08_8D48CE641331__INCLUDED_)
#define AFX_ORGANIZEVIEW_H__57A9D1DE_2B50_41D4_9B08_8D48CE641331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrganizeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COrganizeView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MainFrm.h"
#include "PVSManager.h"

class COrganizeView : public CFormView
{
	friend class CMainFrame;

// Generic Private Data..
	int	m_iSelectionIndex;	
	int	m_iSelectionShapeIndex;
	int m_iSelectionLinkedShapeIndex;

// Imagelist for small icons
	CImageList m_GlobalImageList;

	int iTempOffs;

protected:
	COrganizeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COrganizeView)

// Form Data
public:
	//{{AFX_DATA(COrganizeView)
	enum { IDD = IDD_PROPERTY };
	CComboBox	m_ComboFloor;
	CListCtrl	m_ShapesListCtrl;
	CListCtrl	m_LinkedListCtrl;
	CListCtrl	m_LinkToListCtrl;
	CListCtrl	m_SelectedListCtrl;
	//}}AFX_DATA

// Attributes
public:
	//..
	CPVSManager	m_PVSMgr;

// Operations
public:
	void	InitEnvironSetting();
	void	RefreshSelectedList();	
	void	RefreshLinkToList();
	void    RefreshLinkedList();
	void	RefreshTotalShape();
	void	OnCreatePortalGateWrapper(e_PvsWallType ePWT);

	//..
	void	OnCurserSelect(e_EditMode eED);
	void	SelectVolumeByPick(CPortalVolume* pVol, bool bOne = true);
	void	SelectObjectByDragRect(RECT* pRect, BOOL bAdd);	// 드래그 해서 객체 선택하기

	void    TickRender();

	bool	OnFileNew();
	void	OnFileWorkshopOpen();
	void	OnFileOpenGamedata();
	void	OnFileSaveWorkshop();
	void	OnFileSaveGamedata();

	void	OnFileServerData();

	void	OpenWorkShopFile(std::string strFile);
	void	OpenGameDataFile(std::string strFile);

	void	OpenWSFileInternal(std::string strFile);
	void	OpenGDFileInternal(std::string strFile);

	void	ShapeLinkToPVolumn(CN3Base* pBase);
	bool	IsSelectedPvs();
	void    ShapeRegisterToManager(CN3Base* pBase);
	ShapeInfo* GetNthShapeInfo(int iOrder);

	bool	QueryHeight(float fx, float fz, float& fy);
	void	ExecuteCameraChange();
	void	RestoreExecuteCameraChange();

	void	SelectNextShape();
	ShapeInfo* GetSelectedTotalShape();		

	void	UpdatePosAll(int iX, int iY, int iZ);

	// Floor..
	int		  GetCurFloor();
	void	RefreshFloorCombo();
	void	RefreshFloorInfoAll(int iID);

	// Server Map Data..
	int		  GetTerrainSize();
	void	WriteTerrainData(HANDLE hFile, int iMax, int iFloor);
	float	 GetFloorHeight(int iFloor, int iX, int iZ);
	void	WriteObjectEventData(HANDLE hFile, int iFloor);
	void	WriteCollisionData(HANDLE hFile, int iMax);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrganizeView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COrganizeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(COrganizeView)
	afx_msg void OnButtonResourcePath();
	afx_msg void OnEdit();
	afx_msg void OnCompile();
	afx_msg void OnActive();
	afx_msg void OnStatePvolumn();
	afx_msg void OnAddShape();
	afx_msg void OnAddCollision();
	afx_msg void OnBtnCreateVol();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnCreateGate1();
	afx_msg void OnBtnCreateGate2();
	afx_msg void OnBtnCreateGate3();
	afx_msg void OnBtnCreateGate4();
	afx_msg void OnBtnCreateGate5();
	afx_msg void OnBtnCreateGate6();
	afx_msg void OnBtnLinkedDelete();
	afx_msg void OnItemchangingListSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFileName();
	afx_msg void OnBtnShapeDelete();
	afx_msg void OnItemchangingListShapes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangingListLinked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusListShapes(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusListLinked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStateShape();
	afx_msg void OnIndiPick();
	afx_msg void OnIndiMove();
	afx_msg void OnIndiRotate();
	afx_msg void OnIndiScale();
	afx_msg void OnViewSelectedTotalShape();
	afx_msg void OnChangeTotalMoveX();
	afx_msg void OnChangeTotalMoveY();
	afx_msg void OnChangeTotalMoveZ();
	afx_msg void OnSetFloor();
	afx_msg void OnDeleteFloor();
	afx_msg void OnCheckFloor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORGANIZEVIEW_H__57A9D1DE_2B50_41D4_9B08_8D48CE641331__INCLUDED_)
