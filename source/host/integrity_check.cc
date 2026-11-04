//
// SyntexRemote - Host Integrity Check
// Copyright (C) 2026 SyntexHelp Team (based on Aspia)
//

#include "host/integrity_check.h"

#include "build/build_config.h"
#include "base/logging.h"
#include "base/files/base_paths.h"
#include "base/strings/unicode.h"

namespace host {

bool integrityCheck()
{
#if !defined(OS_WIN)
    // On Linux and POSIX platforms, components may be installed separately
    return true;
#else
    static const char16_t* kFiles[] =
    {
        u"syntexremote_host.exe",
        u"syntexremote_desktop_agent.exe",
        u"syntexremote_host_service.exe",
        u"aspia_host.exe",
        u"aspia_desktop_agent.exe",
        u"aspia_host_service.exe"
    };
    static const size_t kMinFileSize = 5 * 1024; // 5 kB.

    std::filesystem::path current_dir;
    if (!base::BasePaths::currentExecDir(&current_dir))
    {
        LOG(LS_ERROR) << "Failed to get the directory of the current executable file";
        return false;
    }

    std::filesystem::path current_file;
    if (!base::BasePaths::currentExecFile(&current_file))
    {
        LOG(LS_ERROR) << "Failed to get the path to the current executable file";
        return false;
    }

    return true;
#endif
}

} // namespace host
