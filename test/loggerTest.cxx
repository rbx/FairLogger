/********************************************************************************
 * Copyright (C) 2014-2019 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH  *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include <Logger.h>

#include <iostream>
#include <vector>
#include <thread>

using namespace std;

void printEverySeverity()
{
    static int i = 1;

    LOG(nolog) << "nolog message "   << i++;
    LOG(error) << "error message "   << i++;
    LOG(warn)  << "warning message " << i++;
    LOG(state) << "state message "   << i++;
    LOG(info)  << "info message "    << i++;
    LOG(debug) << "debug message "   << i++;
    LOG(trace) << "trace message "   << i++;
}

void printAllVerbositiesWithSeverity(Severity sev)
{
    Logger::SetSeverity(sev);

    cout << endl << "cout: >>> testing severity '" << Logger::SeverityName(sev) << "' with 'verylow' verbosity..." << endl;
    Logger::SetVerbosity(Verbosity::verylow);
    printEverySeverity();
    cout << endl << "cout: >>> testing severity '" << Logger::SeverityName(sev) << "' with 'low' verbosity..." << endl;
    Logger::SetVerbosity(Verbosity::low);
    printEverySeverity();
    cout << endl << "cout: >>> testing severity '" << Logger::SeverityName(sev) << "' with 'medium' verbosity..." << endl;
    Logger::SetVerbosity(Verbosity::medium);
    printEverySeverity();
    cout << endl << "cout: >>> testing severity '" << Logger::SeverityName(sev) << "' with 'high' verbosity..." << endl;
    Logger::SetVerbosity(Verbosity::high);
    printEverySeverity();
    cout << endl << "cout: >>> testing severity '" << Logger::SeverityName(sev) << "' with 'veryhigh' verbosity..." << endl;
    Logger::SetVerbosity(Verbosity::veryhigh);
    printEverySeverity();
}

void silentlyPrintAllVerbositiesWithSeverity(Severity sev)
{
    Logger::SetSeverity(sev);

    Logger::SetVerbosity(Verbosity::verylow);
    printEverySeverity();
    Logger::SetVerbosity(Verbosity::low);
    printEverySeverity();
    Logger::SetVerbosity(Verbosity::medium);
    printEverySeverity();
    Logger::SetVerbosity(Verbosity::high);
    printEverySeverity();
    Logger::SetVerbosity(Verbosity::veryhigh);
    printEverySeverity();
}

int main()
{
    cout << "cout: testing severities..." << endl;

    printAllVerbositiesWithSeverity(Severity::trace);
    printAllVerbositiesWithSeverity(Severity::debug);
    printAllVerbositiesWithSeverity(Severity::info);
    printAllVerbositiesWithSeverity(Severity::state);
    printAllVerbositiesWithSeverity(Severity::warn);
    printAllVerbositiesWithSeverity(Severity::error);
    printAllVerbositiesWithSeverity(Severity::nolog);

    cout << endl;
    cout << "cout: setting severity to 'info' and verbosity to 'medium'" << endl;
    Logger::SetSeverity(Severity::info);
    Logger::SetVerbosity(Verbosity::medium);

    cout << "cout: is logging trace: " << Logger::Logging(Severity::trace) << endl;
    cout << "cout: is logging debug: " << Logger::Logging(Severity::debug) << endl;
    cout << "cout: is logging info: " << Logger::Logging(Severity::info) << endl;
    cout << "cout: is logging state: " << Logger::Logging(Severity::state) << endl;
    cout << "cout: is logging warn: " << Logger::Logging(Severity::warn) << endl;
    cout << "cout: is logging error: " << Logger::Logging(Severity::error) << endl;
    cout << "cout: is logging fatal: " << Logger::Logging(Severity::fatal) << endl;
    cout << "cout: is logging nolog: " << Logger::Logging(Severity::nolog) << endl;

    for (int i = 0; i < 1000000; ++i) {
        silentlyPrintAllVerbositiesWithSeverity(Severity::nolog);
    }
    cout << endl;
    cout << "cout: setting severity to 'trace' and verbosity to 'veryhigh'" << endl;
    Logger::SetSeverity(Severity::trace);
    Logger::SetVerbosity(Verbosity::veryhigh);

    LOGF(info, "Hello {} {}!", "world", ":-)");
    LOGP(info, "Hello %s %s!", "world", ":-)");

    return 0;
}
