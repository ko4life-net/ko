// SowSeedMng.cpp: implementation of the CSowSeedMng class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MainFrm.h"
#include "MapMng.h"
#include "N3Base/N3Shape.h"

#include "SowSeedMng.h"
#include "DlgSowSeed.h"
#include "DlgShapeList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSowSeedMng::CSowSeedMng() {
    Grass_Group.clear();
    bActive = FALSE;
    ptSelHeightMapPos.x = 0;
    ptSelHeightMapPos.y = 0;
    Grass = NULL;
    Grass_Count = 0;
    Render_Grass = FALSE;
    Select_Group_Id = -1;
    Add_List_Count = 0;

    m_pVertices = new __VertexColor[8];
};
CSowSeedMng::~CSowSeedMng() {
    Release();
};

void CSowSeedMng::Release() {
    it_Grass_Group it = Grass_Group.begin();
    for (int i = 0; i < Grass_Group.size(); i++, it++) {
        LPGRASS_GROUP group = (LPGRASS_GROUP)*it;
        it_Grass      it_grass = group->grass.begin();
        for (int j = 0; j < group->grass.size(); j++, it_grass++) {
            LPGRASS grass = *it_grass;
            if (grass != NULL) {
                delete[] grass;
            }
        }
        group->grass.clear();
        if (group != NULL) {
            delete group;
        }
    }
    Grass_Group.clear();

    if (Grass != NULL) {
        for (int i = 0; i < Grass_Count; i++) {
            if (Grass[i].Sub_Grass != NULL) {
                delete Grass[i].Sub_Grass;
            }
        }
        delete Grass;
    }

    Obj_Name.clear();
};

void CSowSeedMng::SetActive(BOOL flage) {
    bActive = flage;
};
BOOL CSowSeedMng::MouseMessage(LPMSG pMsg) {
    if (bActive == FALSE) {
        return FALSE;
    }

    static int   iPrevScreenY = 0;
    const float  fDelta = 0.10f;
    static int   iSumOfEditedHeight = 0; // Sum of terrain height changed by this drag
    CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();

    switch (pMsg->message) {
    case WM_MOUSEMOVE: {
        DWORD nFlags = pMsg->wParam;
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};

        if (pFrame->m_pDlgSowSeed->Sow_Select_Flage == CS_SOW) {
            // update brush
            if (pFrame->GetMapMng()->GetTerrain()->Pick(point.x, point.y, NULL, &ptSelHeightMapPos)) {
                pFrame->GetMapMng()->GetTerrain()->UpdateBrushArea(ptSelHeightMapPos);
            }
        } else if (pFrame->m_pDlgSowSeed->Sow_Select_Flage == CS_SELECT) {
        }
    }
        return TRUE;
    case WM_LBUTTONDOWN: {
        POINT point = {short(LOWORD(pMsg->lParam)), short(HIWORD(pMsg->lParam))};
        if (pFrame->m_pDlgSowSeed->Sow_Select_Flage == CS_SOW) {
            Render_Grass = FALSE;
            Add_Grass();
        } else if (pFrame->m_pDlgSowSeed->Sow_Select_Flage == CS_SELECT) {
            Select_Group_Id = SelectSeed(point);
            if (Select_Group_Id >= 0) {
                SetListPos();
            }
        }
    }
        return TRUE;
    case WM_LBUTTONUP: {
    }
        return TRUE;
    }
    return TRUE;
};

void CSowSeedMng::Create_Grass(int Num) {
    if (Grass != NULL) {
        delete[] Grass;
    }

    Grass = (LPGRASS) new GRASS[Num];
    Grass_Count = Num;

    for (int i = 0; i < Grass_Count; i++) {
        Grass[i].Tile_x = -1;
        Grass[i].Tile_z = -1;
        Grass[i].Pos = __Vector3(0.0f, 0.0f, 0.0f);
        Grass[i].Sub_Grass = NULL;
        Grass[i].Sub_Grass_Count = 0;
    }
};

void CSowSeedMng::Random_Grass(void) {
    CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
    for (int i = 0, Num = 0; i < MAX_BRUSH_SIZE; ++i) {
        for (int j = 0; j < MAX_BRUSH_SIZE; ++j) {
            if (pFrame->GetMapMng()->GetTerrain()->m_fBrushIntensityMap[i][j] <= 0) {
                continue;
            }

            //            if(pFrame->m_pDlgSowSeed->Sow_Array[i][j] == 0)
            //                continue;

            //            if( (pFrame->m_pDlgSowSeed->Sow_Array[i][j] == 2) || (pFrame->m_pDlgSowSeed->Sow_Array[i][j] == 1))
            {
                if (Grass[Num].Sub_Grass != NULL) {
                    delete[] Grass[Num].Sub_Grass;
                    Grass[Num].Sub_Grass = NULL;
                }

                Grass[Num].Sub_Grass_Count = rand() % pFrame->m_pDlgSowSeed->SowDetail;
                Grass[Num].Sub_Grass = (LPGRASS) new GRASS[Grass[Num].Sub_Grass_Count];
                for (int k = 0; k < Grass[Num].Sub_Grass_Count; k++) {
                    int rad = rand() % 4;
                    if (rad == 0) {
                        Grass[Num].Sub_Grass[k].Detx = (rand() % 20) * 0.1f;
                        Grass[Num].Sub_Grass[k].Detz = (rand() % 20) * 0.1f;
                        Grass[Num].roll = rand() % 360;
                    } else if (rad == 1) {
                        Grass[Num].Sub_Grass[k].Detx = -(rand() % 20) * 0.1f;
                        Grass[Num].Sub_Grass[k].Detz = (rand() % 20) * 0.1f;
                        Grass[Num].roll = rand() % 360;
                    } else if (rad == 2) {
                        Grass[Num].Sub_Grass[k].Detx = -(rand() % 20) * 0.1f;
                        Grass[Num].Sub_Grass[k].Detz = -(rand() % 20) * 0.1f;
                        Grass[Num].roll = rand() % 360;
                    } else if (rad == 3) {
                        Grass[Num].Sub_Grass[k].Detx = (rand() % 20) * 0.1f;
                        Grass[Num].Sub_Grass[k].Detz = -(rand() % 20) * 0.1f;
                        Grass[Num].roll = rand() % 360;
                    }

                    Grass[Num].Sub_Grass[k].Sub_Grass = NULL;
                }
                Num++;
            }
        }
    }
    Render_Grass = TRUE;
};

void CSowSeedMng::Render(LPDIRECT3DDEVICE9 lpD3DDevice) {
    if (bActive == FALSE) {
        return;
    }
    CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();

    if (pFrame->m_pDlgSowSeed->Sow_Select_Flage == CS_SOW) {
        // Grass selected on brush
        if (Render_Grass == TRUE) {
            pFrame->GetMapMng()->GetTerrain()->RenderBrushArea();
            for (int i = 0, Num = 0; i < MAX_BRUSH_SIZE; ++i) {
                for (int j = 0; j < MAX_BRUSH_SIZE; ++j) {
                    if (pFrame->GetMapMng()->GetTerrain()->m_fBrushIntensityMap[i][j] <= 0) {
                        continue;
                    }

                    //                if(pFrame->m_pDlgSowSeed->Sow_Array[i][j] == 0)
                    //                    continue;

                    //                if( (pFrame->m_pDlgSowSeed->Sow_Array[i][j] == 2) || (pFrame->m_pDlgSowSeed->Sow_Array[i][j] == 1) )
                    {
                        if (Grass != NULL) {
                            Grass[Num].Tile_x = ptSelHeightMapPos.x - BRUSH_CENTER + i;
                            Grass[Num].Tile_z = ptSelHeightMapPos.y - BRUSH_CENTER + j;

                            if (Grass[Num].Tile_x < 0 || Grass[Num].Tile_z < 0) {
                                continue;
                            }

                            float pos_x = (float)(Grass[Num].Tile_x * TERRAIN_CELL_SIZE);
                            float pos_z = (float)(Grass[Num].Tile_z * TERRAIN_CELL_SIZE);

                            Grass[Num].Pos.Set(pos_x,
                                               pFrame->GetMapMng()
                                                   ->GetTerrain()
                                                   ->m_ppMapData[Grass[Num].Tile_x][Grass[Num].Tile_z]
                                                   .fHeight,
                                               pos_z);

                            if (((CN3Shape *)pFrame->GetMapMng()->GetSelSourceObj())) {
                                ((CN3Shape *)pFrame->GetMapMng()->GetSelSourceObj())->PosSet(Grass[Num].Pos);
                                ((CN3Shape *)pFrame->GetMapMng()->GetSelSourceObj())->Render();
                            }

                            if (Grass[Num].Sub_Grass != NULL) {
                                for (int k = 0; k < Grass[Num].Sub_Grass_Count; k++) {
                                    Grass[Num].Sub_Grass[k].Tile_x = ptSelHeightMapPos.x - BRUSH_CENTER + i;
                                    Grass[Num].Sub_Grass[k].Tile_z = ptSelHeightMapPos.y - BRUSH_CENTER + j;

                                    Grass[Num].Sub_Grass[k].Pos.Set(
                                        pos_x + Grass[Num].Sub_Grass[k].Detx,
                                        pFrame->GetMapMng()
                                            ->GetTerrain()
                                            ->m_ppMapData[Grass[Num].Tile_x][Grass[Num].Tile_z]
                                            .fHeight,
                                        pos_z + Grass[Num].Sub_Grass[k].Detz);

                                    if (((CN3Shape *)pFrame->GetMapMng()->GetSelSourceObj())) {
                                        ((CN3Shape *)pFrame->GetMapMng()->GetSelSourceObj())
                                            ->PosSet(Grass[Num].Sub_Grass[k].Pos);
                                        ((CN3Shape *)pFrame->GetMapMng()->GetSelSourceObj())->Render();
                                    }
                                }
                            }
                            Num++;
                        }
                    }
                }
            }
        }
    }

    // Draw the grass on the map
    if (Render_Grass == TRUE) {
        it_Grass_Group it = Grass_Group.begin();
        for (int i = 0; i < Grass_Group.size(); i++, it++) {
            LPGRASS_GROUP group = *it;
            it_Grass      it_grass = group->grass.begin();

            if (Select_Group_Id != i) {
                for (int j = 0; j < group->grass.size(); j++, it_grass++) {
                    LPGRASS grass = *it_grass;

                    CN3Shape * pShape =
                        (CN3Shape *)pFrame->GetMapMng()->m_pDlgSourceList->m_ListShape.GetItemDataPtr(group->Obj_ID);
                    if ((CN3Shape *)pShape) {
                        pShape->PosSet(grass->Pos);
                        pShape->Tick();
                        if (pShape->m_bDontRender == false) {
                            pShape->Render();
                        }
                    }
                }
            } else if (Select_Group_Id == i) // selected group
            {
                Render_Box(lpD3DDevice, group->Pos);
                for (int j = 0; j < group->grass.size(); j++, it_grass++) {
                    LPGRASS grass = *it_grass;

                    CN3Shape * pShape =
                        (CN3Shape *)pFrame->GetMapMng()->m_pDlgSourceList->m_ListShape.GetItemDataPtr(group->Obj_ID);
                    if ((CN3Shape *)pShape) {
                        pShape->PosSet(grass->Pos);
                        pShape->Tick();
                        if (pShape->m_bDontRender == false) {
                            pShape->Render();
                        }
                    }
                }
            }
        }
    }
};

void CSowSeedMng::Add_Grass(void) {
    if (Grass != NULL) {
        __Vector3    Max = __Vector3(-99999.0f, -999999.0f, -999999.0f);
        __Vector3    Min = __Vector3(99999.0f, 999999.0f, 999999.0f);
        CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
        pFrame->m_pDlgSowSeed->m_CB_Seed.ResetContent();

        bool add_list_group = false;
        bool add_list = false;

        int           Grass_ID = Add_List_Count;
        LPGRASS_GROUP temp = new GRASS_GROUP;

        temp->grass.clear();
        temp->Group_id = Grass_ID;
        temp->Obj_ID = pFrame->m_pDlgSowSeed->Obj_Index;
        temp->b_size = pFrame->m_pDlgSowSeed->Brush_Size;

        char Name[256];

        CN3Shape * pShape = (CN3Shape *)pFrame->GetMapMng()->m_pDlgSourceList->m_ListShape.GetItemDataPtr(temp->Obj_ID);
        strcpy(temp->FileName, pShape->FileName().c_str());

        //        temp->Pos = Grass[0].Pos;
        for (int i = 0, listCount = 0; i < Grass_Count; i++) {
            LPGRASS grass = new GRASS;
            grass->Sub_Grass = NULL;
            grass->Tile_x = Grass[i].Tile_x;
            grass->Tile_z = Grass[i].Tile_z;
            grass->Pos = Grass[i].Pos;

            if (Max.x < grass->Pos.x) {
                Max.x = grass->Pos.x;
            }
            if (Max.y < grass->Pos.y) {
                Max.y = grass->Pos.y;
            }
            if (Max.z < grass->Pos.z) {
                Max.z = grass->Pos.z;
            }

            if (Min.x > grass->Pos.x) {
                Min.x = grass->Pos.x;
            }
            if (Min.y > grass->Pos.y) {
                Min.y = grass->Pos.y;
            }
            if (Min.z > grass->Pos.z) {
                Min.z = grass->Pos.z;
            }

            add_list_group = true;
            add_list = true;

            it_Grass_Group it = Grass_Group.begin();
            for (int ii = 0; ii < Grass_Group.size(); ii++, it++) {
                LPGRASS_GROUP group_list = *it;
                it_Grass      it_grass = group_list->grass.begin();
                for (int jj = 0; jj < group_list->grass.size(); jj++, it_grass++) {
                    LPGRASS grass_list = *it_grass;
                    if (grass->Pos == grass_list->Pos) {
                        add_list_group = false;
                        break;
                    }
                }
            }

            if ((add_list_group == true) && (grass->Tile_x > 0) && (grass->Tile_z > 0)) {
                temp->grass.push_back(grass);

                char text[256];
                sprintf(text, "SubID:%d, x :%f, y :%f, z :%f", listCount, grass->Pos.x, grass->Pos.y, grass->Pos.z);
                pFrame->m_pDlgSowSeed->m_CB_Seed.AddString(text);
                listCount++;
            }

            if (Grass[i].Sub_Grass != NULL) {
                for (int j = 0; j < Grass[i].Sub_Grass_Count; j++) {
                    LPGRASS grass_sub = new GRASS;
                    grass_sub->Sub_Grass = NULL;
                    grass_sub->Tile_x = Grass[i].Sub_Grass[j].Tile_x;
                    grass_sub->Tile_z = Grass[i].Sub_Grass[j].Tile_z;
                    grass_sub->Pos = Grass[i].Sub_Grass[j].Pos;
                    grass_sub->Detx = Grass[i].Sub_Grass[j].Detx;
                    grass_sub->Detz = Grass[i].Sub_Grass[j].Detz;

                    if (Max.x < grass_sub->Pos.x) {
                        Max.x = grass_sub->Pos.x;
                    }
                    if (Max.y < grass_sub->Pos.y) {
                        Max.y = grass_sub->Pos.y;
                    }
                    if (Max.z < grass_sub->Pos.z) {
                        Max.z = grass_sub->Pos.z;
                    }

                    if (Min.x > grass_sub->Pos.x) {
                        Min.x = grass_sub->Pos.x;
                    }
                    if (Min.y > grass_sub->Pos.y) {
                        Min.y = grass_sub->Pos.y;
                    }
                    if (Min.z > grass_sub->Pos.z) {
                        Min.z = grass_sub->Pos.z;
                    }

                    add_list = true;
                    it_Grass_Group it = Grass_Group.begin();
                    for (int ii = 0; ii < Grass_Group.size(); ii++, it++) {
                        LPGRASS_GROUP group_list = *it;
                        it_Grass      it_grass = group_list->grass.begin();
                        for (int jj = 0; jj < group_list->grass.size(); jj++, it_grass++) {
                            LPGRASS grass_list = *it_grass;
                            if (grass_sub->Pos == grass_list->Pos) {
                                add_list = false;
                                break;
                            }
                        }
                    }

                    if ((add_list == true) && (grass_sub->Tile_x > 0) && (grass_sub->Tile_z > 0)) {
                        temp->grass.push_back(grass_sub);
                        char text[256];
                        sprintf(text, "SubID:%d, x :%f, y :%f, z :%f", listCount, grass_sub->Pos.x, grass_sub->Pos.y,
                                grass_sub->Pos.z);
                        pFrame->m_pDlgSowSeed->m_CB_Seed.AddString(text);
                        listCount++;
                    }
                }
            }
            int CurPos = pFrame->m_pDlgSowSeed->m_CB_Seed.GetCount();
            pFrame->m_pDlgSowSeed->m_CB_Seed.SetCurSel(CurPos);
        }

        if ((add_list == true) || (add_list_group == true)) {
            __Vector3 mid = (Max + Min) / 2;
            temp->Pos.Set(mid.x, mid.y, mid.z);
            Grass_Group.push_back(temp);
        }

        int Count = 0;
        for (int i = 0; i < MAX_BRUSH_SIZE; i++) {
            for (int j = 0; j < MAX_BRUSH_SIZE; j++) {
                if (pFrame->GetMapMng()->GetTerrain()->m_fBrushIntensityMap[i][j] <= 0) {
                    continue;
                }

                pFrame->m_pDlgSowSeed->Sow_Array[i][j] = rand() % 3;
                Count++;
            }
        }

        Create_Grass(Count);
        Random_Grass();
        Render_Grass = TRUE;

        pFrame->m_pDlgSowSeed->m_CB_TileGroup.GetLBText(0, Name);
        if (strcmp(Name, "NONE") == 0) {
            pFrame->m_pDlgSowSeed->m_CB_TileGroup.DeleteString(0);
        }

        pFrame->GetMapMng()->m_pDlgSourceList->m_ListShape.GetText(temp->Obj_ID, Name);

        char text[256];
        sprintf(text, "Pool ID: %d , File name:%s", Grass_ID, Name);
        int CurPos = pFrame->m_pDlgSowSeed->m_CB_TileGroup.GetCount();

        pFrame->m_pDlgSowSeed->m_CB_TileGroup.AddString(text);
        pFrame->m_pDlgSowSeed->m_CB_TileGroup.SetCurSel(CurPos);

        Add_List_Count++;
    }
};

int CSowSeedMng::SelectSeed(POINT point) {
    __Vector3    vPos;
    CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();

    if (pFrame->GetMapMng()->GetTerrain()->Pick(point.x, point.y, &vPos)) {
        it_Grass_Group it = Grass_Group.begin();
        for (int i = 0; i < Grass_Group.size(); i++, it++) {
            LPGRASS_GROUP group = *it;
            it_Grass      it_grass = group->grass.begin();
            for (int j = 0; j < group->grass.size(); j++, it_grass++) {
                LPGRASS grass = *it_grass;

                CN3Shape * pShape =
                    (CN3Shape *)pFrame->GetMapMng()->m_pDlgSourceList->m_ListShape.GetItemDataPtr(group->Obj_ID);
                if ((CN3Shape *)pShape) {
                    pShape->PosSet(grass->Pos);
                    pShape->Tick();
                    if (pShape->m_bDontRender == false) {
                        int check_id = pShape->CheckCollisionPrecisely(true, point.x, point.y);
                        if (check_id >= 0) {
                            CreateBox(__Vector3(group->b_size * TERRAIN_CELL_SIZE / 2, 1.0f,
                                                group->b_size * TERRAIN_CELL_SIZE / 2),
                                      __Vector3(-(group->b_size * TERRAIN_CELL_SIZE / 2), 0.0f,
                                                -(group->b_size * TERRAIN_CELL_SIZE / 2)));
                            return i;
                        }
                    }
                }
            }
        }
    }
    return -1;
};

void CSowSeedMng::SetListPos(void) {
    CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
    if (Grass_Group.size() > Select_Group_Id) {
        pFrame->m_pDlgSowSeed->m_CB_Seed.ResetContent();

        it_Grass_Group it = Grass_Group.begin();
        for (int i = 0, listCount = 0; i < Grass_Group.size(); i++, it++) {
            if (i == Select_Group_Id) {
                LPGRASS_GROUP group = *it;
                it_Grass      it_grass = group->grass.begin();
                for (int j = 0; j < group->grass.size(); j++, it_grass++) {
                    LPGRASS grass = *it_grass;
                    char    text[256];
                    sprintf(text, "SubID:%d, x :%f, y :%f, z :%f", listCount, grass->Pos.x, grass->Pos.y, grass->Pos.z);
                    pFrame->m_pDlgSowSeed->m_CB_Seed.AddString(text);
                    listCount++;
                }
                pFrame->m_pDlgSowSeed->m_CB_TileGroup.SetCurSel(Select_Group_Id);
                return;
            }
        }

        pFrame->m_pDlgSowSeed->m_CB_TileGroup.SetCurSel(Select_Group_Id);
    }
};

void CSowSeedMng::CreateBox(__Vector3 vMax, __Vector3 vMin) {
    m_pVertices[0].Set(vMin.x, vMax.y, vMin.z, 0xffff0000);
    m_pVertices[1].Set(vMax.x, vMax.y, vMin.z, 0xffff0000);
    m_pVertices[2].Set(vMax.x, vMin.y, vMin.z, 0xffff0000);
    m_pVertices[3].Set(vMin.x, vMin.y, vMin.z, 0xffff0000);
    m_pVertices[4].Set(vMax.x, vMax.y, vMax.z, 0xffff0000);
    m_pVertices[5].Set(vMin.x, vMax.y, vMax.z, 0xffff0000);
    m_pVertices[6].Set(vMin.x, vMin.y, vMax.z, 0xffff0000);
    m_pVertices[7].Set(vMax.x, vMin.y, vMax.z, 0xffff0000);
};

void CSowSeedMng::Render_Box(LPDIRECT3DDEVICE9 lpD3DDevice, __Vector3 Pos) {
    WORD  m_Index[26] = {0, 1, 1, 4, 4, 5, 5, 0, 3, 2, 2, 7, 7, 6, 6, 3, 1, 2, 0, 3, 1, 2, 4, 7, 5, 6};
    DWORD dwLight;
    lpD3DDevice->GetRenderState(D3DRS_LIGHTING, &dwLight);
    if (dwLight) {
        lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    }

    CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
    __Matrix44   WorldMtx;
    WorldMtx.Identity();
    WorldMtx.PosSet(Pos);
    lpD3DDevice->SetTransform(D3DTS_WORLD, &WorldMtx);

    lpD3DDevice->SetTexture(0, NULL);
    lpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

    lpD3DDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 13, m_Index, D3DFMT_INDEX16, m_pVertices,
                                        sizeof(__VertexColor));

    if (dwLight) {
        lpD3DDevice->SetRenderState(D3DRS_LIGHTING, dwLight);
    }
};

void CSowSeedMng::SaveData(void) {
    // Read Seed List...
    DWORD       dwFlags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_LONGNAMES | OFN_OVERWRITEPROMPT;
    CFileDialog dlg(FALSE, "tgi", NULL, dwFlags, "Grass Info File(*.tgi)|*.tgi||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    int size = Grass_Group.size();
    if (size > 0) {
        FILE * fp = fopen((LPCTSTR)dlg.GetPathName(), "w");
        // Total number of gets in the group
        fwrite(&size, sizeof(int), 1, fp);
        it_Grass_Group it = Grass_Group.begin();
        for (int i = 0; i < size; i++, it++) {
            LPGRASS_GROUP group = *it;
            // brush size
            fwrite(&group->b_size, sizeof(group->b_size), 1, fp);
            // group ID
            fwrite(&group->Group_id, sizeof(group->Group_id), 1, fp);
            // object ID
            fwrite(&group->Obj_ID, sizeof(group->Obj_ID), 1, fp);
            // group position
            fwrite(group->Pos, sizeof(group->Pos), 1, fp);
            // subgroup size
            int grass_size = group->grass.size();
            fwrite(&grass_size, sizeof(grass_size), 1, fp);

            // Write file name
            int len = strlen(group->FileName);
            fwrite(&len, sizeof(int), 1, fp);
            fwrite(group->FileName, len, 1, fp);

            it_Grass it_grass = group->grass.begin();
            for (int j = 0; j < grass_size; j++, it_grass++) {
                LPGRASS grass = *it_grass;
                // location of the pool
                fwrite(grass->Pos, sizeof(grass->Pos), 1, fp);
                // tile number in the pool
                fwrite(&grass->Tile_x, sizeof(grass->Tile_x), 1, fp);
                fwrite(&grass->Tile_z, sizeof(grass->Tile_z), 1, fp);
            }
        }
        fclose(fp);
    }

    //    Test_GameDataSave();
};
void CSowSeedMng::LoadData(void) {
    char szOldPath[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szOldPath);

    // Read Seed List...
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(TRUE, "tgi", NULL, dwFlags, "Grass Info File(*.tgi)|*.tgi||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    // list initialization
    if (Grass_Group.size() > 0) {
        Render_Grass = FALSE;
        it_Grass_Group it = Grass_Group.begin();
        for (int i = 0; i < Grass_Group.size(); i++, it++) {
            LPGRASS_GROUP group = (LPGRASS_GROUP)*it;
            it_Grass      it_grass = group->grass.begin();
            for (int j = 0; j < group->grass.size(); j++, it_grass++) {
                LPGRASS grass = *it_grass;
                if (grass != NULL) {
                    delete[] grass;
                }
            }
            group->grass.clear();
            if (group != NULL) {
                delete group;
            }
        }
        Grass_Group.clear();
    }
    // start loading
    FILE * fp = fopen((LPCTSTR)dlg.GetPathName(), "r");

    int size = 0;

    fread(&size, sizeof(size), 1, fp);

    for (int i = 0; i < size; i++) {
        LPGRASS_GROUP group = new GRASS_GROUP;
        fread(&group->b_size, sizeof(group->b_size), 1, fp);
        fread(&group->Group_id, sizeof(group->Group_id), 1, fp);
        fread(&group->Obj_ID, sizeof(group->Obj_ID), 1, fp);
        fread(group->Pos, sizeof(group->Pos), 1, fp);
        int grass_sub_size = 0;
        fread(&grass_sub_size, sizeof(grass_sub_size), 1, fp);

        // read file name
        int len = 0;
        fread(&len, sizeof(int), 1, fp);
        fread(group->FileName, len, 1, fp);

        for (int j = 0; j < grass_sub_size; j++) {
            LPGRASS grass = new GRASS;
            // location of the pool
            fread(grass->Pos, sizeof(grass->Pos), 1, fp);
            // tile number in the pool
            fread(&grass->Tile_x, sizeof(grass->Tile_x), 1, fp);
            fread(&grass->Tile_z, sizeof(grass->Tile_z), 1, fp);
            group->grass.push_back(grass);
        }

        Grass_Group.push_back(group);
    }

    SetCurrentDirectory(szOldPath);

    fclose(fp);

    //    Test_GameDataLoad();
};

void CSowSeedMng::SaveDataGame(void) {
    CMainFrame * m_pRefFrm = (CMainFrame *)AfxGetMainWnd();

    if (m_pRefFrm->m_SeedFileName[0] == NULL) {
        ::MessageBox(NULL, "Save the map file for use in the game.", "Warning", MB_OK);
        return;
    }

    char szOldPath[_MAX_PATH];
    GetCurrentDirectory(_MAX_PATH, szOldPath);

    char szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
    char szModuleFilePath[_MAX_PATH];
    GetModuleFileName(NULL, szModuleFilePath, _MAX_PATH);
    char szCurrDir[MAX_PATH];
    char szNewDir[MAX_PATH];
    _splitpath(szModuleFilePath, szDrive, szDir, NULL, NULL);
    _makepath(szCurrDir, szDrive, szDir, NULL, NULL);
    SetCurrentDirectory(szCurrDir);

    CreateDirectory("Seeds", NULL);
    sprintf(szNewDir, "%s\\Seeds", szCurrDir);
    SetCurrentDirectory(szNewDir);

    char szNewPath[_MAX_PATH];
    //char szNewFName[_MAX_PATH];
    //_splitpath(szModuleFilePath, szDrive, szDir, NULL, NULL);
    _makepath(szNewPath, NULL, NULL, m_pRefFrm->m_SeedFileName, "grs");

    HANDLE hFile = CreateFile(szNewPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD dwRWC;

    char Buff[80];
    sprintf(Buff, "GrassInfoFile");
    WriteFile(hFile, Buff, 80, &dwRWC, NULL);

    // group size
    Obj_Name.clear();
    it_Grass_Group it = Grass_Group.begin();
    for (int i = 0, Object_ID = 0; i < Grass_Group.size(); i++, it++) {
        LPGRASS_GROUP group = *it;
        BOOL          back_push = TRUE;
        it_Obj_Name   it_Obj = Obj_Name.begin();
        for (int j = 0; j < Obj_Name.size(); j++, it_Obj++) {
            LPOBJ_NAME Obj = *it_Obj;
            if (strcmp(group->FileName, Obj->FileName) == 0) {
                back_push = FALSE;
            }
        }
        if (back_push == TRUE) {
            LPOBJ_NAME File_Name = new OBJ_NAME;
            strcpy(File_Name->FileName, group->FileName);
            File_Name->Id = Object_ID;
            Obj_Name.push_back(File_Name);
            Object_ID++;
        }
    }

    int iNum = Obj_Name.size();
    WriteFile(hFile, &iNum, sizeof(int), &dwRWC, NULL);

    it_Obj_Name it_Obj = Obj_Name.begin();
    for (int j = 0; j < Obj_Name.size(); j++, it_Obj++) {
        LPOBJ_NAME Obj = *it_Obj;
        int        len = strlen(Obj->FileName);
        WriteFile(hFile, &len, sizeof(int), &dwRWC, NULL);
        WriteFile(hFile, Obj->FileName, strlen(Obj->FileName), &dwRWC, NULL);
    }

    CloseHandle(hFile);

    SetCurrentDirectory(szOldPath);
}

void CSowSeedMng::Test_GameDataSave(void) {
    DWORD       dwRWC = 0;
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(FALSE, "tst", NULL, dwFlags, "Grass Info File(*.tst)|*.tst||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    HANDLE hFile = CreateFile(dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    int          Map_Size = pFrm->GetMapMng()->GetTerrain()->m_iHeightMapSize;
    //Save pool properties to tile..
    LPSEEDGROUP SeedAttr = new SEEDGROUP[Map_Size * Map_Size];
    ZeroMemory(SeedAttr, sizeof(unsigned char) * Map_Size * Map_Size);

    for (int i = 0; i < Map_Size * Map_Size; i++) {
        SeedAttr[i].Obj_Id = 0;
        SeedAttr[i].Seed_Count = 0;
        SeedAttr[i].SeedGroup_Sub = NULL;
        SeedAttr[i].sub_flage = 0;
    }
    int            size = pFrm->GetMapMng()->m_SowSeedMng.Grass_Group.size();
    it_Grass_Group it = pFrm->GetMapMng()->m_SowSeedMng.Grass_Group.begin();
    for (int i = 0; i < size; i++, it++) {
        LPGRASS_GROUP group = (LPGRASS_GROUP)*it;
        it_Grass      it_grass = group->grass.begin();
        for (int j = 0; j < group->grass.size(); j++, it_grass++) {
            LPGRASS grass = *it_grass;

            if (SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Obj_Id == 0) {
                SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Obj_Id = group->Group_id + 1;

                if (SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Seed_Count < 15) {
                    SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Seed_Count += 1;
                }
            } else {
                if (SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Obj_Id == group->Group_id + 1) {
                    if (SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Seed_Count < 15) {
                        SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].Seed_Count += 1;
                    }
                } else {
                    if (SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].SeedGroup_Sub == NULL) {
                        SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].sub_flage = 1;
                        SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].SeedGroup_Sub = new SEEDGROUP;
                    }

                    SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].SeedGroup_Sub->Obj_Id = group->Group_id + 1;
                    if (SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].SeedGroup_Sub->Seed_Count < 15) {
                        SeedAttr[grass->Tile_z + (grass->Tile_x * Map_Size)].SeedGroup_Sub->Seed_Count += 1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < Map_Size * Map_Size; i++) {
        WriteFile(hFile, &SeedAttr[i], sizeof(unsigned char), &dwRWC, NULL);
        if (SeedAttr[i].sub_flage == 1) {
            WriteFile(hFile, SeedAttr[i].SeedGroup_Sub, sizeof(unsigned char), &dwRWC, NULL);
        }
    }

    CloseHandle(hFile);
}

void CSowSeedMng::Test_GameDataLoad(void) {
    DWORD       dwRWC = 0;
    DWORD       dwFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
    CFileDialog dlg(TRUE, "tst", NULL, dwFlags, "Grass Info File(*.tst)|*.tst||", NULL);

    if (dlg.DoModal() == IDCANCEL) {
        return;
    }

    //    HANDLE hFile = CreateFile(dlg.GetPathName(), GENERIC_READ, 0, NULL,
    //        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    FILE * fp = fopen((LPCTSTR)dlg.GetPathName(), "r");

    CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
    int          Map_Size = pFrm->GetMapMng()->GetTerrain()->m_iHeightMapSize;
    //Save pool properties to tile..
    LPSEEDGROUP SeedAttr = new SEEDGROUP[Map_Size * Map_Size];
    ZeroMemory(SeedAttr, sizeof(unsigned char) * Map_Size * Map_Size);

    for (int i = 0; i < Map_Size * Map_Size; i++) {
        SeedAttr[i].Obj_Id = 0;
        SeedAttr[i].Seed_Count = 0;
        SeedAttr[i].SeedGroup_Sub = NULL;
        SeedAttr[i].sub_flage = 0;
    }

    for (int i = 0; i < Map_Size * Map_Size; i++) {
        //        ReadFile(hFile,&SeedAttr[i],sizeof(unsigned char),&dwRWC,NULL);
        fread(&SeedAttr[i], sizeof(unsigned char), 1, fp);
        if (SeedAttr[i].sub_flage == 1) {
            SeedAttr[i].SeedGroup_Sub = new SEEDGROUP;
            //            ReadFile(hFile,SeedAttr[i].SeedGroup_Sub,sizeof(unsigned char),&dwRWC,NULL);
            fread(SeedAttr[i].SeedGroup_Sub, sizeof(unsigned char), 1, fp);
        }
    }

    // Let's extract it as a text file.
    FILE * stream = fopen("c:\\grass.txt", "w");
    for (int z = 0; z < Map_Size; z++) {
        for (int x = 0; x < Map_Size; x++) {
            SEEDGROUP v = SeedAttr[z + (x * Map_Size)];
            fprintf(stream, "%d,%d\t", v.Obj_Id, v.Seed_Count);

            if (v.SeedGroup_Sub != NULL) {
                fprintf(stream, "���� %d,%d\t", v.SeedGroup_Sub->Obj_Id, v.SeedGroup_Sub->Seed_Count);
            }

            fprintf(stream, "\n");
        }
    }
    fclose(stream);

    //    CloseHandle(hFile);
    fclose(fp);
}