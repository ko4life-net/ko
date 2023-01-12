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
    void RenderFPSGraph();
    void RenderGameController();

    // Game Controller Render Functions
    void RenderCamera();
    void RenderCharacter();
    void RenderTerrain();
    void RenderEnvironment();

  private:
    static bool s_bReleaseCalled;
};
