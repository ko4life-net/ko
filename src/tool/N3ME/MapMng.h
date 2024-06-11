// MapMng.h: interface for the CMapMng class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

// Cursor Mode
enum {
    CM_SELECT = 1,
    CM_OBJECT_BRUSH = 2,
    CM_POS = 3,
    CM_ROTATE = 4,
    CM_SCALE = 5,
    CM_EDIT_TERRAIN = 6,
    CM_SMOOTH_TERRAIN = 7,
    CM_SET_DTEX = 8,
    CM_EDIT_RIVER = 9,
    CM_EDIT_LIGHTMAP = 10,
    CM_MAKE_NPCPATH = 11,
    CM_MAKE_WALL = 12,
    CM_EDIT_EVENT = 13,
    CM_EDIT_POND = 14,
    CM_FLAT_TERRAIN = 15,
    CM_EDIT_REGENUSER = 16,
    CM_EDIT_SOUND = 17,
    CM_EDIT_SEED = 18,
    CM_EDIT_LIGHT = 19
};

const int NUM_UNIT_LIGHT = 2;

#include <afxtempl.h>
#include "N3Base/N3Base.h"
#include "PosDummy.h"
#include "RotDummy.h"
#include "ScaleDummy.h"
#include "RiverMng.h"
#include "PondMng.h"
#include "LyTerrain.h"
#include "RegenUser.h"
#include "SowSeedMng.h"

class CMainFrame;
class CN3Scene;
class CN3Camera;
class CN3Light;
//class CDlgSceneGraph;
class CDlgShapeList;
class CBrushDlg;
class CN3Transform;
class CN3TransformCollision;
class CDlgBase;
class CN3Base;
class CNPCPathMgr;
class CWallMgr;
class CN3ShapeMgr;
class CEventMgr;
class CRegenUser;
class CWarpMgr;
class CSoundMgr;
class CLightObjMgr;

class CMapMng : public CN3Base {
  public:
    CMapMng(CMainFrame * pMainFrm);
    virtual ~CMapMng();

    // Attributes
  public:
    CN3Transform * GetSelOutputObj(int iIndex) const {
        if (iIndex < 0 || iIndex >= m_SelOutputObjArray.GetSize()) {
            return NULL;
        }
        return m_SelOutputObjArray.GetAt(iIndex);
    }
    int          GetSelOutputObjCount() const { return m_SelOutputObjArray.GetSize(); };
    CLyTerrain * GetTerrain() const { return m_pTerrain; }
    CWnd *       GetDlgSourceList() const { return (CWnd *)m_pDlgSourceList; }
    CWnd *       GetDlgOutputList() const { return (CWnd *)m_pDlgOutputList; }
    CWnd *       GetDlgBase() const { return (CWnd *)m_pDlgBase; }
    CN3Scene *   GetSceneOutput() { return m_pSceneOutput; }
    D3DSHADEMODE m_ShadeMode;
    D3DFILLMODE  m_FillMode;
    bool         m_bViewWireFrame;     // View the selected object as a wireframe.
    bool         m_bRenderAxisAndGrid; // Decide whether to draw the axis and grid or not.

    int  m_iZoneID;
    bool m_bHideObj;
    bool m_bLoadingComplete; // Only render if this is true.

    CMainFrame *    m_pMainFrm; // Main window
    CSowSeedMng     m_SowSeedMng;
    CDlgShapeList * m_pDlgSourceList; // Dialog to show the object list
    CN3Transform *  GetSelSourceObj() { return m_pSelSourceObj; }

  protected:
    enum ENUM_EDIT_STATE {
        eEDIT_NOT = 0,
        eEDIT_COPY,
        eEDIT_PASTE,
        eEDIT_NUM
    };

    CLightObjMgr * m_pLightObjMgr;
    CSoundMgr *    m_pSoundMgr;
    CWarpMgr *     m_pWarpMgr;
    CRegenUser *   m_pRegenUser;
    CEventMgr *    m_pEventMgr;    //Zone event management...
    CWallMgr *     m_pWall;        //Management of walls that do not allow access to the outside of the zone..
    CNPCPathMgr *  m_pNPCPath;     // NPC road making class..
    CLyTerrain *   m_pTerrain;     // terrain
    CN3Scene *     m_pSceneSource; // Scene containing things to show in the source object list
    CN3Scene *     m_pSceneOutput; // Scene containing objects placed on the map

    //CDlgSceneGraph*        m_pDlgSourceList;            // Dialog to show the object list
    //CDlgSceneGraph*        m_pDlgOutputList;            // Dialog to show objects placed on the map
    CDlgShapeList * m_pDlgOutputList; // Dialog to show objects placed on the map
    CDlgBase *      m_pDlgBase;       // Object properties edit dialog box...
    CBrushDlg *     m_pBrushDlg;      // Dialog box for specifying terrain editing brush options

    CN3Transform * m_pSelSourceObj; // selected from the source object list
    //    CTypedPtrArray<CPtrArray, CN3TransformCollision*> m_SelOutputObjArray;    // Selected objects among the objects placed on the map
    CTypedPtrArray<CPtrArray, CN3Transform *>
        m_SelOutputObjArray; // Selected objects among the objects placed on the map

    ENUM_EDIT_STATE m_eSelObjBackState; // Display status of temporary copy (ctrl+c)
    CTypedPtrArray<CPtrArray, CN3Transform *>
        m_SelOutputObjBack; // Contains selected objects among the objects placed on the map

    int  m_CursorMode;    // Classification of mouse function modes
    BOOL m_bRenderSelObj; // Determines whether to draw or not draw the selected object from the source object list

    CTransDummy * m_pDummy;     // Class responsible for moving, rotating, zooming in/out the object
    CPosDummy     m_PosDummy;   // movement
    CRotDummy     m_RotDummy;   // rotation
    CScaleDummy   m_ScaleDummy; // Zoom in zoom out
    CRiverMng     m_RiverMng;   // River information management
    CPondMng      m_PondMng;    // Pond information management

    RECT m_rcSelDrag; // drag area

    struct __Sort // Structure to sort when picking objects
    {
        CN3TransformCollision * pObj;
        float                   fCamDist;
        __Vector3               vMin, vMax;
    };

    // Operations
  public:
    void SetLight(float fLgt);
    void ActWarpMgr();
    bool IsWarpMgrActive();
    void DeleteSelectedSourceObjects();
    void DeleteOverlappedObjects();
    void DeleteUnusedFiles();
    void SetZoneID(int id) {
        m_iZoneID = id;
        if (m_pTerrain) {
            m_pTerrain->m_iZoneID = id;
        }
    } //Terrain zone ID setting.
    void ImportShape(const char * szFullPath);
    void MakeTerrainMovableAttr(
        CN3ShapeMgr *
            pShapeMgr); //Organize the tiles that can be accessed and those that cannot be accessed on the terrain.
    void ImportPostDataFromScene(const char * szFileName); // Load the object placement in the scene.
    void UpDateFP();
    void Tick();
    void Render();
    void Release();
    void SavePartition(float x, float z, float width);
    void SaveToFile(LPCTSTR lpszPathName);           // Save Map file
    void LoadFromFile(LPCTSTR lpszPathName);         // Load Map file
    BOOL MouseMsgFilter(LPMSG pMsg);                 // mouse functions
    void ImportTerrain(const char * szMeshFN);       // Read terrain data from VMesh file
    void ImportTerrainHeight(const char * szMeshFN); // Read only the height value of the terrain from the VMesh file..
    void MakeGameFiles(LPCTSTR lpszPathName, float fSize = 128.0f); // Convert to game data
    void MakeServerDataFiles(LPCTSTR lpszPathName);
    void SelectObject(CN3Base * pObj, BOOL IsSourceObj, BOOL bAdd = FALSE); // Select an object.
    void RenderObjectToWindow(CN3TransformCollision * pObj, HWND hWnd);     // Draws an object in a specific window.
    void SetCursorMode(int iMode);                                          // Change the way the mouse cursor is used.
    int  GetCursorMode() const { return m_CursorMode; } // Get information on how to use the mouse cursor.
    void Invalidate();                                  // View screen update
    void FocusSelObj();                                 // Move the porter to the selected object.
    void FocusAll();                                    // Make the entire map visible.
    void FocusAt(__Vector3 v);                          // Move focus to the specified point.
    void DropSelObjToTerrain();            // Attach the selected object to the terrain. (Adjust Y value only)
    void DeleteSelObjectFromOutputScene(); // Delete the selected object from the OutputScene.
    void RenderDragRect(RECT * rc);        // Draw the drag area.
    void UpdateAll();                      // Update source and output dialog.

    void           LoadObjectPostData(LPCTSTR lpszFileName); // Read shape placement information from text file.
    void           SaveObjectPostData(LPCTSTR lpszFileName); // Save shape placement information as a text file.
    void           SaveObjectPostDataPartition(LPCTSTR lpszFileName, float psx, float psz, float width);
    void           RenderGrid(float fGridSize, float fMaxDistance); // Draws a grid at regular intervals on the map
    void           OnSelChanged();                                  // Things to do when the selected object changes
    void           SelectObjectByDragRect(RECT * pRect, BOOL bAdd); // Select object by dragging
    void           LoadSourceObjects();                             // Read Source Object from Folder.
    CN3Transform * AddChr(CN3Scene * pDestScene, const std::string & szFN,
                          BOOL bGenerateChainNumber); // Copy and add the character object to a specific scene
    CN3Transform * AddShape(CN3Scene * pDestScene, const std::string & szFN,
                            BOOL bGenerateChainNumber); // Copy and add the Shape object to a specific scene
    CN3Transform * AddObjectToOutputScene(
        CN3Transform *
            pObj); // If you insert the object selected from the source list, it is copied and inserted into the OutputScene.
    CN3Camera * CameraGet();

    CN3Base *  Pick(POINT point, int * pnPart); // object picking function
    static int SortByCameraDistance(const void * pArg1,
                                    const void * pArg2); // Used for sorting function qsort according to camera distance

    //    BOOL    CameraMove(LPMSG pMsg);        // Process messages about camera movement
    BOOL GetObjectMinMax(CN3Transform * pObj, __Vector3 & vMin,
                         __Vector3 & vMax); // Find the maximum and minimum points of the object

    CStatusBar * GetStatusBar();

    void SetEditState(ENUM_EDIT_STATE eEditStat); // Draw and edit the selected object
};
