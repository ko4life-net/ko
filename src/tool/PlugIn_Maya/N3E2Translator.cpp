// Copyright (C) 1998-2000 Alias | Wavefront, a division of Silicon Graphics Limited.
//
// The information in this file is provided for the exclusive use of the
// licensees of Alias | Wavefront.  Such users have the right to use, modify,
// and incorporate this code into other products for purposes authorized
// by the Alias | Wavefront license agreement, without fee.
//
// ALIAS | WAVEFRONT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
// EVENT SHALL ALIAS | WAVEFRONT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
// DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.
//
// RCSfile:CN3E2Translator.h $ $Revision: /main/13 $
//
//
// N3E2 Translator Maya specific source
//

#include "StdAfx.h"

#include "N3E2Translator.h"

#include <unordered_set>

// Initialize our magic "number"
MString CN3E2Translator::magic("filetype gx");
// A GE2 file is an ascii file where the 1st line contains
// the string "filetype gx"

MString CN3E2Translator::version("2.0");

CN3E2Translator::CN3E2Translator() {}

CN3E2Translator::~CN3E2Translator() {}

void * CN3E2Translator::creator() {
    return new CN3E2Translator();
}

//
//
// The reader is not implemented yet.
//
MStatus CN3E2Translator::reader(const MFileObject & file, const MString & options,
                                MPxFileTranslator::FileAccessMode mode) {
    MStatus rval(MS::kSuccess);

    return rval;
}

// Write method of the N3E2 translator / file exporter
MStatus CN3E2Translator::writer(const MFileObject & fileObject, const MString & options,
                                MPxFileTranslator::FileAccessMode mode) {
    // Lets now do all of the option processing
    if (options.length() > 0) {
        //Start parsing.
        MStringArray optionList;
        MStringArray theOption;

        options.split(';', optionList);
    }

    fs::path fsFile = fileObject.name().asUTF8();
    fsFile.make_lower();
    if (!n3std::iequals(fsFile.extension(), ".n3scene")) {
        fsFile.replace_extension(".n3scene");
    }

    m_Wrapper.SetFileName(fsFile);
    m_Wrapper.SetPath(fileObject.path().asChar());
    m_Wrapper.SceneExport();

    m_Wrapper.Release();

    return MS::kSuccess;
}

MPxFileTranslator::MFileKind CN3E2Translator::identifyFile(const MFileObject & fileName, const char * buffer,
                                                           short size) const {
    static const std::unordered_set<std::string> vSupportedExts = {
        ".N3Scene", ".N3Camera", ".N3Light", ".N3Mesh", ".N3PMesh", ".N3Shape", ".N3Joint", ".N3IMesh", ".N3DSKN"};

    fs::path fsFile = fileName.name().asUTF8();
    if (vSupportedExts.contains(fsFile.extension().string())) {
        return kIsMyFileType;
    }

    return kNotMyFileType;
}

// Shouldn't be any C functions trying to call us, should there?
/*extern "C" */ MStatus initializePlugin(MObject obj) {
    MStatus   status;
    MFnPlugin plugin(obj, "Noah System", CN3E2Translator::getVersion().asChar(), "Any");

    /*
    // Get Maya-set values and initialize them --
    // if user wants to override them then that's ok
    MTime startFrame( MAnimControl::minTime().value(), MTime::uiUnit() );
    MTime endFrame( MAnimControl::maxTime().value(), MTime::uiUnit() );
    MTime currFrame( MAnimControl::currentTime().value(), MTime::uiUnit() );
    MAnimControl::PlaybackMode playbackMode = MAnimControl::playbackMode();
    MAnimControl::PlaybackViewMode playbackViewMode = MAnimControl::viewMode();
*/

    //Register the translator with the system (NAME, image, creator func. mel script
    status = plugin.registerFileTranslator("N3Export2", "N3E2Translator.rgb", CN3E2Translator::creator);

    if (!status) {
        status.perror("registerFileTranslator");
        return status;
    }

    return status;
}

// Shouldn't be any C functions trying to call us, should there?
/*extern "C" */ MStatus uninitializePlugin(MObject obj) {
    MStatus   status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterFileTranslator("N3Export2");
    if (!status) {
        status.perror("deregisterFileTranslator");
        return status;
    }

    return status;
}
