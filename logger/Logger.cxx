/********************************************************************************
 * Copyright (C) 2014-2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "Logger.h"

#if FMT_VERSION < 60000
#include <fmt/time.h>
#else
#include <fmt/chrono.h>
#endif

#include <cstdio> // printf
#include <iostream>
#include <time.h> // time_t

using namespace std;

string GetColoredSeverityString(Severity severity)
{
    switch (severity) {
        case Severity::nolog:  return "\033[01;39mNOLOG\033[0m";  break;
        case Severity::fatal:  return "\033[01;31mFATAL\033[0m";  break;
        case Severity::error:  return "\033[01;31mERROR\033[0m";  break;
        case Severity::warn:   return "\033[01;33mWARN\033[0m";   break;
        case Severity::state:  return "\033[01;35mSTATE\033[0m";  break;
        case Severity::info:   return "\033[01;32mINFO\033[0m";   break;
        case Severity::debug:  return "\033[01;34mDEBUG\033[0m";  break;
        case Severity::trace:  return "\033[01;36mTRACE\033[0m";  break;
        default:               return "UNKNOWN";                  break;
    }
}

Verbosity Logger::fVerbosity = Verbosity::low;
Severity Logger::fSeverity = Severity::info;
bool Logger::fIsDestructed = false;
Logger::DestructionHelper fDestructionHelper;

#if defined(__APPLE__) || defined(__FreeBSD__)
const string Logger::fProcessName = getprogname();
#elif defined(_GNU_SOURCE)
const string Logger::fProcessName = program_invocation_short_name;
#else
const string Logger::fProcessName = "?";
#endif

const unordered_map<string, Verbosity> Logger::fVerbosityMap =
{
    { "veryhigh", Verbosity::veryhigh },
    { "high",     Verbosity::high     },
    { "medium",   Verbosity::medium   },
    { "low",      Verbosity::low      },
    { "verylow",  Verbosity::verylow  }
};

const unordered_map<string, Severity> Logger::fSeverityMap =
{
    { "nolog", Severity::nolog },
    { "fatal", Severity::fatal },
    { "error", Severity::error },
    { "warn",  Severity::warn  },
    { "state", Severity::state },
    { "info",  Severity::info  },
    { "debug", Severity::debug },
    { "trace", Severity::trace }
};

const array<string, 8> Logger::fSeverityNames =
{
    {
        "NOLOG",
        "FATAL",
        "ERROR",
        "WARN",
        "STATE",
        "INFO",
        "DEBUG",
        "TRACE"
    }
};

const array<string, 5> Logger::fVerbosityNames =
{
    {
        "verylow",
        "low",
        "medium",
        "high",
        "veryhigh"
    }
};

Logger::Logger(Severity severity, const string& file, const string& line, const string& func)
{
    if (!fIsDestructed) {
        using namespace std::chrono;
        if (fVerbosity >= Verbosity::high) {
            time_point<system_clock> now = system_clock::now();
            fmt::format_to(fPrefix, "[{}][{}{:%H:%M:%S}.{:06}{}]", ColorOut(Color::fgBlue, fProcessName), startColor(Color::fgCyan), fmt::localtime(system_clock::to_time_t(now)), (duration_cast<microseconds>(now.time_since_epoch()) % 1000000).count(), endColor());
        } else if (fVerbosity == Verbosity::medium) {
            time_point<system_clock> now = system_clock::now();
            fmt::format_to(fPrefix, "[{}{:%H:%M:%S}{}]", startColor(Color::fgCyan), fmt::localtime(system_clock::to_time_t(now)), endColor());
        }

        if (fVerbosity > Verbosity::verylow) {
            fmt::format_to(fPrefix, "[{}]", GetColoredSeverityString(severity));
        }

        if (fVerbosity == Verbosity::veryhigh) {
            size_t pos = file.rfind("/");
            string shortfile = file.substr(pos + 1);
            fmt::format_to(fPrefix, "[{}:{}:{}]", ColorOut(Color::fgBlue, shortfile), ColorOut(Color::fgYellow, line), ColorOut(Color::fgBlue, func));
        }

        if (fVerbosity != Verbosity::verylow) {
            fmt::format_to(fPrefix, " ");
        }
    }
}

Logger::~Logger() noexcept(false)
{
    if (fIsDestructed) {
        printf("post-static destruction output: %s\n", fContent.str().c_str());
        return;
    }

    // "\n" + flush instead of endl makes output thread safe.
    fmt::print("{}{}\n", to_string(fPrefix), fContent.str());
    cout << flush;
}

void Logger::SetSeverity(const string& severityStr)
{
    if (fSeverityMap.count(severityStr)) {
        SetSeverity(fSeverityMap.at(severityStr));
    } else {
        LOG(error) << "Unknown severity setting: '" << severityStr << "', setting to default 'info'.";
        SetSeverity(Severity::info);
    }
}

bool Logger::Logging(Severity severity)
{
    if (Severity::fatal == severity) {
        return true;
    }
    return severity <= fSeverity && severity > Severity::nolog;
}

bool Logger::Logging(const string& severityStr)
{
    if (fSeverityMap.count(severityStr)) {
        return Logging(fSeverityMap.at(severityStr));
    } else {
        LOG(error) << "Unknown severity setting: '" << severityStr;
        return false;
    }
}

void Logger::SetVerbosity(const string& verbosityStr)
{
    if (fVerbosityMap.count(verbosityStr)) {
        fVerbosity = fVerbosityMap.at(verbosityStr);
    } else {
        LOG(error) << "Unknown verbosity setting: '" << verbosityStr << "', setting to default 'low'.";
        fVerbosity = Verbosity::low;
    }
}

Logger& Logger::operator<<(ios_base& (*manip) (ios_base&))
{
    fContent << manip;
    return *this;
}

Logger& Logger::operator<<(ostream& (*manip) (ostream&))
{
    fContent << manip;
    return *this;
}
