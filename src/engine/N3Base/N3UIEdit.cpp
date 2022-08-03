// N3UIEdit.cpp: implementation of the CN3UIEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3UIEdit.h"
#include "N3UIString.h"
#include "N3UIImage.h"
#include "DFont.h"
#include "N3UIStatic.h"

#include "N3SndMgr.h"
#include "N3SndObj.h"
#include <imm.h>


const float CARET_FLICKERING_TIME = 0.4f;

//HWND CN3UIEdit::s_hWndParent = NULL;
HWND CN3UIEdit::s_hWndEdit = NULL;
HWND CN3UIEdit::s_hWndParent = NULL;
WNDPROC	CN3UIEdit::s_lpfnEditProc = NULL;
char CN3UIEdit::s_szBuffTmp[512] = "";

//////////////////////////////////////////////////////////////////////
// CN3UIEdit::CN3Caret
//////////////////////////////////////////////////////////////////////
CN3UIEdit::CN3Caret::CN3Caret()
{
	m_pVB[0].Set(0,0,UI_DEFAULT_Z, UI_DEFAULT_RHW, 0xffff0000);
	m_pVB[1].Set(0,10,UI_DEFAULT_Z, UI_DEFAULT_RHW, 0xffff0000);
	m_bVisible = FALSE;
	m_fFlickerTimePrev = CN3Base::TimeGet();	// �����̱� ���� �ð�..
	m_bFlickerStatus = true;

}
CN3UIEdit::CN3Caret::~CN3Caret()
{
}
void CN3UIEdit::CN3Caret::SetPos(int x, int y)
{
	m_pVB[0].x = x; m_pVB[0].y = y;
	m_pVB[1].x = x; m_pVB[1].y = y+m_iSize;
	this->InitFlckering();
}
void CN3UIEdit::CN3Caret::MoveOffset(int iOffsetX, int iOffsetY)
{
	m_pVB[0].x += iOffsetX; m_pVB[0].y += iOffsetY; 
	m_pVB[1].x = m_pVB[0].x; m_pVB[1].y = m_pVB[0].y+m_iSize;
	this->InitFlckering();
}
void CN3UIEdit::CN3Caret::SetSize(int iSize)
{
	m_iSize = iSize;
	m_pVB[1].y = m_pVB[0].y+m_iSize;
}
void CN3UIEdit::CN3Caret::SetColor(D3DCOLOR color)
{
	m_pVB[0].color = color;
	m_pVB[1].color = color;
}
void CN3UIEdit::CN3Caret::Render(LPDIRECT3DDEVICE9	lpD3DDev)
{
	if (FALSE == m_bVisible) return;

	// ������ ó��..
	float fTime = CN3Base::TimeGet();
	if(fTime - m_fFlickerTimePrev > CARET_FLICKERING_TIME)
	{
		m_bFlickerStatus = !m_bFlickerStatus;
		m_fFlickerTimePrev = fTime;
	}
	if(!m_bFlickerStatus) return;

	__ASSERT(lpD3DDev, "DIRECT3DDEVICE9 is null");
	lpD3DDev->SetTexture(0, NULL);
//	lpD3DDev->SetTextureStageState( 0, D3DTSS_COLOROP,    D3DTOP_SELECTARG1 );
//	lpD3DDev->SetTextureStageState( 0, D3DTSS_COLORARG1,  D3DTA_DIFFUSE );
	lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
	lpD3DDev->DrawPrimitiveUP(D3DPT_LINELIST, 1, m_pVB, sizeof(m_pVB[0]));
}
void CN3UIEdit::CN3Caret::InitFlckering()
{
	m_fFlickerTimePrev = CN3Base::TimeGet();	// �����̱� ���� �ð�..
	m_bFlickerStatus = true;
}

//////////////////////////////////////////////////////////////////////
// CN3UIEdit
//////////////////////////////////////////////////////////////////////

BOOL CN3UIEdit::CreateEditWindow(HWND hParent, RECT rect)
{
	if(NULL == hParent) return FALSE;
	if(s_hWndEdit) return FALSE;

	s_hWndParent = hParent;
	s_hWndEdit = CreateWindow("EDIT", "EditWindow", WS_CHILD|WS_TABSTOP|ES_LEFT|ES_WANTRETURN, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, hParent, NULL, NULL, NULL);
	s_lpfnEditProc = (WNDPROC)SetWindowLong(s_hWndEdit, GWL_WNDPROC, (DWORD)(CN3UIEdit::EditWndProc));

	// Set the edit control's text size to the maximum.
	::SendMessage(s_hWndEdit, EM_LIMITTEXT, 0, 0);

	// Set the edit control's font
	HFONT hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
	::SendMessage(s_hWndEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
	::SendMessage(s_hWndEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

	// ��� �����...??
	HDC hDC = GetDC(s_hWndEdit);
	SetBkMode(hDC, TRANSPARENT);
	SetROP2(hDC, R2_XORPEN);
	ReleaseDC(s_hWndEdit, hDC);

	return TRUE;
}

LRESULT APIENTRY CN3UIEdit::EditWndProc(HWND hWnd, WORD Message, WPARAM wParam, LPARAM lParam)
{
   //
   // When the focus is in an edit control inside a dialog box, the
   //  default ENTER key action will not occur.
   //
    switch (Message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			if(s_pFocusedEdit && s_pFocusedEdit->GetParent())
			{
				s_pFocusedEdit->GetParent()->ReceiveMessage(s_pFocusedEdit, UIMSG_EDIT_RETURN);
			}
			return 0;
		}
		(CallWindowProc(s_lpfnEditProc, hWnd, Message, wParam, lParam));
		if(s_pFocusedEdit) CN3UIEdit::UpdateCaretPosFromEditCtrl();
		return 0;
		//break;

    case WM_CHAR:
		if(s_pFocusedEdit) CN3UIEdit::UpdateCaretPosFromEditCtrl();
		if(wParam==VK_RETURN) return 0;
		if(wParam==VK_TAB) return 0;
		break;
	case WM_INPUTLANGCHANGE:
		{
			POINT ptPos;
			ptPos.x = 0;
			ptPos.y = 0;
			SetImeStatus(ptPos,true);
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		{
			POINT ptPos;
			ptPos.x = -1000;
			ptPos.y = -1000;
			SetImeStatus(ptPos,false);
		}
		break;
	case WM_IME_STARTCOMPOSITION:
		{
			POINT ptPos;
			ptPos.x = 0;
			ptPos.y = 0;
			SetImeStatus(ptPos,true);
		}
		break;
    } // switch









/*
	switch(Message)
	{
	case WM_IME_CHAR:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_COMPOSITION:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_COMPOSITIONFULL:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_CONTROL:
		{
			int iiii = 0;
			DWORD dwCmd = wParam;
			switch(dwCmd)
			{
			case IMC_GETCANDIDATEPOS: 
				iiii = 0;
				break;
			case IMC_OPENSTATUSWINDOW:
				iiii = 0;
				break;
			case IMC_GETCOMPOSITIONFONT:
				iiii = 0;
				break;
			case IMC_SETCANDIDATEPOS :
				iiii = 0;
				break;
			case IMC_GETCOMPOSITIONWINDOW:
				iiii = 0;
				break;
			case IMC_SETCOMPOSITIONFONT :
				iiii = 0;
				break;
//			case IMC_GETCONVERSIONMODE:
//				iiii = 0;
//				break;
			case IMC_SETCOMPOSITIONWINDOW :
				iiii = 0;
				break;
//			case IMC_GETOPENSTATUS:
//				iiii = 0;
//				break;
//			case IMC_SETCONVERSIONMODE :
//				iiii = 0;
//				break;
//			case IMC_GETSENTENCEMODE:
//				iiii = 0;
//				break;
//			case IMC_SETOPENSTATUS :
//				iiii = 0;
//				break;
			case IMC_GETSTATUSWINDOWPOS:
				iiii = 0;
				break;
//			case IMC_SETSENTENCEMODE :
//				iiii = 0;
//				break;
			case IMC_CLOSESTATUSWINDOW:
				iiii = 0;
				break;
			case IMC_SETSTATUSWINDOWPOS :
				iiii = 0;
				break;
			}
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_KEYDOWN:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_KEYUP:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_NOTIFY:
		{
			int iiii = 0;

			switch(wParam)
			{
			case IMN_CHANGECANDIDATE:
				{
					int iiii = 0;
					break;
				}
			case IMN_SETCANDIDATEPOS:
				{
					int iiii = 0;
					break;
				}
			case IMN_CLOSECANDIDATE:
				{
					int iiii = 0;
					break;
				}
			case IMN_SETCOMPOSITIONFONT:
				{
					int iiii = 0;
					break;
				}
			case IMN_CLOSESTATUSWINDOW:
				{
					int iiii = 0;
					break;
				}
			case IMN_SETCOMPOSITIONWINDOW:
				{
					int iiii = 0;
					
					COMPOSITIONFORM CompForm;
					CompForm.dwStyle = CFS_RECT;
					int msg2 = (UINT) WM_IME_CONTROL;
					WPARAM wParam2 = (WPARAM) IMC_GETCOMPOSITIONWINDOW;
					WPARAM lParam2 = (LPARAM) &CompForm;
					SendMessage(hWnd, msg2, wParam2, lParam2);

					break;
				}
			case IMN_GUIDELINE:
				{
					int iiii = 0;
					break;
				}
			case IMN_SETCONVERSIONMODE:
				{
					int iiii = 0;
					break;
				}
			case IMN_OPENCANDIDATE:
				{
					int iiii = 0;
					break;
				}
			case IMN_SETOPENSTATUS:
				{
					int iiii = 0;
					break;
				}
			case IMN_OPENSTATUSWINDOW:
				{
					int iiii = 0;
					break;
				}
			case IMN_SETSENTENCEMODE:
				{
					int iiii = 0;
					break;
				}
			case IMN_PRIVATE:
				{
					int iiii = 0;

					CANDIDATEFORM cf;
					cf.dwStyle = CFS_RECT;
					iiii = ::SendMessage(hWnd, WM_IME_CONTROL, IMC_GETCANDIDATEPOS, (LPARAM)(&cf));

					LOGFONT lf;
					iiii = ::SendMessage(hWnd, WM_IME_CONTROL, IMC_GETCOMPOSITIONFONT, (LPARAM)(&lf));

					iiii = 0;



					break;
				}
			case IMN_SETSTATUSWINDOWPOS:
				{
					int iiii = 0;
					break;
				}
			}

//			CANDIDATEFORM form;
//			form.dwIndex = 0;
//			form.dwStyle = CFS_EXCLUDE;
//			this->SendMessage(0x0288, 0x0002, (LPARAM)(&form));
//			iiii = -1;
//			this->SendMessage(WM_IME_REQUEST, IMR_CANDIDATEWINDOW, &form);
		}
		break;
	case WM_IME_SELECT:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_SETCONTEXT:
		{
			int iiii = 0;
		}
		break;
	case WM_IME_STARTCOMPOSITION:
		{
			int iiii = 0;
		}
		break;
	}
*/
















	return (CallWindowProc(s_lpfnEditProc, hWnd, Message, wParam, lParam));
}

CN3UIEdit::CN3Caret CN3UIEdit::s_Caret;

CN3UIEdit::CN3UIEdit()
{
	m_eType = UI_TYPE_EDIT;
	m_nCaretPos = 0;
	m_iCompLength = 0;
	m_iMaxStrLen = 0x7fffffff;
	KillFocus();
	m_pSnd_Typing = NULL;
}

CN3UIEdit::~CN3UIEdit()
{
	KillFocus();
	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Typing);
}

void CN3UIEdit::Release()
{
	CN3UIBase::Release();
	m_nCaretPos = 0;
	m_iCompLength = 0;
	m_iMaxStrLen = 0x7fffffff;
	KillFocus();
	m_szPassword = "";
	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Typing);
}

void CN3UIEdit::Render()
{
	if (!m_bVisible) return;

	CN3UIStatic::Render();
	if (HaveFocus())
	{
		s_Caret.Render(s_lpD3DDev);	// ��Ŀ���� ������ ĳ�� �׸���
	}
}

void CN3UIEdit::SetVisible(bool bVisible)
{
	CN3UIBase::SetVisible(bVisible);

	if (false == bVisible && true == m_bVisible)	// ������ �ʰ� �Ҷ�
	{
		KillFocus();
	}
}

void CN3UIEdit::KillFocus()
{
	if (HaveFocus())
	{
		s_pFocusedEdit = NULL;
		s_Caret.m_bVisible = FALSE;

		if(s_hWndEdit)
		{
			::SetWindowText(s_hWndEdit, "");
			::SetFocus(s_hWndParent);
		}
	}
}

bool CN3UIEdit::SetFocus()
{
//	if (HaveFocus()) return true;		// �̹� ���� ��Ŀ���� ������ ������ return true;
	if (NULL != s_pFocusedEdit) s_pFocusedEdit->KillFocus();	// �ٸ� edit �� ������ ������ killfocusȣ��
	s_pFocusedEdit = this;				// ��Ŀ���� ������ �ִ� edit�� ���� ����

	SIZE size;
	if (m_pBuffOutRef && m_pBuffOutRef->GetTextExtent("��",2,&size))
	{
		s_Caret.SetSize(size.cy);
		s_Caret.SetColor(m_pBuffOutRef->GetFontColor());
	}

	s_Caret.m_bVisible = TRUE;
	s_Caret.InitFlckering();
	CN3UIEdit::UpdateCaretPosFromEditCtrl(); // ĳ�� ������ ����

	if(s_hWndEdit)
	{
		::SetFocus(s_hWndEdit);


		RECT rcEdit = GetRegion();
		int iX		= rcEdit.left;
		int iY		= rcEdit.top;
		int iH		= rcEdit.bottom - rcEdit.top;
		int iW		= rcEdit.right - rcEdit.left;
		::MoveWindow(s_hWndEdit, iX, iY, iW, iH, false);
		
		if(UISTYLE_EDIT_PASSWORD & m_dwStyle)
		{
			::SetWindowText(s_hWndEdit, m_szPassword.c_str());
		}
		else
		{
			if(m_pBuffOutRef) ::SetWindowText(s_hWndEdit, m_pBuffOutRef->GetString().c_str());
			else ::SetWindowText(s_hWndEdit, "");
		}
	}

	return true;
}

DWORD CN3UIEdit::MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	DWORD dwRet = UI_MOUSEPROC_NONE;
	if (!m_bVisible) return dwRet;
	if(dwFlags & UI_MOUSE_LBCLICK &&IsIn(ptCur.x, ptCur.y))	// ���� �ȿ��� ���� ��ư�� ��������
	{
		SetFocus();	// ������ ��Ŀ���� �ش�.
		dwRet |= (UI_MOUSEPROC_DONESOMETHING|UI_MOUSEPROC_INREGION);
		return dwRet;
	}
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

void CN3UIEdit::SetCaretPos(UINT nPos)
{
	if (nPos > m_iMaxStrLen) nPos = m_iMaxStrLen;	// �ִ� ���̺��� ���� �۰� ����

	const std::string& szBuff = m_pBuffOutRef->GetString();

	m_nCaretPos = nPos;

	
	__ASSERT(szBuff.empty() || -1 == szBuff.find('\n'), "multiline edit");	// ������ multiline�� �������� �ʴ´�.
	SIZE size = {0,0};
	if (!szBuff.empty() && m_pBuffOutRef ) m_pBuffOutRef->GetTextExtent(szBuff, m_nCaretPos, &size) ;

	int iRegionWidth = m_rcRegion.right - m_rcRegion.left;
	if (size.cx > iRegionWidth) size.cx = iRegionWidth;
	s_Caret.SetPos(m_pBuffOutRef->m_ptDrawPos.x + size.cx, m_pBuffOutRef->m_ptDrawPos.y);
}

void CN3UIEdit::SetMaxString(int iMax)		// �ִ� �۾� ���� �����ش�
{
	if (iMax <= 0) {__ASSERT(0, "�ִ� �۾� ���� 0���� ũ�� �����ּ���"); return;}
	m_iMaxStrLen = iMax;

	if (NULL == m_pBuffOutRef) return;

	const std::string szBuff = GetString();
	if ( m_iMaxStrLen >= szBuff.size()) return;

	// ������� ���� ���� ���� �۾����̰� iMax���� ū ����̹Ƿ� ���ѱ��ڿ� ���� �߶��ְ� �ٽ� �����Ѵ�.
	SetString(szBuff);
}

/////////////////////////////////////////////////////////////////////
//
// Ư�� ��ġ�� �ѱ��� 2byte�߿� �ι�° ����Ʈ���� �˻��ϴ� �κ��̴�.
// IsDBCSLeadByte��� �Լ��� ������ �������� ����
// ����Byte�� ��byte�� ������ ������ �� �Լ��� �˻� �� �� ����.
// ���� ó������ �˻縦 �ϴ� ��� �ܿ��� �ٸ� ����� ����.
//
// NT�� Unicode������ ��� �ۿ��ϴ��� �˻��� ���� �ʾ�����
// �� �ٸ� ���� ���� ����� �� �ִٰ� �����Ѵ�.
//
/////////////////////////////////////////////////////////////////////
BOOL CN3UIEdit::IsHangulMiddleByte( const char* lpszStr, int iPos )
{
    if( !lpszStr ) return FALSE;
    if (iPos <= 0) return FALSE;
    int nLength = lstrlen(lpszStr);
    if (iPos >= nLength) return FALSE;
    if (!(lpszStr[iPos] & 0x80))
        return FALSE;

    BOOL bMiddle = FALSE;
    for (int i=0; i<iPos && i <nLength; i++)
    {
        if (lpszStr[i] & 0x80)
            bMiddle = !bMiddle;
    }
    return bMiddle;
}

const std::string& CN3UIEdit::GetString()
{
	if (UISTYLE_EDIT_PASSWORD & m_dwStyle) return m_szPassword;
	return CN3UIStatic::GetString();
}

void CN3UIEdit::SetString(const std::string& szString)
{
	if (NULL == m_pBuffOutRef) return;
	if (szString.size() > m_iMaxStrLen)
	{
		std::string szNewBuff(m_iMaxStrLen, '?');

		if (IsHangulMiddleByte(szString.c_str(), m_iMaxStrLen))
		{
			szNewBuff = szString.substr(0, m_iMaxStrLen-1);	// -1�� �ѱ��̹Ƿ� �ϳ� �� ī���ϱ� ���� +1�� �� �������� NULL �ֱ� ����
			if (UISTYLE_EDIT_PASSWORD & m_dwStyle)
			{
				int iNewBuffLen = szNewBuff.size();
				m_szPassword = szNewBuff;

				szNewBuff.assign(m_iMaxStrLen-1, '*');
				__ASSERT(NULL == szNewBuff[m_iMaxStrLen-1],"���ڼ��� �ٸ���.");
			}
			m_pBuffOutRef->SetString(szNewBuff);
		}
		else
		{
			szNewBuff = szString.substr(0, m_iMaxStrLen);	// +1�� �� �������� NULL �ֱ� ����
			if (UISTYLE_EDIT_PASSWORD & m_dwStyle)
			{
				int iNewBuffLen = szNewBuff.size();
				m_szPassword = szNewBuff;

				szNewBuff.assign(m_iMaxStrLen, '*');
				__ASSERT(NULL == szNewBuff[m_iMaxStrLen],"���ڼ��� �ٸ���.");
			}
			m_pBuffOutRef->SetString(szNewBuff);
		}
	}
	else
	{
		if (UISTYLE_EDIT_PASSWORD & m_dwStyle)
		{
			m_szPassword = szString;
			if (!szString.empty())
			{
				std::string szNewBuff(szString.size(), '*');
				m_pBuffOutRef->SetString(szNewBuff);
			}
			else m_pBuffOutRef->SetString("");
		}
		else
		{
			m_pBuffOutRef->SetString(szString);
		}
	}

	const std::string& szTempStr = m_pBuffOutRef->GetString();
	int iStrLen = szTempStr.size();
	if (m_nCaretPos > iStrLen) SetCaretPos(iStrLen);
}

BOOL CN3UIEdit::MoveOffset(int iOffsetX, int iOffsetY)		// ��ġ ����(chilren�� ��ġ�� ���� �ٲپ��ش�. caret��ġ�� ���� �ٲپ���.)
{
	if (FALSE == CN3UIBase::MoveOffset(iOffsetX, iOffsetY)) return FALSE;
/*
	RECT rcEdit = GetRegion();
	int iX		= rcEdit.left;
	int iY		= rcEdit.top;
	int iH		= rcEdit.bottom - rcEdit.top;
	int iW		= rcEdit.right - rcEdit.left;

	::MoveWindow(s_hWndEdit, iX, iY, iW, iH, false);
*/
	if (HaveFocus()) s_Caret.MoveOffset(iOffsetX, iOffsetY);
	return TRUE;
}

bool CN3UIEdit::Load(HANDLE hFile)
{
	if (false == CN3UIStatic::Load(hFile)) return false;

	// ���� uif������ ������ �Ϸ��� ���� �ε� �ϴ� �κ� ����
	int iSndFNLen = 0;
	DWORD dwNum;

	ReadFile(hFile, &iSndFNLen, sizeof(iSndFNLen), &dwNum, NULL);		//	���� ���� ���ڿ� ����
	if (iSndFNLen>0)
	{
		std::vector<char> buffer(iSndFNLen+1, NULL);
		ReadFile(hFile, &buffer[0], iSndFNLen, &dwNum, NULL);

		__ASSERT(NULL == m_pSnd_Typing, "memory leak");
		m_pSnd_Typing = s_SndMgr.CreateObj(std::string(buffer.begin(), buffer.end()), SNDTYPE_2D);
	}

	return true;
}

#ifdef _N3TOOL
void CN3UIEdit::operator = (const CN3UIEdit& other)
{
	CN3UIStatic::operator = (other);
	SetSndTyping(other.GetSndFName_Typing());
}

bool CN3UIEdit::Save(HANDLE hFile)
{
	if (false == CN3UIStatic::Save(hFile)) return false;

	DWORD dwNum;

	int iSndFNLen = 0;
	if (m_pSnd_Typing) iSndFNLen = m_pSnd_Typing->m_szFileName.size();
	WriteFile(hFile, &iSndFNLen, sizeof(iSndFNLen), &dwNum, NULL);		//	���� ���� ���ڿ� ����
	if (iSndFNLen>0) WriteFile(hFile, m_pSnd_Typing->m_szFileName.c_str(), iSndFNLen, &dwNum, NULL);

	return true;
}

void CN3UIEdit::SetSndTyping(const std::string& strFileName)
{
	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Typing);
	if (0 == strFileName.size()) return;

	CN3BaseFileAccess tmpBase;
	tmpBase.FileNameSet(strFileName);	// Base��ο� ���ؼ� ����� ��θ� �Ѱ��ش�.

	SetCurrentDirectory(tmpBase.PathGet().c_str());
	m_pSnd_Typing = s_SndMgr.CreateObj(tmpBase.FileName(), SNDTYPE_2D);
}

std::string CN3UIEdit::GetSndFName_Typing() const
{
	if (m_pSnd_Typing) return m_pSnd_Typing->m_szFileName;
	else return std::string("");
}
#endif

void CN3UIEdit::UpdateTextFromEditCtrl()
{
	if(NULL == s_pFocusedEdit || NULL == s_hWndEdit) return;

	::GetWindowText(s_hWndEdit, s_szBuffTmp, 512);
	s_pFocusedEdit->SetString(s_szBuffTmp);
}

void CN3UIEdit::UpdateCaretPosFromEditCtrl()
{
	if(NULL == s_pFocusedEdit || NULL == s_hWndEdit) return;

/*	int iCaret = 0;
	int iLen = GetWindowTextLength(s_hWndEdit);
	POINT ptCaret;
	GetCaretPos(&ptCaret);
	if(ptCaret.x > 0)
	{
		HDC hDC = GetDC(s_hWndEdit);
		SIZE size;
		GetTextExtentPoint32(hDC, "1", 1, &size);
		iCaret = ptCaret.x / size.cx;
		ReleaseDC(s_hWndEdit, hDC);
	}
*/


	std::string getText = s_pFocusedEdit->GetString();
	int cursorPosition = getText.size();

	SendMessageA(s_hWndEdit, EM_SETSEL, cursorPosition, cursorPosition);


	//int iCTmp2 = HIWORD(iTmp);
	s_pFocusedEdit->SetCaretPos(cursorPosition);
}

void CN3UIEdit::SetImeStatus(POINT ptPos, bool bOpen)
{
#ifndef _N3TOOL
	HKL hHKL = GetKeyboardLayout(0);
	if(ImmIsIME(hHKL))
	{
		HIMC hImc = ImmGetContext(s_hWndEdit);
		if(bOpen)
		{
			SendMessage(s_hWndEdit, WM_IME_NOTIFY, IMN_OPENSTATUSWINDOW, 0);
			ImmSetStatusWindowPos(hImc,&ptPos);
		}
		else
		{
			SendMessage(s_hWndEdit, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, 0);
		}
		ImmReleaseContext( s_hWndEdit, hImc );
	}
#endif
}

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////
//	IME �����ؼ�
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include<map>
typedef typename std::map<HWND, CN3UIEdit*>::iterator		it_Edit;
typedef typename std::map<HWND, CN3UIEdit*>::value_type		val_Edit;
static std::map<HWND, CN3UIEdit*> s_Edits;

bool CN3UIEdit::AddEdit(CN3UIEdit* pEdit)
{
	if(pEdit == NULL)
	{
		__ASSERT(0, "NULL POINTER");
		return false;
	}

	it_Edit it = s_Edits.find(pEdit->m_hWndEdit);
	if(it == s_Edits.end()) // �ߺ��Ȱ� ������..
	{
		s_Edits.insert(val_Edit(pEdit->m_hWndEdit, pEdit));
		return true;
	}
	else // �ߺ��Ǿ�����..
	{
		__ASSERT(0, "Edit Handle Duplicate");
		return false;
	}
}

bool CN3UIEdit::DeleteEdit(CN3UIEdit* pEdit)
{
	if(pEdit == NULL)
	{
		__ASSERT(0, "NULL POINTER");
		return false;
	}

	it_Edit it = s_Edits.find(pEdit->m_hWndEdit);
	if(it == s_Edits.end()) return false;

	s_Edits.erase(it);
	return true;
}

CN3UIEdit* CN3UIEdit::GetEdit(HWND hWnd)
{
	it_Edit it = s_Edits.find(hWnd);
	if(it == s_Edits.end()) return NULL;

	return it->second;
}

LRESULT APIENTRY CN3UIEdit::EditWndProc(HWND hWnd, WORD Message, WPARAM wParam, LPARAM lParam)
{
   //
   // When the focus is in an edit control inside a dialog box, the
   //  default ENTER key action will not occur.
   //
	CN3UIEdit* pEdit = CN3UIEdit::GetEdit(hWnd);
	if(pEdit) pEdit->EditWndProcFuncPointer(hWnd, Message, wParam, lParam);

	return 0;
}

LRESULT APIENTRY CN3UIEdit::EditWndProcFuncPointer(HWND hWnd, WORD Message, WPARAM wParam, LPARAM lParam)
{
   //
   // When the focus is in an edit control inside a dialog box, the
   //  default ENTER key action will not occur.
   //
    switch (Message)
	{
	case WM_CREATE:
		::SetWindowText(m_hWndEdit,"");
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			if(GetParent())
			{
				GetParent()->ReceiveMessage(this, UIMSG_EDIT_RETURN);
			}
			return 0;
		}
		(CallWindowProc(m_lpfnEditProc, hWnd, Message, wParam, lParam));
		UpdateCaretPosFromEditCtrl();
		return 0;
		//break;

    case WM_CHAR:
		CN3UIEdit::UpdateCaretPosFromEditCtrl();
		if(wParam==VK_RETURN) return 0;
		if(wParam==VK_TAB) return 0;
		break;
    } // switch
	return (CallWindowProc(m_lpfnEditProc, hWnd, Message, wParam, lParam));
}
*/

