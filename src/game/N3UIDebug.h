#pragma once

class CN3UIDebug {
  public:
    CN3UIDebug();

    bool Init();
    void BeginScene();
    void EndScene();
    void Present();

    static void             Release();
    static LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  private:
    void RenderDockSpace();
    void RenderGameMetrics();
    void RenderFPSGraph();

  private:
    static bool s_bReleaseCalled;
};
