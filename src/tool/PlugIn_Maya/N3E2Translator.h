#if !defined(AFX_CN3E2Translator_h__INCLUDED_)
#define AFX_CN3E2Translator_h__INCLUDED_

/*==============================================*/
/* Header file for the VRML2.0 translator	*/
/*==============================================*/

#define MAYA_SRC

#include <maya/MPxFileTranslator.h>
#include <maya/MFnPlugin.h>

#include "N3E2Wrapper.h"

class CN3E2Translator : public MPxFileTranslator {

public:

	CN3E2Translator ();
	virtual ~CN3E2Translator ();
	static void    *creator ();

	MStatus         reader ( const MFileObject& file,
			                 const MString& optionsString,
			                 MPxFileTranslator::FileAccessMode mode);
	MStatus         writer ( const MFileObject& file,
		                     const MString& optionsString,
		                     MPxFileTranslator::FileAccessMode mode);

	bool            haveReadMethod () const;
	bool            haveWriteMethod () const;
	MString         defaultExtension () const;
	bool            canBeOpened () const;
	MFileKind       identifyFile ( const MFileObject& fileName,
				                   const char *buffer,
				                   short size) const;

	static MString	getVersion();

private:
	CN3E2Wrapper		m_Wrapper;
	
	static MString  magic;
	static MString	version;
};


// virtual MPxFileTranslator methods
inline bool CN3E2Translator::haveReadMethod () const
{
	return false;
}

inline bool CN3E2Translator::haveWriteMethod () const
{
	return true;
}

// ge2 has 5 different extensions -- grp seems to make sense as this is the
// scene file, but may need some modification here
// Whenever Maya needs to know the preferred extension of this file format,
// it calls this method.  For example, if the user tries to save a file called
// "test" using the Save As dialog, Maya will call this method and actually
// save it as "test.grp".  Note that the period should * not * be included in
// the extension.
inline MString CN3E2Translator::defaultExtension () const
{
	return "N3Scene";
}

// This method tells Maya whether the translator can open and import files
// (returns true) or only import  files (returns false)
inline bool CN3E2Translator::canBeOpened () const
{
	return false;
}

inline MString CN3E2Translator::getVersion()
{
	return version;
}

#endif // #if !defined(AFX_CN3E2Translator_h__INCLUDED_)

