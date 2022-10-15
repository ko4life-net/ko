// TransDummy.h: interface for the CTransDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "N3Base/N3Transform.h"
#include <afxtempl.h>
#include <vector>

enum {
    DUMMY_CENTER = 0,
    DUMMY_X = 1,
    DUMMY_Y = 2,
    DUMMY_Z = 3,
    NUM_DUMMY = 4,
    DUMMY_NO = 5
};
const int NUM_CUBEVERTEX = 36;

struct __DUMMYCUBE {
    int                    iType;                    // Center , X, Y, Z 종류 구분.
    __VertexXyzNormalColor Vertices[NUM_CUBEVERTEX]; // 큐브의 구성 점들
    __Vector3              vCenterPos;               // 큐브의 가운데 점
    float                  fDistance;                // 카메라와의 거리
};

class CPortalVolume;
enum e_SelectType {
    TYPE_VOLUME_TOTAL = 0,
    TYPE_VOLUME_ONLY,
    TYPE_SHAPE_ONLY
};
enum e_DummyType {
    DUMMY_TRANS = 0,
    DUMMY_POS,
    DUMMY_ROT,
    DUMMY_SCALE,
    DUMMY_SWAP
};

typedef struct tagSelectElement {
    e_SelectType   eST;
    CN3Transform * pSelectPointer;

    tagSelectElement() { pSelectPointer = NULL; }
} SelectElement;

class CTransDummy : public CN3Transform {
  public:
    CTransDummy();
    virtual ~CTransDummy();

    // Attributes
  public:
  protected:
    e_DummyType      m_ceDType;
    __DUMMYCUBE      m_DummyCubes[NUM_DUMMY]; // 4개의 큐브(center, x, y, z);
    __VertexXyzColor m_LineVertices[6];       // 4개의 큐브를 이어주는 선을 구성하는 점
  public:
    e_DummyType                          m_cePrevType;
    CArray<SelectElement, SelectElement> m_SelObjArray; // MapMng에서 선택된 객체들
  protected:
    __DUMMYCUBE * m_pSortedCubes[NUM_DUMMY]; // Cube 거리에 따라 정렬된 포인터
    __DUMMYCUBE * m_pSelectedCube;           // 선택된 큐브
    __Vector3     m_vPrevPos;                // 이번 마우스 드래그 이전에 위치 회전 확대/축소값
    __Quaternion  m_qPrevRot;                // 이번 마우스 드래그 이전에 위치 회전 확대/축소값

    typedef std::vector<__Vector3>             Tv;
    typedef typename std::vector<Tv>::iterator tvit;
    std::vector<Tv>
        m_vPrevScaleArray; // 이번 마우스 드래그 이전에 확대/축소값들 (선택된 객체들 모두의 값을 가지고 있어야 하기 때문에 배열)

    //    __Vector3*            m_vPrevScaleArray;                                        // 이번 마우스 드래그 이전에 확대/축소값들 (선택된 객체들 모두의 값을 가지고 있어야 하기 때문에 배열)

    // Operations
  public:
    e_DummyType             GetDummyState() { return m_ceDType; }
    virtual bool            IsExistTotalVolByPointer(CPortalVolume * pVol);
    virtual CPortalVolume * GetFirstElementTotalVol();

    virtual void Tick();
    virtual void Render();
    virtual void Release();
    virtual void SetSelObj(SelectElement Obj, bool bOne = true); // 선택된 객체 바꾸기
    void         ClearObjs() {
        m_SelObjArray.RemoveAll();
        tvit ti = m_vPrevScaleArray.begin();
        while (ti != m_vPrevScaleArray.end()) {
            Tv tv = *ti++;
            tv.clear();
        }
        m_vPrevScaleArray.clear();
    }
    virtual void AddSelObj(SelectElement Obj); // 선택된 객체 추가
    virtual BOOL MouseMsgFilter(LPMSG pMsg);   // 마우스 메세지 처리
    void         GetPickRay(POINT point, __Vector3 & vDir,
                            __Vector3 & vOrig); // 마우스 포인터가 가리키는 쪽으로 뻗어나가는 선 얻는 함수
  protected:
    __DUMMYCUBE * Pick(int x, int y);                                                                   // 큐브 picking
    void InitDummyCube(int iType, __DUMMYCUBE * pDummyCube, const __Vector3 & vOffset, D3DCOLOR color); // 큐브 초기화
    static int   SortCube(const void * pArg1, const void * pArg2); // 큐브 카메라 거리에 대한 정렬함수
    virtual void TransDiff(__Vector3 * pvDiffPos, __Quaternion * pvDiffRot,
                           __Vector3 * pvDiffScale); // 차이만큼 선택된 오므젝트들을 변형시킨다.
};
