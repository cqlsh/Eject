// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#include "Eject.Shared/Sdk/GameLogSink.hpp"

#include <scssdk.h>

#include "Eject.Logging/LogEntry.hpp"
#include "Eject.Logging/LogLevel.hpp"
#include "Eject.Logging/LogSink.hpp"
#include "Eject.Text/Formatting/TextBuilder.hpp"

namespace Eject::Shared::Sdk
{
    GameLogSink::GameLogSink(scs_log_t log)
        : m_log(log)
    {
    }

    Eject::Logging::LogSink GameLogSink::Sink()
    {
        return Eject::Logging::LogSink{
            .write = &GameLogSink::Write,
            .context = this,
            .minimumLevel = Eject::Logging::LogLevel::Info};
    }

    void GameLogSink::Write(void* context, const Eject::Logging::LogEntry& entry)
    {
        auto* const sink = static_cast<GameLogSink*>(context);
        scs_log_type_t type = SCS_LOG_TYPE_message;
        switch (entry.level)
        {
            case Eject::Logging::LogLevel::Debug:
            case Eject::Logging::LogLevel::Info:
                break;
            case Eject::Logging::LogLevel::Warning:
                type = SCS_LOG_TYPE_warning;
                break;
            case Eject::Logging::LogLevel::Error:
                type = SCS_LOG_TYPE_error;
                break;
        }

        Eject::Text::Formatting::TextBuilder builder(sink->m_line);
        builder.Append(Prefix);
        builder.Append(entry.message);
        sink->m_log(type, builder.Terminate());
    }
}