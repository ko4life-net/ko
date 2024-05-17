// VtxPosDummy.h: interface for the CVtxPosDummy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "TransDummy.h"

// 점을 이동시키는 기능을 가졌다.
class CVtxPosDummy : public CTransDummy {
  public:
    CVtxPosDummy();
    virtual ~CVtxPosDummy();

    // Attributes
  public:
  protected:
    CTypedPtrArray<CPtrArray, __VertexXyzT1 *> m_SelVtxArray; // selected points

    // Operations
  public:
    virtual void Tick();
    virtual void Render();
    virtual void Release();
    virtual void SetSelObj(CN3Transform * pObj); // change selected object
    virtual void AddSelObj(CN3Transform * pObj); // add selected object
    virtual BOOL MouseMsgFilter(LPMSG pMsg);     // Process mouse messages

    void SetSelVtx(__VertexXyzT1 * pVtx); // change selected point
    void AddSelVtx(__VertexXyzT1 * pVtx); // add selected point

    void AddPos(__Vector3 vAddPos) { m_vPos += vAddPos; }
    void PosRotate(__Matrix44 vRotate, __Vector3 vCenterPos);

  protected:
    virtual void TransDiff(__Vector3 * pvDiffPos, __Quaternion * pqDiffRot,
                           __Vector3 * pvDiffScale); // Transform the selected objects by the difference.
};
