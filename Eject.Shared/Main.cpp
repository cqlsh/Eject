// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#include <scssdk.h>
#include <scssdk_telemetry.h>
#include <scssdk_telemetry_event.h>

#include "Eject.Logging/Log.hpp"
#include "Eject.Logging/LogLevel.hpp"
#include "Eject.Logging/LogSource.hpp"
#include "Eject.Shared/Sdk/GameLogSink.hpp"

namespace
{
    using Eject::Logging::Log;
    using Eject::Logging::LogLevel;
    using Eject::Logging::LogSource;
    using Eject::Shared::Sdk::GameLogSink;

    constexpr LogSource Source{.name = "Shared"};
    GameLogSink gameLogSink(nullptr);

    SCSAPI_VOID OnFrameEnd(scs_event_t, const void*, scs_context_t)
    {
        Log::Drain();
    }
}

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
{
    if (version != SCS_TELEMETRY_VERSION_1_01)
    {
        return SCS_RESULT_unsupported;
    }

    const auto* const versionParams = static_cast<const scs_telemetry_init_params_v101_t*>(params);
    if (versionParams->register_for_event(SCS_TELEMETRY_EVENT_frame_end, &OnFrameEnd, nullptr) != SCS_RESULT_ok)
    {
        return SCS_RESULT_generic_error;
    }

    gameLogSink = GameLogSink(versionParams->common.log);
    Log::AddSink(gameLogSink.Sink());
    Log::Write(LogLevel::Info, Source, "has been successfully loaded.");
    Log::Drain();
    return SCS_RESULT_ok;
}

SCSAPI_VOID scs_telemetry_shutdown()
{
    Log::Write(LogLevel::Info, Source, "was successfully ejected.");
    Log::Drain();
    Log::RemoveSink(gameLogSink.Sink());
}