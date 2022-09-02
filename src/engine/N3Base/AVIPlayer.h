// AVIPlayer.h: interface for the CAVIPlayer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <dshow.h>

class CAVIPlayer {
  public:
    CAVIPlayer();
    virtual ~CAVIPlayer();

  public:
    bool               PlayCutScene(LPTSTR pszMovie, HINSTANCE hInstance);
    static LONG WINAPI WindowProc_Player(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static bool        m_bInterrupted;

  protected:
    HWND            m_hWndMain;
    IGraphBuilder * m_pGraphBuilder;
    IMediaControl * m_pMediaControl;
    IVideoWindow *  m_pVideoWindow;
    IMediaEvent *   m_pMediaEvent;

    BOOL CreateHiddenWindow(HINSTANCE hInstance, LPTSTR szFile);
    bool InitInterfaces();
    void Release();
};
