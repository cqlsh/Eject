// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <string>

namespace Eject::Platform::Files
{
    class FileSystem
    {
    public:
        static bool CreateFolder(const std::string& path);
        static void Move(const std::string& sourcePath, const std::string& targetPath);
    };
}