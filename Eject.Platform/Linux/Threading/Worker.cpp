// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#include "Eject.Platform/Threading/Worker.hpp"

#include <cstdint>

#include <pthread.h>

namespace Eject::Platform::Threading
{
    namespace
    {
        void* Run(void* const parameter)
        {
            const auto* const task = static_cast<Worker::Task*>(parameter);
            task->run(task->context);
            return nullptr;
        }
    }

    bool Worker::Start(const Task& task)
    {
        m_task = task;
        pthread_t thread{};
        if (pthread_create(&thread, nullptr, &Run, &m_task) != 0)
        {
            return false;
        }

        m_thread = static_cast<std::uintptr_t>(thread);
        return true;
    }

    void Worker::Join()
    {
        if (m_thread == 0)
        {
            return;
        }

        pthread_join(static_cast<pthread_t>(m_thread), nullptr);
        m_thread = 0;
    }
}