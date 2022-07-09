// Copyright (C) 1997-2001 Alias|Wavefront,
//                         a division of Silicon Graphics Limited.
// 
// The information in this file is provided for the exclusive use of the
// licensees of Alias|Wavefront.  Such users have the right to use, modify,
// and incorporate this code into other products for purposes authorized
// by the Alias|Wavefront license agreement, without fee.
// 
// ALIAS|WAVEFRONT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
// EVENT SHALL ALIAS|WAVEFRONT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
// DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef _iffreader_h
#define _iffreader_h

#include <math.h>
#include <maya/ilib.h>

typedef unsigned char byte;

class MStatus;
class MString;

class IFFimageReader
{
public:
	IFFimageReader ();
	~IFFimageReader ();
	MStatus open (MString filename);
	MStatus close ();
	MStatus readImage ();
	MStatus getSize (int &x, int &y);
	MString errorString ();
	int getBytesPerChannel ();
	bool isRGB ();
	bool isGrayscale ();
	bool hasDepthMap ();
	bool hasAlpha ();
	MStatus getPixel (int x, int y, int *r, int *g, int *b, int *a = NULL);
	MStatus getDepth (int x, int y, float *d);
	const byte *getPixelMap () const;
	const float *getDepthMap () const;

protected:
	ILimage *fImage;
	byte *fBuffer;
	float *fZBuffer;
};

#endif
