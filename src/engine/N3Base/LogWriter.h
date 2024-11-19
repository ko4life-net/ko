#pragma once

class CLogWriter {
  public:
    static void Open(const fs::path & fsFile);
    static void Close();

    template <class... T> static void Write(std::string_view szFmt, T &&... fmtArgs) {
        WriteImpl(szFmt, std::make_format_args(fmtArgs...));
    }

  private:
    static void WriteImpl(std::string_view szFmt, const std::format_args fmtArgs);

  private:
    static fs::path s_fsLogFile;
};
