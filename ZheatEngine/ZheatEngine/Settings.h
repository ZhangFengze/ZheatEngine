#pragma once

#include <atomic>

#include <QString>
#include <QDir>

inline std::atomic_uint gUIRefreshResultsInterval = 100; // milliseconds
inline std::atomic_uint gUIRefreshWatchListInterval = 100; // milliseconds
inline std::atomic_uint gPreferredMemoryUsage = 3072; // mega bytes
inline std::atomic_uint gMaxShownResults = 10000;
inline QString gTempDir = QDir::tempPath();

inline bool gScanMEM_PRIVATE = true;
inline bool gScanMEM_IMAGE = true;
inline bool gScanMEM_MAPPED = false;
