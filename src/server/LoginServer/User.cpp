// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "LoginServer.h"
#include "LoginServerDlg.h"
#include "User.h"

#pragma warning(disable : 4786) // Visual C++ Only
#include <set>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser() {}

CUser::~CUser() {}

void CUser::Initialize() {
    m_pMain = (CLoginServerDlg *)AfxGetApp()->GetMainWnd();

    CIOCPSocket2::Initialize();
}

void CUser::CloseProcess() {
    CIOCPSocket2::CloseProcess();
}

void CUser::Parsing(int len, char * pData) {
    int  index = 0, send_index = 0, client_version = 0;
    char buff[2048];
    memset(buff, 0x00, 2048);
    BYTE command = GetByte(pData, index);

    switch (command) {
    case LS_VERSION_REQ:
        SetByte(buff, LS_VERSION_REQ, send_index);
        SetShort(buff, m_pMain->m_nLastVersion, send_index);
        Send(buff, send_index);
        break;
    case LS_SERVERLIST:
        m_pMain->m_DBProcess.LoadUserCountList(); // 기범이가 ^^;
        SetByte(buff, LS_SERVERLIST, send_index);
        SetByte(buff, m_pMain->m_nServerCount, send_index);
        for (int i = 0; i < m_pMain->m_ServerList.size(); i++) {
            SetShort(buff, strlen(m_pMain->m_ServerList[i]->strServerIP), send_index);
            SetString(buff, m_pMain->m_ServerList[i]->strServerIP, strlen(m_pMain->m_ServerList[i]->strServerIP),
                      send_index);
            SetShort(buff, strlen(m_pMain->m_ServerList[i]->strServerName), send_index);
            SetString(buff, m_pMain->m_ServerList[i]->strServerName, strlen(m_pMain->m_ServerList[i]->strServerName),
                      send_index);
            SetShort(buff, m_pMain->m_ServerList[i]->sUserCount, send_index); // 기범이가 ^^;
        }
        Send(buff, send_index);
        break;
    case LS_DOWNLOADINFO_REQ:
        client_version = GetShort(pData, index);
        SendDownloadInfo(client_version);
        break;
    case LS_LOGIN_REQ:
        LogInReq(pData + index);
        break;
    case LS_MGAME_LOGIN:
        MgameLogin(pData + index);
        break;
    case LS_NEWS: {
        constexpr char BOX_START[] = {'#', '\0'};
        constexpr char LINE_ENDING[] = {'\0'};
        constexpr char BOX_END[] = {'#', '\0'};

        std::ostringstream oss;

        for (const auto & news : m_pMain->m_ServerNews) {
            if (!news.IsActive) {
                continue;
            }

            oss << news.Title;
            oss.write(BOX_START, sizeof(BOX_START));
            oss << news.Content;
            oss.write(LINE_ENDING, sizeof(LINE_ENDING));
            oss.write(BOX_END, sizeof(BOX_END));
        }

        std::string fullContent = oss.str();
        std::string header = "Login Notice";

        SetByte(buff, LS_NEWS, send_index);

        int headerLen = static_cast<int>(header.size());
        SetShort(buff, headerLen, send_index);
        SetString(buff, header.data(), headerLen, send_index);

        if (!fullContent.empty()) {
            uint16_t size = static_cast<uint16_t>(fullContent.size());
            SetShort(buff, size, send_index);
            SetString(buff, fullContent.data(), size, send_index);
        } else {
            std::string emptyStr = "<empty>";
            int         emptyLen = static_cast<int>(emptyStr.size());
            SetShort(buff, emptyLen, send_index);
            SetString(buff, emptyStr.data(), emptyLen, send_index);
        }

        Send(buff, send_index);
        break;
    }
    }
}
void CUser::LogInReq(char * pBuf) {
    int  index = 0, idlen = 0, pwdlen = 0, send_index = 0, result = 0, serverno = 0;
    BOOL bCurrentuser = FALSE;
    char send_buff[256];
    memset(send_buff, 0x00, 256);
    char serverip[20];
    memset(serverip, 0x00, 20);
    char accountid[MAX_ID_SIZE + 1], pwd[13];
    memset(accountid, NULL, MAX_ID_SIZE + 1);
    memset(pwd, NULL, 13);

    idlen = GetShort(pBuf, index);
    if (idlen > MAX_ID_SIZE || idlen <= 0) {
        goto fail_return;
    }
    GetString(accountid, pBuf, idlen, index);
    pwdlen = GetShort(pBuf, index);
    if (pwdlen > 12 || pwdlen < 0) {
        goto fail_return;
    }
    GetString(pwd, pBuf, pwdlen, index);

    result = m_pMain->m_DBProcess.AccountLogin(accountid, pwd);
    SetByte(send_buff, LS_LOGIN_REQ, send_index);
    if (result == 1) { // success
        bCurrentuser = m_pMain->m_DBProcess.IsCurrentUser(accountid, serverip, serverno);
        if (bCurrentuser) {
            result = 0x05; // Kick out
            SetByte(send_buff, result, send_index);
            SetShort(send_buff, strlen(serverip), send_index);
            SetString(send_buff, serverip, strlen(serverip), send_index);
            SetShort(send_buff, serverno, send_index);
        } else {
            SetByte(send_buff, result, send_index);
        }
    } else {
        SetByte(send_buff, result, send_index);
    }
    Send(send_buff, send_index);

    return;
fail_return:
    SetByte(send_buff, LS_LOGIN_REQ, send_index);
    SetByte(send_buff, 0x02, send_index); // id, pwd 이상...
    Send(send_buff, send_index);
}

void CUser::MgameLogin(char * pBuf) {
    int  index = 0, idlen = 0, pwdlen = 0, send_index = 0, result = 0;
    char send_buff[256];
    memset(send_buff, 0x00, 256);
    char accountid[MAX_ID_SIZE + 1], pwd[13];
    memset(accountid, NULL, MAX_ID_SIZE + 1);
    memset(pwd, NULL, 13);

    idlen = GetShort(pBuf, index);
    if (idlen > MAX_ID_SIZE || idlen <= 0) {
        goto fail_return;
    }
    GetString(accountid, pBuf, idlen, index);
    pwdlen = GetShort(pBuf, index);
    if (pwdlen > 12) {
        goto fail_return;
    }
    GetString(pwd, pBuf, pwdlen, index);

    result = m_pMain->m_DBProcess.MgameLogin(accountid, pwd);
    SetByte(send_buff, LS_MGAME_LOGIN, send_index);
    SetByte(send_buff, result, send_index);
    Send(send_buff, send_index);

    return;
fail_return:
    SetByte(send_buff, LS_MGAME_LOGIN, send_index);
    SetByte(send_buff, 0x02, send_index); // login fail...
    Send(send_buff, send_index);
}

void CUser::SendDownloadInfo(short sVersion) {
    int  send_index = 0;
    char send_buff[2048]{};
    SetByte(send_buff, LS_DOWNLOADINFO_REQ, send_index);
    SetShort(send_buff, m_pMain->m_szFtpUrl.length(), send_index);
    SetString(send_buff, m_pMain->m_szFtpUrl.data(), m_pMain->m_szFtpUrl.length(), send_index);
    SetShort(send_buff, m_pMain->m_szFtpPath.length(), send_index);
    SetString(send_buff, m_pMain->m_szFtpPath.data(), m_pMain->m_szFtpPath.length(), send_index);

    std::set<std::string> vPatchFiles;
    for (const auto & [_, pInfo] : m_pMain->m_VersionList.m_UserTypeMap) {
        if (pInfo->sVersion == sVersion) {
            vPatchFiles.insert(pInfo->fsPatchFileName.string());
        }
    }
    SetShort(send_buff, vPatchFiles.size(), send_index);
    for (std::string szPatchFile : vPatchFiles) {
        SetShort(send_buff, szPatchFile.length(), send_index);
        SetString(send_buff, szPatchFile.data(), szPatchFile.length(), send_index);
    }

    Send(send_buff, send_index);
}
