// N3AlphaPrimitiveManager.h: interface for the CN3AlphaPrimitiveManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "My_3DStruct.h"

//const DWORD RF_NOTHING = 0;
//const DWORD RF_ALPHABLENDING = 1;
//const DWORD RF_NOTUSEFOG = 2;
//const DWORD RF_DOUBLESIDED = 4;

struct __AlphaPrimitive {
    float              fCameraDistance; // Distance from camera...
    DWORD              dwBlendSrc;
    DWORD              dwBlendDest;
    int                nRenderFlags;    // Rendering flag.. RF_... See..
    LPDIRECT3DTEXTURE9 lpTex;           // Texture pointer
    DWORD              dwFVF;           // flexible vertex format
    D3DPRIMITIVETYPE   ePrimitiveType;  // Primitive Type
    int                nPrimitiveCount; // PrimitiveCount
    DWORD              dwPrimitiveSize; // Primitive Size .. stream 0 stride
    BOOL               bUseVB;          // Whether to use vertex buffer or index buffer
    const void *       pwIndices;       // If it is Index-based... just enter something other than NULL.
    int                nVertexCount;
    const void *       pVertices; //Triangle.. is a vector type, but it must be used to accept various point formats through forced type conversion..
    __Matrix44         MtxWorld;  // Matrix
};

const int MAX_ALPHAPRIMITIVE_BUFFER = 1024;

class CN3AlphaPrimitiveManager {
  protected:
    int              m_nToDrawCount;                       // Number of buffers to draw
    __AlphaPrimitive m_Buffers[MAX_ALPHAPRIMITIVE_BUFFER]; // Primitive buffer..

  public:
    int                ToDrawCount() { return m_nToDrawCount; }
    __AlphaPrimitive * Add();

    void Render();

    static int SortByCameraDistance(const void * pArg1, const void * pArg2); // Sort function..

    CN3AlphaPrimitiveManager();
    virtual ~CN3AlphaPrimitiveManager();
};
