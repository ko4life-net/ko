#include "StdAfx.h"
#include "N3UIDebug.h"

#include "GameEng.h"
#include "GameProcedure.h"
#include "N3WorldManager.h"

#include "N3Base/N3Camera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>

// ImGui Window IDs
#define IMGUI_WND_ID_DEMO      "Dear ImGui Demo"
#define IMGUI_WND_ID_FPS       "FPS Graph##fps_graph"
#define IMGUI_WND_ID_METRICS   "Game Metrics##game_metrics"

bool CN3UIDebug::s_bReleaseCalled = false;

CN3UIDebug::CN3UIDebug() {}

void CN3UIDebug::Release() {
    s_bReleaseCalled = true;
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool CN3UIDebug::Init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(CN3Base::s_hWndBase);
    ImGui_ImplDX9_Init(CN3Base::s_lpD3DDev);

    ImGuiStyle & style = ImGui::GetStyle();
    style.FrameRounding = 10;
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.98f, 0.81f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.09f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.57f, 0.57f, 0.72f, 0.50f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.32f, 0.69f, 0.54f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.08f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.17f, 0.19f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.90f, 0.52f, 0.50f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.16f, 0.91f, 0.27f, 0.20f);

    return true;
}

void CN3UIDebug::BeginScene() {}

void CN3UIDebug::EndScene() {
    if (s_bReleaseCalled) {
        return;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RenderDockingSpace();

    static bool s_bDemoWindow = true;
    ImGui::ShowDemoWindow(&s_bDemoWindow);

    RenderFPSGraph();
    RenderGameMetrics();

    ImGui::EndFrame();
}

void CN3UIDebug::Present() {
    if (s_bReleaseCalled) {
        return;
    }

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CN3UIDebug::WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) {
        return true;
    }

    if (ImGui::GetCurrentContext() && (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)) {
        return true;
    }

    return false;
}

void CN3UIDebug::RenderDockingSpace() {
    static ImGuiWindowFlags s_DockWindowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
                                                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // Make sure the docking window fully covers the whole region.
    ImGuiViewport * pVP = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(pVP->Pos);
    ImGui::SetNextWindowSize(pVP->Size);
    ImGui::SetNextWindowViewport(pVP->ID);

    // Foreground window style: we don't need it visible, so disable some props.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Begin
    ImGui::Begin("Dockspace", NULL, s_DockWindowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGui::DockSpace(ImGui::GetID("MyDockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
}

void CN3UIDebug::RenderGameMetrics() {
    ImGui::Begin(IMGUI_WND_ID_METRICS);

    ImGui::Text("Camera:");
    CN3Camera * pCamera = CGameProcedure::s_pEng->CameraGetActive();
    ImGui::BulletText("%-8.2f Field of View", D3DXToDegree(pCamera->m_Data.fFOV), 5, 5, " ");
    ImGui::BulletText("%-8.2f Near Plane", pCamera->m_Data.fNP, 5, 5, " ");
    ImGui::BulletText("%-8.2f Far Plane", pCamera->m_Data.fFP, 5, 5, " ");
    ImGui::Checkbox("Fog Enabled", (bool *)&pCamera->m_bFogUse);
    ImGui::DragInt("View Distance", &CN3Base::s_Options.iViewDist, 0.5f, 0, 10000);
    if (ImGui::DragFloat("Field of View", &pCamera->m_Data.fFOV, 0.001f, 0.001f, 15.0f)) {
        CN3Base::s_CameraData.fFOV = pCamera->m_Data.fFOV;
    }
    if (ImGui::DragFloat("Near Plane", &pCamera->m_Data.fNP, 0.001f, 0.01f, 5.0f)) {
        CN3Base::s_CameraData.fNP = pCamera->m_Data.fNP;
    }

    ImGui::Separator();
    ImGui::Text("Character:");
    ImGui::BulletText("%-8d Character", CN3Base::s_RenderInfo.nChr);
    ImGui::BulletText("%-8d Part", CN3Base::s_RenderInfo.nChr_Part);
    ImGui::BulletText("%-8d Polygon", CN3Base::s_RenderInfo.nChr_Polygon);
    ImGui::BulletText("%-8d Plug", CN3Base::s_RenderInfo.nChr_Plug);
    ImGui::BulletText("%-8d Plug Polygon", CN3Base::s_RenderInfo.nChr_Plug_Polygon);

    if (CGameBase::ACT_WORLD) {
        ImGui::Separator();
        ImGui::Text("Terrain:");
        ImGui::BulletText("%-8d Polygon", CN3Base::s_RenderInfo.nTerrain_Polygon);
        ImGui::BulletText("%-8d Tile Polygon", CN3Base::s_RenderInfo.nTerrain_Tile_Polygon);
        ImGui::BulletText("%-8d Shape Count", CN3Base::s_RenderInfo.nShape);
        ImGui::BulletText("%-8d Shape Part", CN3Base::s_RenderInfo.nShape_Part);
        ImGui::BulletText("%-8d Shape Polygon", CN3Base::s_RenderInfo.nShape_Polygon);
    }

    ImGui::End();
}

void CN3UIDebug::RenderFPSGraph() {
    ImGui::Begin(IMGUI_WND_ID_FPS);
    static std::vector<float> s_Frames = {60.0f};

    float fTime = CN3Base::TimeGet();

    static float s_fTimePrev = fTime;
    static float s_fTimeInterval = 0.0f;
    static float s_fCurFrmPerSec = CN3Base::s_fFrmPerSec;
    static float s_fFrmAvg = s_Frames[0];
    static float s_fFrmMax = s_Frames[0];

    static bool s_bPauseFrms = false;
    if (!s_bPauseFrms) {
        s_fTimeInterval += fTime - s_fTimePrev;
        if (s_fTimeInterval >= 0.25f) {
            s_fTimeInterval = 0.0f;
            s_fCurFrmPerSec = CN3Base::s_fFrmPerSec;
            s_Frames.push_back(s_fCurFrmPerSec);
            if (s_Frames.size() > 50) {
                s_Frames.erase(s_Frames.begin());
            }
        }
        s_fTimePrev = fTime;

        s_fFrmAvg = s_Frames[0];
        s_fFrmMax = s_Frames[0];
        for (const float & fFrm : s_Frames) {
            s_fFrmAvg += fFrm;
            if (s_fFrmMax < fFrm) {
                s_fFrmMax = fFrm;
            }
        }
        s_fFrmAvg /= (float)s_Frames.size();
    }

    char szOverlay[64]{};
    sprintf(szOverlay, "FPS %4.4f\nAvg %4.4f", s_fCurFrmPerSec, s_fFrmAvg);
    ImGui::PlotLines("##fps", s_Frames.data(), s_Frames.size(), 0, szOverlay, -10.0f, s_fFrmMax + 50.0f,
                     ImVec2(ImGui::GetWindowContentRegionWidth(), 100.0f));
    ImGui::Checkbox("Pause Frames", &s_bPauseFrms);

    ImGui::End();
}
