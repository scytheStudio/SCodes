/*!
 * This file contains simple debug print definitions that allow for turning on/off debug print in the SCodes library by
 * adding SCODE_DEBUG in compile definitions
 */
#ifndef DEBUG_H
#define DEBUG_H
#include <QDebug>
#ifdef SCODES_DEBUG
#include <chrono>
#define sDebug QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
namespace SCodes {
struct TimeMeasure {
    const char *fname, *id;
    std::chrono::high_resolution_clock::time_point start;
    TimeMeasure(const char* fname, const char* id) : fname{fname}, id{id}, start{std::chrono::high_resolution_clock::now()} {};
    ~TimeMeasure(){
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float, std::chrono::milliseconds::period>(end-start);
        sDebug() << fname << " / " << id << "took: " << time.count() << " ms";
    }
};
}

#define SCODES_MEASURE(x) SCodes::TimeMeasure x (__FUNCTION__, #x)
#else
#define sDebug while (false) QMessageLogger().noDebug
#define SCODES_MEASURE(x) ((void)0)
#endif

#endif // DEBUG_H
