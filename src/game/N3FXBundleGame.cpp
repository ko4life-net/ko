// N3FXBundleGame.cpp: implementation of the CN3FXBundleGame class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3FXBundleGame.h"
#include "GameProcMain.h"
#include "PlayerBase.h"
#include "N3Base/N3SndObj.h"
#include "N3Terrain.h"
#include "N3Base/N3FXPartMesh.h"
#include "N3Base/N3FXPartParticles.h"
//#include "N3Base/N3FXPartBillBoard.h"
#include "N3FXPartBillBoardGame.h"
#include "N3FXPartBottomBoardGame.h"
#include "N3Base/N3ShapeExtra.h"

#include "GameEng.h"
#include "N3Base/N3Camera.h"

/////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////

CN3FXBundleGame::CN3FXBundleGame() {
    m_iIdx = 0;
    m_iID = -1;
    m_bRegion = false;
}

CN3FXBundleGame::~CN3FXBundleGame() {}

void CN3FXBundleGame::Trigger(int iSourceID, int iTargetID, int iTargetJoint, int iSndID) {
    m_bRegion = false;

    CPlayerBase * pTarget = CGameProcedure::s_pProcMain->CharacterGetByID(iTargetID, false);
    if (!pTarget) {
        return;
    }

    CPlayerBase * pSource = CGameProcedure::s_pProcMain->CharacterGetByID(iSourceID, true);
    if (pSource) {
        if (pSource->m_pShapeExtraRef) {
            __Vector3 vMin = pSource->m_pShapeExtraRef->Min();
            __Vector3 vMax = pSource->m_pShapeExtraRef->Max();
            m_vPos = vMin + ((vMax - vMin) * 0.5f);
        } else {
            const __Matrix44 * pMtx = pSource->JointMatrixGet(m_iSourceJoint);
            if (!pMtx) {
                m_vPos = pSource->Position();
            } else {
                pSource->JointPosGet(m_iSourceJoint, m_vPos);
            }
        }

        m_vDestPos = pSource->Position() + pSource->Direction();
    }

    //CPlayerBase* pTarget = CGameProcedure::s_pProcMain->CharacterGetByID(iTargetID, false);
    if (pTarget && pTarget != pSource) {
        if (m_bDependScale) {
            //m_vTargetScale.x = m_vTargetScale.z = pTarget->Radius() * 2.0f;
            //m_vTargetScale.y = pTarget->Height();
            float width = pTarget->Radius() * 2.0f;
            if (width > pTarget->Height()) {
                m_fTargetScale = width;
            } else {
                m_fTargetScale = pTarget->Height();
            }
        }

        if (pTarget->m_pShapeExtraRef) {
            __Vector3 vMin = pTarget->m_pShapeExtraRef->Min();
            __Vector3 vMax = pTarget->m_pShapeExtraRef->Max();

            m_vDestPos = vMin + ((vMax - vMin) * 0.5f);
        } else {
            __Vector3 vMin = pTarget->Min();
            __Vector3 vMax = pTarget->Max();
            m_vDestPos = vMin + ((vMax - vMin) * 0.5f);
            //m_vDestPos = pTarget->Position();

            if (iTargetJoint == -1) {
                __Vector3 vMin = pTarget->Min();
                __Vector3 vMax = pTarget->Max();

                m_vDestPos = vMin + ((vMax - vMin) * 0.5f);
                m_vDestPos.y = vMin.y;
                //m_vDestPos = pTarget->Position();
            } else if (iTargetJoint > -1) {
                const __Matrix44 * pMtx = pTarget->JointMatrixGet(iTargetJoint);
                if (!pMtx) {
                    m_vDestPos = pTarget->Position() + (pTarget->Height() * 0.5f);
                } else {
                    pTarget->JointPosGet(iTargetJoint, m_vDestPos);
                }
            }
        }
        //    m_vDir = pTarget->Direction();
        //    m_vDir.Normalize();
    }

    m_fDistance = (m_vDestPos - m_vPos).Magnitude();
    m_fHeight = m_fDistance / 2.0f;

    m_vDir = m_vDestPos - m_vPos;
    m_vDir.Normalize();

    if (m_bStatic) {
        Trigger(iSourceID, m_vDestPos, iSndID);
        return;
    }

    CN3FXBundle::Trigger(iSourceID, iTargetID, iTargetJoint, iSndID);
}

void CN3FXBundleGame::Trigger(int iSourceID, __Vector3 TargetPos, int iSndID) {
    m_bRegion = true;

    CPlayerBase * pSource = CGameProcedure::s_pProcMain->CharacterGetByID(iSourceID, true);
    if (pSource) {
        if (pSource->m_pShapeExtraRef) {
            __Vector3 vMin = pSource->m_pShapeExtraRef->Min();
            __Vector3 vMax = pSource->m_pShapeExtraRef->Max();

            m_vPos = vMin + ((vMax - vMin) * 0.5f);
        } else {
            const __Matrix44 * pMtx = pSource->JointMatrixGet(m_iSourceJoint);
            if (!pMtx) {
                m_vPos = pSource->Position();
            } else {
                pSource->JointPosGet(m_iSourceJoint, m_vPos);
            }
        }
    }

    m_vDestPos = TargetPos;

    m_fDistance = (m_vDestPos - m_vPos).Magnitude();
    m_fHeight = m_fDistance / 2.0f;

    m_vDir = m_vDestPos - m_vPos;
    m_vDir.Normalize();
    CN3FXBundle::Trigger(iSourceID, -1, -1, iSndID);
}

bool CN3FXBundleGame::Tick() {
    if (m_dwState == FX_BUNDLE_STATE_DEAD) {
        return false;
    }

    m_fLife += CN3Base::s_fSecPerFrm;

    if (m_dwState == FX_BUNDLE_STATE_LIVE) {
        //    if(m_fLife>=m_fLife0) Stop();

        if (!m_bRegion) {
            CPlayerBase * pTarget = CGameProcedure::s_pProcMain->CharacterGetByID(m_iTargetID, false);

            if (pTarget && pTarget->m_pShapeExtraRef) {
                __Vector3 vMin = pTarget->m_pShapeExtraRef->Min();
                __Vector3 vMax = pTarget->m_pShapeExtraRef->Max();

                m_vDestPos = vMin + ((vMax - vMin) * 0.5f);
            } else if (pTarget && m_iTargetJoint == -1) {
                __Vector3 vMin = pTarget->Min();
                __Vector3 vMax = pTarget->Max();

                m_vDestPos = vMin + ((vMax - vMin) * 0.5f);
                m_vDestPos.y = vMin.y;
                //m_vDestPos = pTarget->Position();
            } else if (pTarget && m_iTargetJoint > -1) {
                const __Matrix44 * pMtx = pTarget->JointMatrixGet(m_iTargetJoint);
                if (!pMtx) {
                    m_vDestPos = pTarget->Position();
                } else {
                    pTarget->JointPosGet(m_iTargetJoint, m_vDestPos);
                }
            }
        }

        switch (m_iMoveType) {
        case FX_BUNDLE_MOVE_CURVE_FIXEDTARGET: {
            m_vPos.x = (m_vDir * CN3Base::s_fSecPerFrm * m_fVelocity).x;
            m_vPos.z = (m_vDir * CN3Base::s_fSecPerFrm * m_fVelocity).z;
            float fAng;
            if (m_fDistance != 0.0f) {
                fAng = __PI * (m_fDistance - (m_vDestPos - m_vPos).Magnitude()) / m_fDistance;
            } else {
                fAng = 0.0f;
            }

            m_vPos.y = sin(fAng) * m_fHeight;
            break;
        }
        case FX_BUNDLE_MOVE_DIR_SLOW:
        case FX_BUNDLE_MOVE_DIR_FIXEDTARGET: {
            m_vPos += m_vDir * CN3Base::s_fSecPerFrm * m_fVelocity;
            break;
        }
        case FX_BUNDLE_MOVE_DIR_FLEXABLETARGET_RATIO: {
            CPlayerBase * pTarget = CGameProcedure::s_pProcMain->CharacterGetByID(m_iTargetID, false);
            if (!pTarget) {
                m_vPos += m_vDir * CN3Base::s_fSecPerFrm * m_fVelocity;
                break;
            } else {
                /*
                    m_vDestPos = pTarget->Position();
                    const __Matrix44* pMtx = pTarget->JointMatrixGet(m_iTargetJoint);
                    if(pMtx) pTarget->JointPosGet(m_iTargetJoint, m_vDestPos);

                    __Vector3 vDestDir = m_vDestPos - m_vPos;
                    vDestDir.Normalize();

                    __Quaternion qtRot;
                    __Vector3 vDirAxis;
                    float fDirAng;
                    
                    vDirAxis.Cross(m_vDir, vDestDir);

                    int tmp;
                    tmp = vDirAxis.x*10000.0f;
                    vDirAxis.x = (float)(tmp)/10000.0f;
                    tmp = vDirAxis.y*10000.0f;
                    vDirAxis.y = (float)(tmp)/10000.0f;
                    tmp = vDirAxis.z*10000.0f;
                    vDirAxis.z = (float)(tmp)/10000.0f;
                    if(vDirAxis.x==0.0f && vDirAxis.y==0.0f && vDirAxis.z==0.0f) vDirAxis.Set(0,1,0);

                    fDirAng = acos((double)m_vDir.Dot(vDestDir));
                    if(fDirAng > __PI*s_fSecPerFrm) fDirAng = __PI*s_fSecPerFrm;
                    else if(fDirAng < -__PI*s_fSecPerFrm) fDirAng = -__PI*s_fSecPerFrm;

                    qtRot.RotationAxis(vDirAxis, fDirAng);

                    __Matrix44 mtxRot = qtRot;
                    m_vDir *= mtxRot;

                    m_vPos += m_vDir*CN3Base::s_fSecPerFrm*m_fVelocity;
                    
                    float fTerrainY = s_pTerrain->GetHeight(m_vPos.x, m_vPos.z);
                    if(m_vPos.y <= fTerrainY) m_vPos.y = fTerrainY + 0.3f;    //땅을 타고 날라가라..
                    break;
                    */
            }
            //break;
        }
        case FX_BUNDLE_MOVE_DIR_FLEXABLETARGET: {
            CPlayerBase * pTarget = CGameProcedure::s_pProcMain->CharacterGetByID(m_iTargetID, false);
            if (!pTarget) {
                m_vPos += m_vDir * CN3Base::s_fSecPerFrm * m_fVelocity;
                break;
            }

            if (pTarget && pTarget->m_pShapeExtraRef) {
                __Vector3 vMin = pTarget->m_pShapeExtraRef->Min();
                __Vector3 vMax = pTarget->m_pShapeExtraRef->Max();

                m_vDestPos = vMin + ((vMax - vMin) * 0.5f);
            }

            else {
                m_vDestPos = pTarget->Position();

                const __Matrix44 * pMtx = pTarget->JointMatrixGet(m_iTargetJoint);
                if (!pMtx) {
                    m_vDestPos = pTarget->Position();
                } else {
                    pTarget->JointPosGet(m_iTargetJoint, m_vDestPos);
                }
            }

            m_vDir = m_vDestPos - m_vPos;
            m_vDir.Normalize();

            m_vPos += m_vDir * CN3Base::s_fSecPerFrm * m_fVelocity;

            //float fTerrainY = s_pTerrain->GetHeight(m_vPos.x, m_vPos.z);
            //if(m_vPos.y <= fTerrainY) m_vPos.y = fTerrainY + 0.3f;    //땅을 타고 날라가라..
            break;
        }

        case FX_BUNDLE_MOVE_NONE: {
            //m_vDir.Set(0,0,1);
            m_vDir.y = 0.0f;
            m_vDir.Normalize();
            m_vPos = m_vDestPos;
            if (m_iSourceID == m_iTargetID) {
                CPlayerBase * pSource = CGameProcedure::s_pProcMain->CharacterGetByID(m_iSourceID, false);
                if (pSource) {
                    m_vDir = pSource->Direction();
                    m_vDir.y = 0.0f;
                    m_vDir.Normalize();
                }
            }
            break;
        }
        case FX_BUNDLE_REGION_POISON: {
            CN3Camera * pCamera = CGameProcedure::s_pEng->CameraGetActive(); // 활성화된 카메라 얻기..

            __Vector3 vEyePos = pCamera->EyePos();
            __Vector3 vEyeAt = pCamera->AtPos();
            __Vector3 vEyeDir = vEyeAt - vEyePos;
            vEyeDir.Normalize();
            m_vDir = vEyeDir;
            m_vPos = vEyePos + vEyeDir * CN3Base::s_CameraData.fNP * 3;
            break;
        }
        }
    }

    if (m_dwState == FX_BUNDLE_STATE_DYING || m_dwState == FX_BUNDLE_STATE_LIVE) {
        if (CheckAllPartsDead() || (m_fLife0 != 0.0f && m_fLife > m_fLife0)) {
            m_dwState = FX_BUNDLE_STATE_DEAD;
            Init();
            return false;
        }
    }

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i] && m_pPart[i]->pPart) {
            if (m_pPart[i]->fStartTime <= m_fLife && m_pPart[i]->pPart->m_dwState == FX_PART_STATE_READY) {
                m_pPart[i]->pPart->Start();
            }
            m_pPart[i]->pPart->Tick();
        }
    }

    return true;
}

void CN3FXBundleGame::Duplicate(CN3FXBundleGame * pDestBundle) {
    pDestBundle->FilePathSet(FilePath());

    pDestBundle->m_iVersion = m_iVersion;
    pDestBundle->m_fLife0 = m_fLife0;

    pDestBundle->m_fVelocity = m_fVelocity;
    pDestBundle->m_bDependScale = m_bDependScale;
    pDestBundle->m_bStatic = pDestBundle->m_bStatic;

    for (int i = 0; i < MAX_FX_PART; i++) {
        if (m_pPart[i]) {
            if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_PARTICLE) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartParticles;
                CN3FXPartParticles * pPart = (CN3FXPartParticles *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_PARTICLE;

                pPart->Duplicate((CN3FXPartParticles *)m_pPart[i]->pPart);
            } else if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_BOARD) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartBillBoardGame;
                CN3FXPartBillBoardGame * pPart = (CN3FXPartBillBoardGame *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_BOARD;

                pPart->Duplicate((CN3FXPartBillBoardGame *)m_pPart[i]->pPart);
            } else if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_MESH) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartMesh;
                CN3FXPartMesh * pPart = (CN3FXPartMesh *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_MESH;

                pPart->Duplicate((CN3FXPartMesh *)m_pPart[i]->pPart);

            } else if (m_pPart[i]->pPart->m_iType == FX_PART_TYPE_BOTTOMBOARD) {
                pDestBundle->m_pPart[i] = new FXPARTWITHSTARTTIME;

                pDestBundle->m_pPart[i]->fStartTime = m_pPart[i]->fStartTime;

                pDestBundle->m_pPart[i]->pPart = new CN3FXPartBottomBoardGame;
                CN3FXPartBottomBoardGame * pPart = (CN3FXPartBottomBoardGame *)pDestBundle->m_pPart[i]->pPart;

                pPart->m_pRefBundle = pDestBundle;
                pPart->m_pRefPrevPart = NULL;
                pPart->m_iType = FX_PART_TYPE_BOTTOMBOARD;

                pPart->Duplicate((CN3FXPartBottomBoardGame *)m_pPart[i]->pPart);
            }
        }
    }
}

//
//
//
bool CN3FXBundleGame::Load(HANDLE hFile) {
    DWORD dwRWC = 0;

    ReadFile(hFile, &m_iVersion, sizeof(int), &dwRWC, NULL);

    ReadFile(hFile, &m_fLife0, sizeof(float), &dwRWC, NULL);
    if (m_fLife0 > 10.0f) {
        m_fLife0 = 10.0f;
    }
    ReadFile(hFile, &m_fVelocity, sizeof(float), &dwRWC, NULL);

    ReadFile(hFile, &m_bDependScale, sizeof(bool), &dwRWC, NULL);

    if (m_iVersion == 0) {
        for (int i = 0; i < 8; i++) {
            int iType;

            ReadFile(hFile, &iType, sizeof(int), &dwRWC, NULL);

            if (iType == FX_PART_TYPE_NONE) {
                continue;
            }

            else if (iType == FX_PART_TYPE_PARTICLE) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartParticles;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_PARTICLE;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_BOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBillBoardGame;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOARD;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_MESH) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartMesh;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_MESH;
                m_pPart[i]->pPart->Load(hFile);
            } else if (iType == FX_PART_TYPE_BOTTOMBOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBottomBoardGame;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOTTOMBOARD;
                m_pPart[i]->pPart->Load(hFile);
            }
        }
    }
    if (m_iVersion >= 1) {
        for (int i = 0; i < MAX_FX_PART; i++) {
            int iType;

            ReadFile(hFile, &iType, sizeof(int), &dwRWC, NULL);

            if (iType == FX_PART_TYPE_NONE) {
                continue;
            }

            else if (iType == FX_PART_TYPE_PARTICLE) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartParticles;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_PARTICLE;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_BOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBillBoardGame;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOARD;
                m_pPart[i]->pPart->Load(hFile);
            }

            else if (iType == FX_PART_TYPE_MESH) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartMesh;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_MESH;
                m_pPart[i]->pPart->Load(hFile);
            } else if (iType == FX_PART_TYPE_BOTTOMBOARD) {
                m_pPart[i] = new FXPARTWITHSTARTTIME;

                float fStartTime;
                ReadFile(hFile, &(fStartTime), sizeof(float), &dwRWC, NULL);

                m_pPart[i]->fStartTime = fStartTime;

                m_pPart[i]->pPart = new CN3FXPartBottomBoardGame;
                m_pPart[i]->pPart->m_pRefBundle = this;
                m_pPart[i]->pPart->m_pRefPrevPart = NULL;
                m_pPart[i]->pPart->m_iType = FX_PART_TYPE_BOTTOMBOARD;
                m_pPart[i]->pPart->Load(hFile);
            }
        }
    }

    if (m_iVersion >= 2) {
        ReadFile(hFile, &m_bStatic, sizeof(bool), &dwRWC, NULL);
    }

    return true;
}

void CN3FXBundleGame::SetPreBundlePos(int iSourceID, int iJoint) {
    CPlayerBase * pSource = CGameProcedure::s_pProcMain->CharacterGetByID(iSourceID, true);
    if (pSource) {
        if (pSource->m_pShapeExtraRef) {
            __Vector3 vMin = pSource->m_pShapeExtraRef->Min();
            __Vector3 vMax = pSource->m_pShapeExtraRef->Max();
            m_vPrePos = vMin + ((vMax - vMin) * 0.5f);
        } else {
            const __Matrix44 * pMtx = pSource->JointMatrixGet(iJoint);
            if (!pMtx) {
                m_vPrePos = pSource->Position();
            } else {
                pSource->JointPosGet(iJoint, m_vPrePos);
            }
        }
    }
}
