/********************************************************************************
 * Copyright (C) 2014-2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef LOGGER_H
#define LOGGER_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

#include <fmt/core.h>
#include <fmt/printf.h>

#pragma GCC diagnostic pop

#include <array>
#include <chrono>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>

enum class Severity : int
{
    nolog,
    fatal,
    error,
    warn,
    state,
    info,
    debug,
    trace
};

enum class Verbosity : int
{
    verylow,
    low,
    medium,
    high,
    veryhigh
};

class Logger
{
  public:
    Logger(Severity severity, const std::string& file, const std::string& line, const std::string& func);
    virtual ~Logger() noexcept(false);

    Logger& Log() { return *this; }

    enum class Color : int
    {
        bold           = 1,
        dim            = 2,
        underline      = 4,
        blink          = 5,
        reverse        = 7,
        hidden         = 8,

        fgDefault      = 39,
        fgBlack        = 30,
        fgRed          = 31,
        fgGreen        = 32,
        fgYellow       = 33,
        fgBlue         = 34,
        fgMagenta      = 35,
        fgCyan         = 36,
        fgLightGray    = 37,
        fgDarkGray     = 90,
        fgLightRed     = 91,
        fgLightGreen   = 92,
        fgLightYellow  = 93,
        fgLightBlue    = 94,
        fgLightMagenta = 95,
        fgLightCyan    = 96,
        fgWhite        = 97,

        bgDefault      = 49,
        bgBlack        = 40,
        bgRed          = 41,
        bgGreen        = 42,
        bgYellow       = 43,
        bgBlue         = 44,
        bgMagenta      = 45,
        bgCyan         = 46,
        bgLightGray    = 47,
        bgDarkGray     = 100,
        bgLightRed     = 101,
        bgLightGreen   = 102,
        bgLightYellow  = 103,
        bgLightBlue    = 104,
        bgLightMagenta = 105,
        bgLightCyan    = 106,
        bgWhite        = 107
    };

    static std::string startColor(Color color) { return fmt::format("\033[01;{}m", static_cast<int>(color)); }
    static std::string endColor() { return "\033[0m"; }
    static std::string ColorOut(Color c, const std::string& s) { return fmt::format("\033[01;{}m{}\033[0m", static_cast<int>(c), s); }

    static Severity GetSeverity() { return fSeverity; }
    static void SetSeverity(const Severity severity) { fSeverity = severity; }
    static void SetSeverity(const std::string& severityStr);

    static bool Logging(const Severity severity);
    static bool Logging(const std::string& severityStr);

    static void SetVerbosity(const Verbosity verbosity) { fVerbosity = verbosity; }
    static void SetVerbosity(const std::string& verbosityStr);
    static Verbosity GetVerbosity() { return fVerbosity; }

    static std::string SeverityName(Severity s) { return fSeverityNames.at(static_cast<size_t>(s)); }
    static std::string VerbosityName(Verbosity v) { return fVerbosityNames.at(static_cast<size_t>(v)); }

    template<typename T>
    Logger& operator<<(const T& t)
    {
        fContent << t;
        return *this;
    }

    // overload for char* to make sure it is not nullptr
    Logger& operator<<(const char* cptr)
    {
        if (cptr != nullptr) {
            fContent << cptr;
        }
        return *this;
    }

    // overload for char* to make sure it is not nullptr
    Logger& operator<<(char* cptr)
    {
        if (cptr != nullptr) {
            fContent << cptr;
        }
        return *this;
    }

    Logger& operator<<(std::ios_base& (*manip) (std::ios_base&));
    Logger& operator<<(std::ostream& (*manip) (std::ostream&));

    static const std::unordered_map<std::string, Verbosity> fVerbosityMap;
    static const std::unordered_map<std::string, Severity> fSeverityMap;
    static const std::array<std::string, 8> fSeverityNames;
    static const std::array<std::string, 5> fVerbosityNames;

    // protection for use after static destruction took place
    static bool fIsDestructed;
    static struct DestructionHelper { ~DestructionHelper() { Logger::fIsDestructed = true; }} fDestructionHelper;

  private:
    std::ostringstream fContent;
    fmt::memory_buffer fPrefix;
    static const std::string fProcessName;

    static Severity fSeverity;
    static Verbosity fVerbosity;
};

#define IMP_CONVERTTOSTRING(s) # s
#define CONVERTTOSTRING(s) IMP_CONVERTTOSTRING(s)

#define MSG_ORIGIN __FILE__, CONVERTTOSTRING(__LINE__), static_cast<const char*>(__FUNCTION__)

// Log line if the provided severity is below or equals the configured one
#define LOG(severity) \
    for (bool fairLOggerunLikelyvariable = false; Logger::Logging(Severity::severity) && !fairLOggerunLikelyvariable; fairLOggerunLikelyvariable = true) \
        Logger(Severity::severity, MSG_ORIGIN)

// Log with fmt- or printf-like formatting
#define LOGF(severity, ...) LOG(severity) << fmt::format(__VA_ARGS__)
#define LOGP(severity, ...) LOG(severity) << fmt::sprintf(__VA_ARGS__)

#endif // LOGGER_H
