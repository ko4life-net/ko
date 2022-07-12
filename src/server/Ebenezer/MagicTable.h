// MagicTable.h: interface for the CMagicTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICTABLE_H__3BEFBF64_0058_48B1_AB56_D98E86016DFE__INCLUDED_)
#define AFX_MAGICTABLE_H__3BEFBF64_0058_48B1_AB56_D98E86016DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMagicTable  
{
public:
	CMagicTable();
	virtual ~CMagicTable();

public:
	short	m_sNum;				// 매직 번호
	char	m_strName[30];		// 매직 이름
	BYTE	m_bMoral;			// 성향
	BYTE	m_bClass;			// 사용 클래스
	BYTE	m_bLevel;			// 레벨	제한
	BYTE	m_bEffect;			// 효과(0:fire, 1:ice, 2:lightning, 3:magic, 4:disease, 5:poison, 6:light, 7:heal, 8:dagger, 9:sword, 10:mace, 11:spear, 12:polearm)
	short	m_sMp;				// 소모량
	short	m_sHp;				// 소모량
	short	m_sSt;				// 소모량
	int		m_iNItem;			// 필요 아이템(사용시)
	short	m_sDistant;			// 가시거리
	short	m_sContime;			// 지속시간(초)
	short	m_sRepeat;			// 재사용 가능 시간(초)
	BYTE	m_bExpect;			// 성공률
	BYTE	m_bTarget;			// 타겟(0:타겟, 1:자신의 반경, 2:타겟과 반경, 3:지점과 반경, 4:오브젝트)
	BYTE	m_bRange;			// 반경
	BYTE	m_bType;			// 타입(0:공격, 1:능력치변화, 2:콘트롤이상, 3:그래픽변환, 4:캐릭터 컨트롤, 5:순간이동/소환, 6:보호막, 7:캔슬매직, 8:전투 스타일)
	BYTE	m_bRate1;			// 적용률1(소숫점 1자리, 즉 계산시 10으로 나누어 계산해야 한다.)
	BYTE	m_bVar1;			// 적용변수1
	BYTE	m_bLimit1;			// 스킬 제한치1
	BYTE	m_bRate2;			// 적용률2(소숫점 1자리, 즉 계산시 10으로 나누어 계산해야 한다.)
	BYTE	m_bVar2;			// 적용변수2
	BYTE	m_bLimit2;			// 스킬 제한치2
	BYTE	m_bRate3;			// 적용률3(소숫점 1자리, 즉 계산시 10으로 나누어 계산해야 한다.)
	BYTE	m_bVar3;			// 적용변수3
	BYTE	m_bLimit3;			// 스킬 제한치3

};

#endif // !defined(AFX_MAGICTABLE_H__3BEFBF64_0058_48B1_AB56_D98E86016DFE__INCLUDED_)
