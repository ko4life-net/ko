#include "N3DUtil.h"
/*
inline D3DCOLOR _RGB_To_D3DCOLOR(COLORREF cr, float fAlpha)
{
	D3DCOLOR cr2 =	(((DWORD)fAlpha)<<24) | 
					((cr & 0x000000ff)<<16) | // R
					(cr & 0x0000ff00) | // G
					((cr & 0x00ff0000)>>16); // B
	return cr2;
};

inline COLORREF _D3DCOLOR_To_RGB(D3DCOLOR& cr)
{
	COLORREF cr2 =	((cr & 0x00ff0000)>>16) | // R
					(cr & 0x0000ff00) | // G
					((cr & 0x000000ff)<<16); // B
	return cr2;
};

inline COLORREF _D3DCOLORVALUE_To_RGB(D3DCOLORVALUE& cr)
{
	COLORREF cr2 =	(((DWORD)(cr.r*255.0f))) | // R
					(((DWORD)(cr.g*255.0f))<<8) | // G
					(((DWORD)(cr.b*255.0f))<<16); // B
	return cr2;
};

inline D3DCOLORVALUE _RGB_To_D3DCOLORVALUE(COLORREF cr, float fAlpha)
{
	D3DCOLORVALUE cr2;
	cr2.a =	fAlpha; // Alpha
	cr2.r = (cr & 0x000000ff)/255.0f;
	cr2.g = ((cr & 0x0000ff00)>>8)/255.0f;
	cr2.b = ((cr & 0x00ff0000)>>16)/255.0f;
	return cr2;
};
*/