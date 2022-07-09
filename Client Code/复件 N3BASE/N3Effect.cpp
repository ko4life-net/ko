// N3Effect.cpp: implementation of the CN3Effect class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxBase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "N3Effect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CN3EffectPart::CN3EffectPart()
{
	m_nTexCount = 0; // Texture Count
	m_ppTexs = NULL; // Textures - Manager 에서 참조를 가져와서 쓴다..

	this->Release();
}

CN3EffectPart::~CN3EffectPart()
{
	this->Release();
}

void CN3EffectPart::Release()
{
	CN3Base::Release();

	for(int i = 0; i < m_nTexCount; i++)
	{
		s_MngTex.Delete(m_ppTexs[i]);
	}
	delete [] m_ppTexs;
	m_nTexCount = 0; // Texture Count

	m_nBoardCount = 0; // Board Count
	m_fFPS = 30.0f; // Frame Per Second
	m_fBoardSize = 1.0f; // Board Size
	m_vOffset.Zero(); // Offset Position
	m_vRandomMin.Zero(); // offset position 을 기준으로 한 Random Factor
	m_vRandomMax.Zero(); // offset position 을 기준으로 한 Random Factor
	m_BoardColor = 0; // Board Color
	m_fSpeedZ = 0; // Z 축 방향 Move Speed
	m_fSpeedY = 0; // Y 축 방향 Move Speed
	m_fGravity = 0; // 중력가속도값
	m_bNeedCollisionCheck = FALSE; // 충돌 체크가 필요한가??
	memset(m_VPlanes, 0, 6 * sizeof(__VertexXyzColorT1)); // Vertex Buffer

	// Particle
	m_eParticleType = PARTICLE_NOTHING;
	m_nTexCycle = 0; // 파티클 텍스처 사이클 카운트.. 수명 처럼 쓴다..
	m_fParticleDelta = 0; // 초기 속도...
	m_fParticleAccel = 0; // 가속도...

	// Alpha Blending Factor
	m_dwSrcBlend = D3DBLEND_ONE;
	m_dwDestBlend = D3DBLEND_ONE;
}

void CN3EffectPart::BoardBuild()
{
	m_VPlanes[0].Set(-m_fBoardSize/2.0f,  m_fBoardSize/2.0f, 0, m_BoardColor, 0.0f, 0.0f);
	m_VPlanes[1].Set( m_fBoardSize/2.0f,  m_fBoardSize/2.0f, 0, m_BoardColor, 1.0f, 0.0f);
	m_VPlanes[2].Set( m_fBoardSize/2.0f, -m_fBoardSize/2.0f, 0, m_BoardColor, 1.0f, 1.0f);
	m_VPlanes[3] = m_VPlanes[0];
	m_VPlanes[4] = m_VPlanes[2];
	m_VPlanes[5].Set(-m_fBoardSize/2.0f, -m_fBoardSize/2.0f, 0, m_BoardColor, 0.0f, 1.0f);
}

void CN3EffectPart::TexAlloc(int nCount)
{
	if(nCount <= 0) return;

	if(m_nTexCount > 0)
	{
		for(int i = 0; i < m_nTexCount; i++)
		{
			s_MngTex.Delete(m_ppTexs[i]);
		}
		delete [] m_ppTexs; m_ppTexs = NULL;
		m_nTexCount = NULL;
	}
	
	m_nTexCount = nCount;
	m_ppTexs = new CN3Texture*[nCount];
	memset(m_ppTexs, 0, 4 * nCount);
}

void CN3EffectPart::TexSet(const char* szFN) // 이름을 넣어주면 할당된 텍스처 수 만큼 세자리 숫자를 붙여서 부른다. 단 할당된 텍스처가 하나면 그이름 그대로 부른다..
{
	if(m_nTexCount == 1)
	{
		s_MngTex.Delete(m_ppTexs[0]);
		m_ppTexs[0] = s_MngTex.Get(szFN);
	}
	else if(m_nTexCount > 1)
	{
		char szNameTmp[256];
		char szExt[5];
		char szName[256];
		lstrcpy(szName, szFN);
		szName[lstrlen(szFN) - 4] = NULL;

		lstrcpy(szExt, &szFN[lstrlen(szFN) - 4]);
		for(int i = 0; i < m_nTexCount; i++)
		{
			s_MngTex.Delete(m_ppTexs[i]);
			
			wsprintf(szNameTmp, "%s%.2d%s", szName, i, szExt);
			m_ppTexs[i] = s_MngTex.Get(szNameTmp);
		}
	}
}

void CN3EffectPart::TexSet(int index, const char* szName)
{
	if(index < 0 || index >= m_nTexCount) return;
	m_ppTexs[index] = s_MngTex.Get(szName);
}

CN3Effect::CN3Effect()
{
	m_nPartCount = 0;
	m_pParts = NULL;

	m_nTickStepCount = 0;
	m_pTicks = NULL;
}

CN3Effect::~CN3Effect()
{
	Release();
}

void CN3Effect::Release()
{
	CN3Base::Release();
	
	m_iFactor = 0;

	delete [] m_pParts;	m_pParts = NULL;
	m_nPartCount = 0;
	
	delete [] m_pTicks; m_pTicks = NULL;
	m_nTickStepCount = 0;
}

void CN3Effect::PartAlloc(int nCount)
{
	if(nCount <= 0) return;

	delete [] m_pParts;
	m_pParts = new CN3EffectPart[nCount];
	m_nPartCount = nCount;
}

void CN3Effect::PartInit(int nPart, int nTexCount, const char *szFN)
{
	if(nPart < 0 || nPart >= m_nPartCount) return;

	char szFN2[512];

	m_pParts[nPart].Release();
	m_pParts[nPart].TexAlloc(nTexCount);
	for(int i = 0; i < nTexCount; i++)
	{
		wsprintf(szFN2, "%s.2d.DXT");
		m_pParts[nPart].TexSet(i, szFN);
	}
}

void CN3Effect::TickStepAlloc(int nCount)
{
	if(nCount <= 0) return;

	delete [] m_pTicks;
	m_pTicks = new __EFFECT_TICK[nCount];
	m_nTickStepCount = nCount;
}
//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
bool CN3Effect::Parsing(const char* szFN)
{
	FILE* fp = NULL;
	fp = fopen(szFN, "rt"); // 파일 열기..
	if(NULL == fp)
	{
		char szErr[512];
		wsprintf(szErr, "%s - Can't open effect script file (read)", szFN);
		MessageBox(s_hWndBase, szErr, "File Handle 오류", MB_OK);
		return false;
	}

	this->Parsing(fp);

	fclose(fp);

	return true;
}

void CN3Effect::Parsing(FILE* fp)
{
	static 	char szLine[256] = "", szTitle[80] = "", szBuf[5][80] = { "", "", "", "", ""};

	// Factor Number
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	__ASSERT(strstr(szTitle, "<FactorNum>"), "Invalid FactorNumber");
	m_iFactor = atoi(szBuf[0]);

	// Effect Name
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	this->NameSet(szBuf[0]);

	// Effect Part Count
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	int nPartCount = atoi(szBuf[0]);
	this->PartAlloc(nPartCount);

	// Effect Tick Count
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	int nTickCount = atoi(szBuf[0]);
	this->TickStepAlloc(nTickCount);
	for(int i = 0; i < nTickCount; i++)
	{
		fgets(szLine, 256, fp); sscanf(szLine, "%s %s %s %s %s %s", szTitle, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4]);
		lstrcpy(m_pTicks[i].szName, szBuf[0]);
		m_pTicks[i].nPart0 = atoi(szBuf[1]);
		m_pTicks[i].nPart1 = atoi(szBuf[2]);
		m_pTicks[i].nCondition = atoi(szBuf[3]);
		lstrcpy(m_pTicks[i].szSoundFN, szBuf[4]);
	}

	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]); // 한줄 건너기..
	for(i = 0; i < nPartCount; i++)
	{
		this->ParsingPart(fp, &(m_pParts[i])); // Part Parsing
		fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]); // 한줄 건너기..
	}

	///////////////////////////////////////
	// Light Use ??
	fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
	BOOL bLightUse = atoi(szBuf[0]);

	if(bLightUse)
	{
		// Light FlyingRange
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
		sscanf(szLine, "%s %s", szBuf[0], szBuf[1]);
		m_fFlyLightRange = (float)atof(szBuf[0]);

		// Light FlyingAttenuation0
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
		sscanf(szLine, "%s %s", szBuf[0], szBuf[1]);
		m_fFlyLightAtten0 = (float)atof(szBuf[0]);

		// Light FlyingAttenuation1
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
		sscanf(szLine, "%s %s", szBuf[0], szBuf[1]);
		m_fFlyLightAtten1 = (float)atof(szBuf[0]);
		// Light Flying Property
		///////////////////////////////////////

		///////////////////////////////////////
		// Light Explode Property
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);

		// Light ExplodeRange
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
		sscanf(szLine, "%s %s", szBuf[0], szBuf[1]);
		m_fExplodeLightRange = (float)atof(szBuf[0]);

		// Light ExplodeAttenuation0
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
		sscanf(szLine, "%s %s", szBuf[0], szBuf[1]);
		m_fExplodeLightAtten0 = (float)atof(szBuf[0]);

		// Light ExplodeAttenuation1
		fgets(szLine, 256, fp); sscanf(szLine, "%s", szBuf[0]);
		sscanf(szLine, "%s %s", szBuf[0], szBuf[1]);
		m_fExplodeLightAtten1 = (float)atof(szBuf[0]);
		// Light Explode Property
		///////////////////////////////////////
	}
}

void CN3Effect::ParsingPart(FILE* fp, CN3EffectPart* pPart)
{
	static 	char szLine[256] = "", szTitle[80] = "", szBuf[5][80] = { "", "", "", "", ""};
/*
<PartName> 00::Spread
00:BoardOffset 0.0 0.0 0.0
01:BoardCount 30
02:BoardRandomFactorMin -0.2 0 0
03:BoardRandomFactorMax 0.2 0.2 0
04:BoardSize 0.05
05:BoardColor 255 255 255 255
06:MoveSpeed 0 0 // Z,Y축..
07:NeedCheckCollision 0 // 1-지형과 충돌체크 필요 2-건물과 충돌 체크 필요 -> Flag 를 or 연산해서 쓴다.
08:Gravity 0.05
09:TextureFade // Texture Fade In / Out 단위는 milisecond, 양이면 사라지고, 음이면 나타난다.
10:TextureCount 4
11:TextureAnimationSpeed 4 // Texture Animation 속도(Frame per second) 혹은 투명해지는 속도(milisecond초)
12:TextureName FX\Blood.DXT // 확장자 앞에 순서가 두자리로 들어간다. 없으면 순서 두자리는 무시
13:ParticleType EXPLODE // NOTHING NORMAL EXPLODE DROP GATHER
14:ParticleCycle 1
15:ParticleDelta 1
16:ParticleAccelation 0.0
17:SrcBlend SRCALPHA
18:DestBlend INVSRCALPHA
*/
	// Name 
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->NameSet(szBuf[0]);

	// 0 : Board Offset
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s %s %s", szTitle, szBuf[0], szBuf[1], szBuf[2]);
	pPart->m_vOffset.Set((float)atof(szBuf[0]), (float)atof(szBuf[1]), (float)atof(szBuf[2]));

	// 1 : Board Count
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_nBoardCount = atoi(szBuf[0]);

	// 2 : Board Random Factor Min
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s %s %s %s", szTitle, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);
	pPart->m_vRandomMin.Set((float)atof(szBuf[0]), (float)atof(szBuf[1]), (float)atof(szBuf[2]));

	// 3 : Board Random Factor Max
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s %s %s %s", szTitle, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);
	pPart->m_vRandomMax.Set((float)atof(szBuf[0]), (float)atof(szBuf[1]), (float)atof(szBuf[2]));

	// 4 : Board Size
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_fBoardSize = (float)atof(szBuf[0]);

	// 5 : BoardColor
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s %s %s %s", szTitle, szBuf[0], szBuf[1], szBuf[2], szBuf[3]);
	pPart->m_BoardColor = ((atoi(szBuf[0])) << 24) | ((atoi(szBuf[1])) << 16) | ((atoi(szBuf[2])) << 8) | (atoi(szBuf[3]));

	// 중요..
	pPart->BoardBuild(); // 정해진 크기와 색 대로 Board 만들기..

	// 6 : Move Speed
	fgets(szLine, 256, fp); sscanf(szLine, "%s", szTitle); sscanf(szLine, "%s %s %s", szTitle, szBuf[0], szBuf[1]);
	pPart->m_fSpeedZ = (float)atof(szBuf[0]); pPart->m_fSpeedY = (float)atof(szBuf[1]);

	// 7 : 충돌 체크가 필요한가??
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_bNeedCollisionCheck = atoi(szBuf[0]);

	// 8 : Gravity
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_fGravity = (float)atof(szBuf[0]);

	// 9 : Texture Fade
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
//	pPart->TexAlloc(atoi(szBuf[0]));

	// 9 : Texture Count
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->TexAlloc(atoi(szBuf[0]));

	// 10 Texture Animation Speed Or Transparency Speed
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_fFPS = (float)atof(szBuf[0]);

	// 11 Texture Name
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->TexSet(szBuf[0]);

	// 12 Is Particle ??
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	if(lstrcmpi(szBuf[0], "NOTHING") == 0)
	{
		pPart->m_eParticleType = PARTICLE_NOTHING;
	}
	else if(lstrcmpi(szBuf[0], "NORMAL") == 0)
	{
		pPart->m_eParticleType = PARTICLE_NORMAL;
	}
	else if(lstrcmpi(szBuf[0], "DROP") == 0)
	{
		pPart->m_eParticleType = PARTICLE_DROP;
	}
	else if(lstrcmpi(szBuf[0], "EXPLODE") == 0)
	{
		pPart->m_eParticleType = PARTICLE_EXPLODE;
	}
	else if(lstrcmpi(szBuf[0], "GATHER") == 0)
	{
		pPart->m_eParticleType = PARTICLE_GATHER;
	}
	
	// 13 Particle Texture Cycle
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_nTexCycle = atoi(szBuf[0]);

	// 14 Particle Move Factor
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_fParticleDelta = (float)atof(szBuf[0]);

	// 15 Particle 가속도
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	pPart->m_fParticleAccel = (float)atof(szBuf[0]);

	// Blend Factor
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	if(lstrcmpi(szBuf[0], "ONE") == 0) pPart->m_dwSrcBlend = D3DBLEND_ONE;
	else if(lstrcmpi(szBuf[0], "SRCCOLOR") == 0) pPart->m_dwSrcBlend = D3DBLEND_SRCCOLOR;
	else if(lstrcmpi(szBuf[0], "INVSRCCOLOR") == 0) pPart->m_dwSrcBlend = D3DBLEND_INVSRCCOLOR;
	else if(lstrcmpi(szBuf[0], "SRCALPHA") == 0) pPart->m_dwSrcBlend = D3DBLEND_SRCALPHA;
	else if(lstrcmpi(szBuf[0], "INVSRCALPHA") == 0) pPart->m_dwSrcBlend = D3DBLEND_INVSRCALPHA;

	// Blend Factor
	fgets(szLine, 256, fp); sscanf(szLine, "%s %s", szTitle, szBuf[0]);
	if(lstrcmpi(szBuf[0], "ONE") == 0) pPart->m_dwDestBlend = D3DBLEND_ONE;
	else if(lstrcmpi(szBuf[0], "SRCCOLOR") == 0) pPart->m_dwDestBlend = D3DBLEND_SRCCOLOR;
	else if(lstrcmpi(szBuf[0], "INVSRCCOLOR") == 0) pPart->m_dwDestBlend = D3DBLEND_INVSRCCOLOR;
	else if(lstrcmpi(szBuf[0], "SRCALPHA") == 0) pPart->m_dwDestBlend = D3DBLEND_SRCALPHA;
	else if(lstrcmpi(szBuf[0], "INVSRCALPHA") == 0) pPart->m_dwDestBlend = D3DBLEND_INVSRCALPHA;
}
