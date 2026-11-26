// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <cstdint>

namespace Eject::Platform::Threading
{
    class Worker
    {
    public:
        struct Task
        {
            void (*run)(void* context) = nullptr;
            void* context = nullptr;
        };

        bool Start(const Task& task);
        void Join();

    private:
        Task m_task;
        std::uintptr_t m_thread = 0;
    };
}