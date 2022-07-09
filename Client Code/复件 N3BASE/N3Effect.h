// N3Effect.h: interface for the CN3Effect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3EFFECT_H__D6484AEE_8A43_40C2_B0A8_DC5ABAF80762__INCLUDED_)
#define AFX_N3EFFECT_H__D6484AEE_8A43_40C2_B0A8_DC5ABAF80762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"
#include "N3Texture.h"

// 여기 서부터는 Effect 관련 변수이다..
// Enumeration..
enum PARTICLE_TYPE {PARTICLE_NOTHING = -1,
					PARTICLE_NORMAL = 0, PARTICLE_DROP, PARTICLE_EXPLODE, PARTICLE_GATHER };

#include <stdio.h>
#include <stdlib.h>

class CN3EffectPart : public CN3Base
{
	friend class CPartInstance;

protected:
	int		m_nTexCount; // Texture Count
	CN3Texture** m_ppTexs; // Textures - Manager 에서 참조를 가져와서 쓴다..

public:
	int			m_nBoardCount; // Board Count
	float		m_fFPS; // Frame Per Second
	float		m_fBoardSize; // Board Size
	__Vector3	m_vOffset; // Offset Position
	__Vector3	m_vRandomMin; // offset position 을 기준으로 한 Random Factor
	__Vector3	m_vRandomMax; // offset position 을 기준으로 한 Random Factor
	D3DCOLOR	m_BoardColor; // Board Color
	float		m_fSpeedZ; // Z 축 방향 Move Speed
	float		m_fSpeedY; // Y 축 방향 Move Speed
	float		m_fGravity; // 중력가속도값
	BOOL		m_bNeedCollisionCheck; // 충돌 체크가 필요한가??
	__VertexXyzColorT1 m_VPlanes[6]; // Vertex Buffer

	// Particle
	PARTICLE_TYPE	m_eParticleType;
	int				m_nTexCycle; // 파티클 텍스처 사이클 카운트.. 수명 처럼 쓴다..
	float			m_fParticleDelta; // 초기 속도...
	float			m_fParticleAccel; // 가속도...

	// Alpha Blend Factor
	DWORD m_dwSrcBlend, m_dwDestBlend;

public:
	void		BoardBuild();

	CN3Texture* Tex(int index) { if(index < 0 || index >= m_nTexCount) return NULL; return m_ppTexs[index]; }
	void		TexAlloc(int nCount);
	int			TexCount() { return m_nTexCount; }
	void		TexSet(const char* szFN); // 이름을 넣어주면 할당된 텍스처 수 만큼 세자리 숫자를 붙여서 부른다.
	void		TexSet(int index, const char* szName);
	
	void		Release();
	CN3EffectPart();
	~CN3EffectPart();
};

class CN3Effect : public CN3Base
{
public:
	struct __EFFECT_TICK
	{
		char szName[32];
		int nPart0;
		int nPart1;
		int nCondition;
		char szSoundFN[256];

		__EFFECT_TICK()
		{
			memset(this, 0, sizeof(__EFFECT_TICK));
		}
	};

protected:
	int				m_nPartCount; // Unit 갯수..
	CN3EffectPart*	m_pParts; // Effect Parts Pointer
	int				m_nTickStepCount; // Tick 단계 갯수
	__EFFECT_TICK*	m_pTicks; // Effect Tick Struct - 이 구조체에 담긴 내용대로 매 단계 마다 Part 가 바뀌고 Sound 가 재생된다.

public:
	int			m_iFactor; // Factor Number

	float m_fFlyLightRange;
	float m_fFlyLightAtten0;
	float m_fFlyLightAtten1;

	float m_fExplodeLightRange;
	float m_fExplodeLightAtten0;
	float m_fExplodeLightAtten1;

public:
	bool Parsing(const char* szFN);
	void Parsing(FILE* fp);
	void ParsingPart(FILE* fp, CN3EffectPart* pPart);

	void PartAlloc(int nCount);
	void PartInit(int nPart, int nTexCount, const char* szFN);
	CN3EffectPart* Part(int nPart) { if(nPart < 0 || nPart >= m_nPartCount) return NULL; return &(m_pParts[nPart]); }

	__EFFECT_TICK* TickStepStruct(int nTick) { if(nTick < 0 || nTick >= m_nTickStepCount) return NULL; return &(m_pTicks[nTick]); }
	void TickStepAlloc(int nCount);
	int TickStepCount() { return m_nTickStepCount; }

	void	Release();
	CN3Effect();
	virtual ~CN3Effect();

};

#endif // !defined(AFX_N3EFFECT_H__D6484AEE_8A43_40C2_B0A8_DC5ABAF80762__INCLUDED_)
