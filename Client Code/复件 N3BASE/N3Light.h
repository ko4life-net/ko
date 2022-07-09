// N3Light.h: interface for the CN3Light class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3ILight_h__INCLUDED_)
#define AFX_N3ILight_h__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Transform.h"

class CN3Light : public CN3Transform
{
public:
	typedef struct __Light : public _D3DLIGHT8
	{
	public:
		BOOL	bOn; // 라이트가 켜져 있는지..
		int		nNumber; // 0 ~ 8
		
		void Zero() { memset(this, 0, sizeof(__Light)); }
		void InitPoint(D3DXVECTOR3& dvPos, _D3DCOLORVALUE& ltColor, float fRange = 10000.0f, float fAttenuationRate = 0.8f)
		{
			this->Zero();
			Type = D3DLIGHT_POINT;
			Position = dvPos;
			//Specular = 
			Diffuse = ltColor;
			Ambient.r = ltColor.r * 0.5f;
			Ambient.g = ltColor.g * 0.5f;
			Ambient.b = ltColor.b * 0.5f;

			if(fRange < 0.0001f) fRange = 0.0001f;
			Attenuation0 = 1.0f;
			Attenuation1 = (1/fRange) * (1-fAttenuationRate);
//			Attenuation2 = 0.0008f;

			Range = fRange;
			bOn = TRUE;
		}
		void InitDirection(D3DXVECTOR3& dvPos, D3DXVECTOR3& dvDir, _D3DCOLORVALUE& ltColor)
		{
			this->Zero();
			Type = D3DLIGHT_DIRECTIONAL;
			Position = dvPos;
			Direction = dvDir;

			//Specular = 
			Diffuse = ltColor;
			Ambient.r = ltColor.r * 0.5f;
			Ambient.g = ltColor.g * 0.5f;
			Ambient.b = ltColor.b * 0.5f;
		}

		void PosSet(D3DXVECTOR3& dvPos) { Position = dvPos; }
		void PosSet(float fx, float fy, float fz) { Position.x = fx; Position.y = fy; Position.z = fz; }
	} __Light;

	__Light m_Data;

public:
	void Tick(float fFrm);
	
	bool Load(HANDLE hFile);
	bool Save(HANDLE hFile);

	CN3Light();
	virtual ~CN3Light();
	void Release();
};

#endif // !defined(AFX_N3ILight_h__INCLUDED_)
