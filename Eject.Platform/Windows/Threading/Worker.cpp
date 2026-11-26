// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#include "Eject.Platform/Threading/Worker.hpp"

#include <bit>
#include <cstdint>

#include <Windows.h>

namespace Eject::Platform::Threading
{
    namespace
    {
        DWORD WINAPI Run(void* const parameter)
        {
            const auto* const task = static_cast<Worker::Task*>(parameter);
            task->run(task->context);
            return 0;
        }
    }

    bool Worker::Start(const Task& task)
    {
        m_task = task;
        m_thread = std::bit_cast<std::uintptr_t>(CreateThread(nullptr, 0, &Run, &m_task, 0, nullptr));
        return m_thread != 0;
    }

    void Worker::Join()
    {
        if (m_thread == 0)
        {
            return;
        }

        auto* const thread = std::bit_cast<HANDLE>(m_thread);
        WaitForSingleObject(thread, INFINITE);
        CloseHandle(thread);
        m_thread = 0;
    }
}