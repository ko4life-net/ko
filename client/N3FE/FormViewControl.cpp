// FormViewControl.cpp : implementation file
//

#include "stdafx.h"
#include "N3FE.h"
#include "MainFrm.h"
#include "FormViewControl.h"
#include "N3FEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewControl

IMPLEMENT_DYNCREATE(CFormViewControl, CFormView)

CFormViewControl::CFormViewControl()
	: CFormView(CFormViewControl::IDD)
{
	//{{AFX_DATA_INIT(CFormViewControl)
	//}}AFX_DATA_INIT
}

CFormViewControl::~CFormViewControl()
{
}

void CFormViewControl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormViewControl)
	DDX_Control(pDX, IDC_LIST_PART, m_PartList);
	DDX_Control(pDX, IDC_LIST_BUNDLE, m_BundleList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormViewControl, CFormView)
	//{{AFX_MSG_MAP(CFormViewControl)
	ON_BN_CLICKED(IDC_B_ADD, OnBAdd)
	ON_BN_CLICKED(IDC_B_DELETE, OnBDelete)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_B_UPDATE_EFFECT, OnBUpdateEffect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_B_TRIGGER, OnBTrigger)
	ON_BN_CLICKED(IDC_B_STOP, OnBStop)
	ON_EN_CHANGE(IDC_E_PART_SCRIPT, OnChangeEPartScript)
	ON_EN_CHANGE(IDC_E_BUNDLE_SCRIPT, OnChangeEBundleScript)
	ON_LBN_SELCHANGE(IDC_LIST_BUNDLE, OnSelchangeListBundle)
	ON_LBN_SELCHANGE(IDC_LIST_PART, OnSelchangeListPart)
	ON_BN_CLICKED(IDC_B_BUNDLE_ADD, OnBBundleAdd)
	ON_BN_CLICKED(IDC_B_BUNDLE_INSERT, OnBBundleInsert)
	ON_BN_CLICKED(IDC_B_BUNDLE_DELETE, OnBBundleDelete)
	ON_BN_CLICKED(IDC_B_PART_ADD, OnBPartAdd)
	ON_BN_CLICKED(IDC_B_PART_DELETE, OnBPartDelete)
	ON_BN_CLICKED(IDC_B_PART_INSERT, OnBPartInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormViewControl diagnostics

#ifdef _DEBUG
void CFormViewControl::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewControl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CN3FEDoc* CFormViewControl::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CN3FEDoc)));
	return (CN3FEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormViewControl message handlers

void CFormViewControl::OnBAdd() 
{
}

void CFormViewControl::OnBDelete() 
{
}

void CFormViewControl::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CWnd* pWnd = GetDlgItem(IDC_LIST_BUNDLE);
	if(NULL != pWnd->GetSafeHwnd())
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		this->ScreenToClient(&rc);
		pWnd->SetWindowPos(NULL, 0, 0, rc.Width(), cy - rc.top - 4, SWP_NOMOVE | SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_E_BUNDLE_SCRIPT);
	if(NULL != pWnd->GetSafeHwnd())
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		this->ScreenToClient(&rc);
		pWnd->SetWindowPos(NULL, 0, 0, rc.Width(), cy - rc.top - 4, SWP_NOMOVE | SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_LIST_PART);
	if(NULL != pWnd->GetSafeHwnd())
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		this->ScreenToClient(&rc);
		pWnd->SetWindowPos(NULL, 0, 0, rc.Width(), cy - rc.top - 4, SWP_NOMOVE | SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_E_PART_SCRIPT);
	if(NULL != pWnd->GetSafeHwnd())
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		this->ScreenToClient(&rc);
		pWnd->SetWindowPos(NULL, 0, 0, cx - rc.left - 4, cy - rc.top - 4, SWP_NOMOVE | SWP_NOZORDER);
	}
}

void CFormViewControl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CN3FEDoc* pDoc = GetDocument();
	
	SetDlgItemText(IDC_E_PATH, pDoc->m_EffectMgr.PathGet().c_str());
	
	if(m_BundleList.GetSafeHwnd())
	{
		int nPrevSel = m_BundleList.GetCurSel();
		m_BundleList.ResetContent();
		for(int i = 0; i < pDoc->m_EffectMgr.BundleCount(); i++)
		{
			__EffectBundle* pEB = pDoc->m_EffectMgr.Bundle(i);
			m_BundleList.AddString(pEB->szName);
			m_BundleList.SetItemData(i, (DWORD)pEB);
		}
		m_BundleList.SetCurSel(nPrevSel);
	}

	if(m_PartList.GetSafeHwnd())
	{
		int nPrevSel = m_PartList.GetCurSel();
		m_PartList.ResetContent();
		for(int i = 0; i < pDoc->m_EffectMgr.PartCount(); i++)
		{
			__EffectPartBase* pEPB = pDoc->m_EffectMgr.Part(i);
			m_PartList.AddString(pEPB->szName);
			m_PartList.SetItemData(i, (DWORD)pEPB);
		}
		m_PartList.SetCurSel(nPrevSel);
	}

	this->UpdateInfo();
}

void CFormViewControl::UpdateInfo()
{
	CN3FEDoc* pDoc = GetDocument();

	if(m_BundleList.GetSafeHwnd())
	{
		int nB = m_BundleList.GetCurSel();
		if(nB >= 0 && nB < pDoc->m_szBundleScripts.GetSize())
		{
			SetDlgItemText(IDC_E_BUNDLE_SCRIPT, pDoc->m_szBundleScripts[nB]);
		}
		else SetDlgItemText(IDC_E_BUNDLE_SCRIPT, "");
	}

	if(m_PartList.GetSafeHwnd())
	{
		int nP = m_PartList.GetCurSel();
		if(nP >= 0 && nP < pDoc->m_szPartScripts.GetSize())
		{
			SetDlgItemText(IDC_E_PART_SCRIPT, pDoc->m_szPartScripts[nP]);
		}
		else SetDlgItemText(IDC_E_PART_SCRIPT, "");
	}
}

void CFormViewControl::OnBUpdateEffect() 
{
	CN3FEDoc* pDoc = GetDocument();

	int nB = m_BundleList.GetCurSel();
	if(nB >= 0 && nB < pDoc->m_szBundleScripts.GetSize())
	{
		__EffectBundle* pEB = pDoc->m_EffectMgr.Bundle(nB);
		GetDlgItemText(IDC_E_BUNDLE_SCRIPT, pDoc->m_szBundleScripts[nB]);
		std::string szFNTmp = "C:\\Temp.N3FXBundle";
		CFile file;
		if(FALSE != file.Open(szFNTmp.c_str(), CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(pDoc->m_szBundleScripts[nB], pDoc->m_szBundleScripts[nB].GetLength());
			file.Close();
			pDoc->m_EffectMgr.ParsingBundle((LPCTSTR)szFNTmp, pEB);
		}
	}

	int nP = m_PartList.GetCurSel();
	if(nP >= 0 && nP < pDoc->m_szPartScripts.GetSize())
	{
		GetDlgItemText(IDC_E_PART_SCRIPT, pDoc->m_szPartScripts[nP]);
		__EffectPartBase* pEPB = pDoc->m_EffectMgr.Part(nP);

		std::string szFNTmp = "C:\\Temp.N3FXPart";
		CFile file;
		if(FALSE != file.Open(szFNTmp.c_str(), CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(pDoc->m_szPartScripts[nP], pDoc->m_szPartScripts[nP].GetLength());
			file.Close();
			pDoc->m_EffectMgr.ParsingParts((LPCTSTR)szFNTmp, pEPB);
		}
	}
}

void CFormViewControl::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->GetPaneRender()->InvalidateRect(NULL, FALSE);

		CN3FEDoc* pDoc = GetDocument();
		int nEIC = pDoc->m_EffectMgr.InstanceCount(); // Effect Instance 갯수..
		if(nEIC <= 0) KillTimer(0); // 하나도 없으면.. 타이머 죽이기..
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CFormViewControl::OnBTrigger() 
{
	CN3FEDoc* pDoc = GetDocument();

	int nEB = m_BundleList.GetCurSel();
	__EffectBundle* pEB = pDoc->m_EffectMgr.BundleSrc(nEB);
	if(NULL == pEB) return;

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	__Vector3 vTarget = pFrm->GetPaneRender()->m_vTarget;
	vTarget.Normalize();

	pDoc->m_EffectMgr.Trigger(pEB->szName, __Vector3(0, 0, 0), vTarget);

	int nEIC = pDoc->m_EffectMgr.InstanceCount(); // Effect Instance 갯수..
	if(nEIC <= 1)
	{
		SetTimer(0, 10, NULL); // 하나도 없으면.. 타이머 작동..
	}
}

void CFormViewControl::OnBStop() 
{
	CN3FEDoc* pDoc = GetDocument();
	pDoc->m_EffectMgr.ReleaseInstances();
	KillTimer(0);
}

void CFormViewControl::OnChangeEPartScript() 
{
	CN3FEDoc* pDoc = GetDocument();
	
	int nSel = m_PartList.GetCurSel();
	if(nSel >= 0 && nSel < pDoc->m_szPartScripts.GetSize())
	{
		GetDlgItemText(IDC_E_PART_SCRIPT, pDoc->m_szPartScripts[nSel]);
	}
}

void CFormViewControl::OnChangeEBundleScript() 
{
	CN3FEDoc* pDoc = GetDocument();
	
	int nSel = m_BundleList.GetCurSel();
	if(nSel >= 0 && nSel < pDoc->m_szPartScripts.GetSize())
	{
		GetDlgItemText(IDC_E_BUNDLE_SCRIPT, pDoc->m_szBundleScripts[nSel]);
	}
}

void CFormViewControl::OnSelchangeListBundle() 
{
	this->UpdateInfo();
}

void CFormViewControl::OnSelchangeListPart() 
{
	this->UpdateInfo();
}

void CFormViewControl::OnBBundleAdd() 
{
	CN3FEDoc* pDoc = GetDocument();
	
	__EffectBundle* pEB = new __EffectBundle;
	pDoc->m_EffectMgr.m_BundlesSrc.push_back(pEB);
	int n = pDoc->m_szBundleScripts.Add("");
	wsprintf(pEB->szName, "New Bundle %.3d", n);
	m_BundleList.AddString(pEB->szName);
	m_BundleList.SetCurSel(n);

	this->UpdateInfo();
}

void CFormViewControl::OnBBundleInsert() 
{
}

void CFormViewControl::OnBBundleDelete() 
{
	CN3FEDoc* pDoc = GetDocument();
	int nSel = m_BundleList.GetCurSel();
	if(nSel < 0 || nSel >= pDoc->m_EffectMgr.BundleCount()) return;

	it_EB it = pDoc->m_EffectMgr.m_BundlesSrc.begin();
	for(int i = 0; i < nSel; i++, it++);
	delete (*it);
	pDoc->m_EffectMgr.m_BundlesSrc.erase(it);
	pDoc->m_szBundleScripts.RemoveAt(nSel);
	m_BundleList.DeleteString(nSel);
	m_BundleList.SetCurSel(nSel);
}

void CFormViewControl::OnBPartAdd() 
{
	CN3FEDoc* pDoc = GetDocument();
	
	__EffectPartBase* pEPB = new __EffectPartBase;
	pDoc->m_EffectMgr.m_Parts.push_back(pEPB);
	int n = pDoc->m_szPartScripts.Add("");
	wsprintf(pEPB->szName, "New Bundle %.3d", n);
	m_PartList.AddString(pEPB->szName);
	m_PartList.SetCurSel(n);

	this->UpdateInfo();
}

void CFormViewControl::OnBPartDelete() 
{
	CN3FEDoc* pDoc = GetDocument();
	int nSel = m_PartList.GetCurSel();
	if(nSel < 0 || nSel >= pDoc->m_EffectMgr.PartCount()) return;

	it_EP it = pDoc->m_EffectMgr.m_Parts.begin();
	for(int i = 0; i < nSel; i++, it++);
	delete (*it);
	pDoc->m_EffectMgr.m_Parts.erase(it);
	pDoc->m_szPartScripts.RemoveAt(nSel);
	m_PartList.DeleteString(nSel);
	m_PartList.SetCurSel(nSel);
}

void CFormViewControl::OnBPartInsert() 
{
}
