// N3EffectWave2.h: interface for the CN3EffectWave2 class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_PONDMESH_LINE   200
#define MAX_PONDMESH_VERTEX 200 * 4
#define MAX_POND_TEX        32

#include "N3Base/N3BaseFileAccess.h"

class CN3EffectWave2 : public CN3BaseFileAccess {
  public:
    CN3EffectWave2();
    virtual ~CN3EffectWave2();

    struct __VertexPond {
      public:
        float    x, y, z;
        float    nx, ny, nz;
        D3DCOLOR color;
        float    u, v, u2, v2;
        void     Set(float sx, float sy, float sz, float snx, float sny, float snz, D3DCOLOR scolor, float su, float sv,
                     float su2, float sv2) {
            x = sx, y = sy, z = sz;
            nx = snx, ny = sny, nz = snz;
            color = scolor;
            u = su, v = sv;
            u2 = su2, v2 = sv2;
        }
    };

    class CPongMesh {
      public:
        BOOL           m_bTick2Rand;      //    시야에 들어와 tick과rend를 실행결정
        __VertexPond * m_pVertices;       //    Vertices
        float *        m_pfVelocityArray; //    계산 저장
        float          m_pfMaxHeight;     //    물결이 어느정도 이상 올라가지 못하게 함
        WORD *         m_wpIndex;         //    그림을 그릴 순서
        int            m_iIC;             // Index Buffer Count.
        int            m_iVC;             // Vertex Count.

        int   m_iWidthVtx, m_iHeightVtx;      // 계산에 필요
        float m_fWaterScaleX, m_fWaterScaleZ; // 계산에 필요

        __Vector3 m_vCenterPo; //    연못의 중간지점
        float     m_fRadius;   //    연못의 지름

        CPongMesh() {
            m_bTick2Rand = FALSE;
            m_pVertices = NULL, m_wpIndex = NULL;
            m_pfVelocityArray = NULL;
        };
        virtual ~CPongMesh() {
            if (m_pVertices) {
                delete[] m_pVertices;
            }
            m_pVertices = NULL;
            if (m_wpIndex) {
                delete[] m_wpIndex;
            }
            m_wpIndex = NULL;
            if (m_pfVelocityArray) {
                delete[] m_pfVelocityArray;
            }
            m_pfVelocityArray = NULL;
        };
    };

  public:
    int         m_iPondMeshNum; //    전체 연못의 갯수
    CPongMesh * m_pCPondMesh;   //    연못의 정보

    CN3Texture * m_pTexPond[MAX_POND_TEX];
    CN3Texture * m_pTexWave;
    float        m_fTexIndex;

    int     m_iMaxVtxNum; //    가장 많은 vertices수
    float * m_pfMaxVtx;   //    물결높이 계산을 위한 임시

  public:
    void Release();
    bool Load(HANDLE hFile);
    void Render();
    void Tick();
    void Init(const fs::path & fsTexFile);

  private:
    void CheckHeight(float & ChkHeight) {
        if (ChkHeight < -0.01f) {
            ChkHeight += 0.01f;
        } else if (ChkHeight > 0.01f) {
            ChkHeight += -0.01f;
        }
    };

    void UpdateWaterPositions();
};
