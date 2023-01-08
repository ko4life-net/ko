#include "StdAfx.h"
#include "N3UIDebug.h"

#include "GameEng.h"
#include "GameProcedure.h"
#include "N3WorldManager.h"

#include "N3Base/N3Camera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>

// ImGui Window IDs
#define IMGUI_WND_ID_DEMO      "Dear ImGui Demo"
#define IMGUI_WND_ID_FPS       "FPS Graph##fps_graph"
#define IMGUI_WND_ID_METRICS   "Game Metrics##game_metrics"
#define IMGUI_WND_ID_DASHBOARD "Dashboard"

bool CN3UIDebug::s_bReleaseCalled = false;

CN3UIDebug::CN3UIDebug() = default;

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

    ImGuiStyle & style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    style.FrameRounding = 10;
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.98f, 0.81f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.09f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.57f, 0.57f, 0.72f, 0.50f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.32f, 0.69f, 0.54f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.08f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.17f, 0.19f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.90f, 0.52f, 0.50f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.16f, 0.91f, 0.27f, 0.20f);

    ImGui_ImplWin32_Init(CN3Base::s_hWndBase);
    ImGui_ImplDX9_Init(CN3Base::s_lpD3DDev);

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
    RenderDockSpace();

    static bool s_bDemoWindow = true;
    if (s_bDemoWindow) {
        ImGui::ShowDemoWindow(&s_bDemoWindow);
    }

    RenderGameMetrics();
    RenderFPSGraph();

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CN3UIDebug::Present() {}

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

void CN3UIDebug::RenderDockSpace() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin(IMGUI_WND_ID_DASHBOARD);
    ImGuiID dsId = ImGui::GetID("DashboardDS");
    if (!ImGui::DockBuilderGetNode(dsId)) {
        ImGui::SetWindowSize(ImVec2(370.0f, 680.0f));
        ImGui::SetWindowPos(ImVec2(640.0f, 55.0f));

        ImGui::DockBuilderRemoveNode(dsId);
        ImGui::DockBuilderAddNode(dsId);

        ImGuiID dsIdCopy = dsId;
        ImGuiID dsIdUp = ImGui::DockBuilderSplitNode(dsIdCopy, ImGuiDir_Up, 0.34f, NULL, &dsIdCopy);
        ImGuiID dsIdDown = ImGui::DockBuilderSplitNode(dsIdCopy, ImGuiDir_Down, 0.0f, NULL, &dsIdCopy);
        ImGui::DockBuilderDockWindow(IMGUI_WND_ID_FPS, dsIdUp);
        ImGui::DockBuilderDockWindow(IMGUI_WND_ID_METRICS, dsIdDown);
        ImGui::DockBuilderDockWindow(IMGUI_WND_ID_DEMO, dsIdDown);

        ImGui::DockBuilderFinish(dsId);
    }
    ImGui::DockSpace(dsId);
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

        ImGui::Separator();
        ImGui::Text("Environment:");
        CN3SkyMng * pSM = CGameBase::ACT_WORLD->GetSkyRef();

        int iYear = 0, iMonth = 0, iDay = 0, iHour = 0, iMin = 0;
        pSM->GetGameTime(&iYear, &iMonth, &iDay, &iHour, &iMin);

        static int iTimeSliderPos = 0;
        char       szDateTime[60]{};
        sprintf(szDateTime, "%.2d/%.2d/%.4d %.2d:%.2d", iMonth, iDay, iYear, iHour, iMin);
        if (ImGui::SliderInt("Time", &iTimeSliderPos, 0, 24 * 60, szDateTime)) {
            iHour = iTimeSliderPos / 60;
            iMin = iTimeSliderPos % 60;
            if (iHour == 24) {
                iHour = 0;
            }
            pSM->SetGameTime(iYear, iMonth, iDay, iHour, iMin);
            pSM->Tick();
        } else {
            iTimeSliderPos = iHour * 60 + iMin % 60;
        }

        static int iPercentR = 0, iPercentS = 0;
        if (ImGui::DragInt("Rain", &iPercentR, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp)) {
            if (iPercentR <= 0 && iPercentS <= 0) {
                pSM->SetWeather(CN3SkyMng::SW_RAINY, 0);
                pSM->SetWeather(CN3SkyMng::SW_CLEAR, iPercentR);
            } else if (iPercentR > 0) {
                float fPercent = iPercentR / 100.0f;

                iPercentS = 0;
                pSM->SetWeather(CN3SkyMng::SW_CLEAR, 0);
                pSM->SetWeather(CN3SkyMng::SW_RAINY, iPercentR);

                float fDelta = 1.0f;
                fDelta = 0.25f + (1.0f - fPercent) * 0.75f;
                CGameProcedure::s_pEng->FarPlaneDeltaSet(fDelta, false);
            }
        }
        if (ImGui::DragInt("Snow", &iPercentS, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp)) {
            if (iPercentS <= 0 && iPercentR <= 0) {
                pSM->SetWeather(CN3SkyMng::SW_SNOW, 0);
                pSM->SetWeather(CN3SkyMng::SW_CLEAR, iPercentS);
            } else if (iPercentS > 0) {
                float fPercent = iPercentR / 100.0f;

                iPercentR = 0;
                pSM->SetWeather(CN3SkyMng::SW_CLEAR, 0);
                pSM->SetWeather(CN3SkyMng::SW_SNOW, iPercentS);

                float fDelta = 1.0f;
                fDelta = 0.25f + (1.0f - fPercent) * 0.75f;
                CGameProcedure::s_pEng->FarPlaneDeltaSet(fDelta, false);
            }
        }
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

    static float s_fFrmsDelay = 0.25f;
    static int   s_iFrmsCount = 50;
    static bool  s_bFrmsPause = false;

    if (!s_bFrmsPause) {
        s_fTimeInterval += fTime - s_fTimePrev;
        if (s_fTimeInterval >= s_fFrmsDelay) {
            s_fTimeInterval = 0.0f;
            s_fCurFrmPerSec = CN3Base::s_fFrmPerSec;
            s_Frames.push_back(s_fCurFrmPerSec);
            if (s_Frames.size() > s_iFrmsCount) {
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
    ImGui::Checkbox("Pause", &s_bFrmsPause);

    static int s_iFrmsCountTmp = s_iFrmsCount;
    if (ImGui::InputInt("Buffer Count", &s_iFrmsCountTmp, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (s_iFrmsCountTmp < s_iFrmsCount && s_Frames.size() > (s_iFrmsCount - s_iFrmsCountTmp)) {
            s_Frames.erase(s_Frames.begin(), s_Frames.begin() + (s_iFrmsCount - s_iFrmsCountTmp));
        }

        if (s_iFrmsCountTmp < 5) {
            s_iFrmsCount = s_iFrmsCountTmp = 5;
        } else if (s_iFrmsCountTmp > 2000) {
            s_iFrmsCount = s_iFrmsCountTmp = 2000;
        } else {
            s_iFrmsCount = s_iFrmsCountTmp;
        }
    }

    static float s_fFrmsDelayTmp = s_fFrmsDelay;
    if (ImGui::InputFloat("Delay", &s_fFrmsDelayTmp, 0.01f, 100.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (s_fFrmsDelayTmp < 0.0f) {
            s_fFrmsDelay = s_fFrmsDelayTmp = 0.0f;
        } else if (s_fFrmsDelayTmp > 100.0f) {
            s_fFrmsDelay = s_fFrmsDelayTmp = 100.0f;
        } else {
            s_fFrmsDelay = s_fFrmsDelayTmp;
        }
    }

    ImGui::End();
}
