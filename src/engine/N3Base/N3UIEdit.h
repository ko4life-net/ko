// N3UIEdit.h: interface for the CN3UIEdit class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3UIStatic.h"

class CN3UIEdit : public CN3UIStatic {
    //    friend class CN3IME;
  public:
    CN3UIEdit();
    virtual ~CN3UIEdit();
    // class
  protected:
    class CN3Caret {
      public:
        CN3Caret();
        virtual ~CN3Caret();
        void SetPos(int x, int y);
        void MoveOffset(int iOffsetX, int iOffsetY);
        void SetSize(int iSize);
        void SetColor(D3DCOLOR color);
        void Render(LPDIRECT3DDEVICE9 lpD3DDev);
        void InitFlckering(); // 깜박임 초기화..
        BOOL m_bVisible;      // 보이나
      protected:
        int   m_iSize;            // caret의 pixel 크기
        float m_fFlickerTimePrev; // 깜박이기 위한 시간..
        bool  m_bFlickerStatus;
        //        POINT    m_ptPos;                // caret의 pixel 좌표
        __VertexTransformedColor m_pVB[2]; // vertex 버퍼
    };

    // Attributes
  public:
    /*
    static bool        AddEdit(CN3UIEdit* pEdit);
    static bool        DeleteEdit(CN3UIEdit* pEdit);
    static CN3UIEdit* GetEdit(HWND hWnd);
    static HWND        s_hWndParent;

    LRESULT APIENTRY EditWndProcFuncPointer(HWND hWnd, WORD Message, WPARAM wParam, LPARAM lParam);
    static LRESULT    APIENTRY EditWndProc(HWND hWnd, WORD Message, WPARAM wParam, LPARAM lParam);
    static            void SetParentWindowHwnd(HWND hWnd) { s_hWndParent = hWnd; }
*/
    static HWND    s_hWndEdit, s_hWndParent;
    static WNDPROC s_lpfnEditProc;
    static char    s_szBuffTmp[512];

    static void             SetImeStatus(POINT ptPos, bool bOpen);
    static BOOL             CreateEditWindow(HWND hParent, RECT rect);
    static LRESULT APIENTRY EditWndProc(HWND hWnd, WORD Message, WPARAM wParam, LPARAM lParam);
    static void             UpdateTextFromEditCtrl();
    static void             UpdateCaretPosFromEditCtrl(bool bNeedsFocus);

  protected:
    static CN3Caret s_Caret;
    UINT            m_nCaretPos;   // 글자 단위위치(byte단위)
    int             m_iCompLength; // 현재 조합중인 글자의 byte수 0이면 조합중이 아니다.
    int             m_iMaxStrLen;  // 쓸수 있는 글씨의 최대 숫자
    std::string     m_szPassword;  // password buffer

    CN3SndObj * m_pSnd_Typing; // 타이핑 할 때 나는 소리

    // Operations
  public:
    virtual const std::string & GetString();
    virtual void                SetString(const std::string & szString);

    virtual bool  Load(HANDLE hFile);
    virtual void  Render();
    virtual void  Release();
    virtual void  SetVisible(bool bVisible);
    virtual DWORD MouseProc(DWORD dwFlags, const POINT & ptCur, const POINT & ptOld);
    virtual BOOL  MoveOffset(int iOffsetX,
                             int iOffsetY); // 위치 지정(chilren의 위치도 같이 바꾸어준다. caret위치도 같이 바꾸어줌.)
    void          KillFocus(); // 포커스를 없앤다.
    bool          SetFocus();  // 포커스를 준다.
    bool          HaveFocus() const { return (this == s_pFocusedEdit); }
    void          SetCaretPos(UINT nPos); //몇번째 바이트에 있는지 설정한다.
    void          SetMaxString(int iMax); // 최대 글씨 수를 정해준다.
  protected:
    BOOL IsHangulMiddleByte(const char * lpszStr, int iPos); // 한글의 2번째 바이트 글자인가?

#ifdef _N3TOOL
  public:
    virtual void operator=(const CN3UIEdit & other);
    virtual bool Save(HANDLE hFile);
    void         SetSndTyping(const fs::path & fsFile);
    fs::path     GetSndFName_Typing() const;
#endif
};
