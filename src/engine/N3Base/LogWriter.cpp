#include "StdAfx.h"
#include "LogWriter.h"

fs::path CLogWriter::s_fsLogFile;

static std::mutex s_mtxLog;

void CLogWriter::Open(const fs::path & fsFile) {
    if (fsFile.empty()) {
        return;
    }

    std::lock_guard<std::mutex> lock(s_mtxLog);

    s_fsLogFile = fsFile;

    // Remove the file if its size exceeds 512 KB
    if (fs::exists(s_fsLogFile) && fs::file_size(s_fsLogFile) > 512000) {
        fs::remove(s_fsLogFile);
    }

    std::ofstream ofsFile(s_fsLogFile, std::ios::app | std::ios::binary);
    if (!ofsFile) {
        return;
    }

    auto tpNow = std::chrono::system_clock::now();
    auto tmLocal = std::chrono::current_zone()->to_local(tpNow);

    ofsFile << "---------------------------------------------------------------------------\n";
    ofsFile << std::format("// Begin writing log... [{:%m/%d %H:%M}]\n", tmLocal);
}

void CLogWriter::Close() {
    if (s_fsLogFile.empty()) {
        return;
    }

    std::lock_guard<std::mutex> lock(s_mtxLog);

    std::ofstream ofsFile(s_fsLogFile, std::ios::app | std::ios::binary);
    if (!ofsFile) {
        return;
    }

    auto tpNow = std::chrono::system_clock::now();
    auto tmLocal = std::chrono::current_zone()->to_local(tpNow);

    ofsFile << std::format("// End writing log... [{:%m/%d %H:%M}]\n", tmLocal);
    ofsFile << "---------------------------------------------------------------------------\n";

    s_fsLogFile.clear();
}

void CLogWriter::WriteImpl(std::string_view szFmt, const std::format_args fmtArgs) {
    if (s_fsLogFile.empty() || szFmt.empty()) {
        return;
    }

    std::lock_guard<std::mutex> lock(s_mtxLog);

    std::string szMsg = std::vformat(szFmt, fmtArgs);
    N3_DEBUG("CLogWriter::Write: {:s}", szMsg);

    auto tpNow = std::chrono::system_clock::now();
    auto tmLocal = std::chrono::current_zone()->to_local(tpNow);

    std::string szLog = std::format("    [{:%H:%M:%S}] ", tmLocal) + szMsg + "\n";

    std::ofstream ofsFile(s_fsLogFile, std::ios::app | std::ios::binary);
    if (ofsFile) {
        ofsFile.write(szLog.c_str(), szLog.size());
    }
}
