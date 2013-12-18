
#include <Windows.h>
#include <Winbase.h>

#include "GlobalSupport.h"

bool GlobalSupport::enable_debug_privilege( bool bEnable )
{
    HANDLE hToken;
    LUID lUid;
    TOKEN_PRIVILEGES token_privileges;
    // get the procee token
    if (!OpenProcessToken(
        GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        return false;
    }

    // look up the token
    if (!LookupPrivilegeValue(
        NULL, SE_DEBUG_NAME, &token_privileges.Privileges[0].Luid))
    {
        CloseHandle(hToken);
        return false;
    }

    // change the privileges
    token_privileges.PrivilegeCount = 1;
    token_privileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
    if (!AdjustTokenPrivileges(
        hToken, FALSE,
        &token_privileges, sizeof(token_privileges),
        NULL, NULL))
    {
        CloseHandle(hToken);
        return false;
    }

    CloseHandle(hToken);
    return (GetLastError() == ERROR_SUCCESS);
}