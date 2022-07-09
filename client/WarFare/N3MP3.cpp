// N3Mp3.cpp: implementation of the CN3Mp3 class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "N3Mp3.h"
#include <math.h>
#include <malloc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3Mp3::CN3Mp3()
{
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaSeeking = NULL;
	m_pBasicAudio = NULL;
	m_pMediaEvent = NULL;

	m_IsLoop = false;
	m_Active = false;
	ZeroMemory(m_FileName, MAX_PATH);
}

CN3Mp3::~CN3Mp3()
{
	Release();
}


//
//	Init()
//
bool CN3Mp3::Init()
{
	Release();

	if (FAILED (CoInitialize(NULL)) ) return false;

	if(CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, reinterpret_cast <void **> (&m_pGraphBuilder)) != S_OK) return false;
	if(m_pGraphBuilder->QueryInterface(IID_IMediaControl, reinterpret_cast <void **> (&m_pMediaControl)) != S_OK) return false;
	if(m_pGraphBuilder->QueryInterface (IID_IMediaSeeking, reinterpret_cast <void **> (&m_pMediaSeeking)) != S_OK) return false;
	if(m_pGraphBuilder->QueryInterface (IID_IBasicAudio, reinterpret_cast <void **> (&m_pBasicAudio)) != S_OK) return false;
	if(m_pGraphBuilder->QueryInterface (IID_IMediaEvent, reinterpret_cast <void **> (&m_pMediaEvent)) != S_OK) return false;

	m_IsLoop = false;
	m_Active = false;
	ZeroMemory(m_FileName, MAX_PATH);

	return true;
}


//
//
//
void CN3Mp3::Release()
{
	ClearGraph();

	// 인터페이스들을 릴리즈 한다
	if(m_pMediaEvent) { m_pMediaEvent->Release(); m_pMediaEvent = NULL; }
	if(m_pBasicAudio) { m_pBasicAudio->Release(); m_pBasicAudio = NULL; }
	if(m_pMediaSeeking) { m_pMediaSeeking->Release(); m_pMediaSeeking = NULL; }
	if(m_pMediaControl) { m_pMediaControl->Release(); m_pMediaControl = NULL; }
	if(m_pGraphBuilder) { m_pGraphBuilder->Release(); m_pGraphBuilder = NULL; }

	// COM을 uninitialize한다
	CoUninitialize ();
}


//
//
//
bool CN3Mp3::ClearGraph()
{
	if(!m_pGraphBuilder) return false;

	IEnumFilters*	pFilterEnum;
	IBaseFilter**	ppFilters;

	if(m_pMediaControl) m_pMediaControl->Stop();	// 연주를 멈춘다.

	// 그래프 내의 모든 필터를 얻기 위해 EnumFilters 포인터를 얻는다
	if (m_pGraphBuilder->EnumFilters(&pFilterEnum) != S_OK)	return false;

	int iFiltCount = 0;
	int iPos = 0;

	// 필터의 갯수를 파악
	while (S_OK == pFilterEnum->Skip (1)) iFiltCount++;

	ppFilters = reinterpret_cast <IBaseFilter **> (_alloca (sizeof (IBaseFilter *) * iFiltCount));
	
	pFilterEnum->Reset ();

	// 필터에 대한 포인터를 모두 얻어온다
	while (S_OK == pFilterEnum->Next (1, &(ppFilters[iPos++]), NULL));

	if(pFilterEnum) { pFilterEnum->Release(); pFilterEnum = NULL; }

	// 모든 필터를 그래프에서 제거한다
	for (iPos = 0; iPos < iFiltCount; iPos++)
	{
		m_pGraphBuilder->RemoveFilter (ppFilters[iPos]);
		if(!ppFilters[iPos]) continue;
		while (ppFilters[iPos]->Release () != 0);	// ref count 수 만큼 한다
		ppFilters[iPos] = NULL;
	}

	return true;
}


//
//
//
void CN3Mp3::Tick()
{
	if(!m_Active) return;

	long lEventCode = 0;
	long lParam1, lParam2;

	if(m_IsLoop)
	{
		if(m_pMediaEvent)
		{
			m_pMediaEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 1);
			if(lEventCode == EC_COMPLETE) 
			{
				Play(true);
			}
		}
	}
}


//
//
//
void CN3Mp3::Looping(bool loop)
{
	m_IsLoop = loop;
}


//
//
//
bool CN3Mp3::Load(char* szPathName)
{
	WCHAR wFileName[MAX_PATH];
	IBaseFilter*	pFilter;
	IPin*			pPin;

	#ifndef UNICODE
		MultiByteToWideChar (CP_ACP, 0, szPathName, -1, wFileName, MAX_PATH);
	#else
		lstrcpy (wFileName, szPathName);
	#endif

	Stop();
	if(!ClearGraph()) return false;

	if(m_pGraphBuilder->AddSourceFilter(wFileName, wFileName, &pFilter) != S_OK) return false;

	if (pFilter->FindPin(L"Output", &pPin) != S_OK) return false;
	
	if (m_pGraphBuilder->Render(pPin) != S_OK)
	{
		if(pPin) { pPin->Release(); pPin = NULL; }
		if(pFilter) { pFilter->Release(); pFilter = NULL; }
		return false;
	}

	if(pPin) { pPin->Release(); pPin = NULL; }
	if(pFilter) { pFilter->Release(); pFilter = NULL; }

	wsprintf(m_FileName, szPathName);
	return true;
}


//
//
//
void CN3Mp3::Play(bool New)
{
	if(!m_Active) return;
	if(New)	SetPosition(0);
	m_pMediaControl->Run();
}


//
//
//
void CN3Mp3::Pause()
{
	if(!m_Active) return;
	m_pMediaControl->Pause();
}


//
//
//
void CN3Mp3::Stop()
{
	if(!m_Active) return;
	m_pMediaControl->Stop();
}


//
//
//
void CN3Mp3::SetPosition(LONGLONG pos)
{
	if(!m_Active) return;
	m_pMediaSeeking->SetPositions (&pos, AM_SEEKING_AbsolutePositioning, &pos, AM_SEEKING_NoPositioning);
}


//
//
//
int CN3Mp3::GetVolume()
{
	long lVol;
	m_pBasicAudio->get_Volume(&lVol);
	if(lVol == (-10000)) return 0;
	return (int)(pow(10,(double)((float)lVol/3000.0f))*100.0f);
}


//
//
//
void CN3Mp3::SetVolume(int Vol)
{
	if(Vol==0)
	{
		m_pBasicAudio->put_Volume(-10000);
		return;
	}

	float fVol = (float)(Vol) / 100.0f;
	
	long dwVol = (long)(log10(fVol) * 3000);
	m_pBasicAudio->put_Volume(dwVol);
}


//
//
//
long CN3Mp3::GetBalance()
{
	long bal;
	m_pBasicAudio->get_Balance(&bal);
	return bal;
}


//
//
//
void CN3Mp3::SetBalance(long bal)
{
	m_pBasicAudio->put_Balance(bal);
}


//
//
//
FILTER_STATE CN3Mp3::GetFilterState()
{
	FILTER_STATE state;
	m_pMediaControl->GetState(0, reinterpret_cast <long *> (&state));
	return state;
}


//
//
//
bool CN3Mp3::IsPlaying()
{
	if(GetFilterState() == State_Running ) return true;
	return false;	
}