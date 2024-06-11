// RiverMesh.h: interface for the CRiverMesh class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//const int MAXNUM_RIVERMESH_VERTEX = 2048;
#define MAX_RIVERMESH_LINE   200
#define MAX_RIVERMESH_VERTEX 200 * 4
#define MAX_RIVERMESH_INDEX  199 * 18
#define INDEXBUFFER_PERLINE  18

#include "N3Base/N3BaseFileAccess.h"

class CRiverMesh : public CN3BaseFileAccess {
  public:
    CRiverMesh();
    virtual ~CRiverMesh();

    // Attributes
  public:
    int          GetRiverID() const { return m_iRiverID; }
    void         SetRiverID(int iRiverID) { m_iRiverID = iRiverID; }
    float        GetSpeed1() const { return m_fSpeed1; }
    float        GetSpeed2() const { return m_fSpeed2; }
    void         SetSpeed1(float fSpeed) { m_fSpeed1 = fSpeed; }
    void         SetSpeed2(float fSpeed) { m_fSpeed2 = fSpeed; }
    float        GetMeterPerU() const { return m_fMeterPerU; }
    void         SetMeterPerU(float fMeterPerU) { m_fMeterPerU = fMeterPerU; }
    float        GetMeterPerV() const { return m_fMeterPerV; }
    void         SetMeterPerV(float fMeterPerV) { m_fMeterPerV = fMeterPerV; }
    float        GetMeterPerU2() const { return m_fMeterPerU2; }
    void         SetMeterPerU2(float fMeterPerU) { m_fMeterPerU2 = fMeterPerU; }
    float        GetMeterPerV2() const { return m_fMeterPerV2; }
    void         SetMeterPerV2(float fMeterPerV) { m_fMeterPerV2 = fMeterPerV; }
    CN3Texture * TexGet() const { return m_pTexture; }
    int          VertexCount() const { return m_iVC; }
    int          IndexCount() const { return m_iIC; }
    int          GetAnimTexCount() const { return m_iAnimTextureCount; }
    CN3Texture * AnimTexGet(int iIndex) const {
        if (iIndex < 0 || iIndex >= m_iAnimTextureCount) {
            return NULL;
        }
        return m_pAnimTextures[iIndex];
    }
    float GetAnimTexFPS() const { return m_fAnimTexFPS; }
    void  SetAnimTexFPS(float fFPS) { m_fAnimTexFPS = fFPS; }
    DWORD GetAlphaFactor() const { return m_dwAlphaFactor; }
    void  SetAlphaFactor(DWORD dwFactor) { m_dwAlphaFactor = dwFactor; }

  protected:
    int           m_iRiverID;
    __VertexXyzT2 m_pVertices[MAX_RIVERMESH_VERTEX];
    WORD          m_wIndex[MAX_RIVERMESH_INDEX];
    int           m_iIC; // Index Buffer Count.
    int           m_iVC; // Vertex Count.
    CN3Texture *  m_pTexture;
    int           m_iAnimTextureCount; // Number of animated textures
    CN3Texture ** m_pAnimTextures;     // Animated texture pointers...
    float         m_fSpeed1;           // Flow speed: Change in v coordinate per second
    float         m_fSpeed2;           // Flow speed: Change in v2 coordinates per second
    float         m_fMeterPerU;        // The length of the river corresponding to U-coordinate 1.0
    float         m_fMeterPerV;        // The length of the river corresponding to V coordinate 1.0
    float         m_fMeterPerU2;       // The length of the river corresponding to U2 coordinate 1.0
    float         m_fMeterPerV2;       // The length of the river corresponding to V2 coordinate 1.0
    float         m_fAnimTexFPS;       // Number of frames per second of AnimTexture;
    DWORD         m_dwAlphaFactor;     // Alpha value to make the river transparent

    // Operations
  public:
    virtual void Release();
    virtual bool Load(HANDLE hFile);
    virtual bool Save(HANDLE hFile);
    void         Render();
    void         RenderVertexPoint(); // Redraw only the dots so they are visible

    int             AddVertex(__Vector3 & vPos1, __Vector3 & vPos2, __Vector3 & vPos3, __Vector3 & vPos4);
    int             AddVertex();
    int             DeleteVertex(int iIndex); // Erase in pairs and move the ones behind by two to the front
    __VertexXyzT2 * GetVertex(int iIndex) {
        if (iIndex < 0 || iIndex >= m_iVC) {
            return NULL;
        }
        return m_pVertices + iIndex;
    }
    __Vector3 GetCenter();

    BOOL SetTextureName(LPCTSTR pszFName);
    BOOL SetAnimTextureName(LPCTSTR pszFName, LPCTSTR pszExt, int iCount);
    void ReCalcUV();

  protected:
    void ReleaseAnimTextures();
};
