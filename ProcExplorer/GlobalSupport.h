#ifndef PROCEXPLORER_GLOBALSUPPORT_H_
#define PROCEXPLORER_GLOBALSUPPORT_H_

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#define ProcessEntry PROCESSENTRY32
#define CH_STRING(ch) QString::fromLocal8Bit(ch)

class GlobalSupport
{
public:
//     GlobalSupprot(void);
//     ~GlobalSupprot(void);
    // if debug privilege or normal privilege
    static bool enable_debug_privilege(bool);
};

#endif // PROCEXPLORER_GLOBALSUPPORT_H_
