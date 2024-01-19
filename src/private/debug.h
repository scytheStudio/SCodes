/*!
 * This file contains simple debug print definitions that allow for turning on/off debug print in the SCodes library by
 * adding SS_CODE_DEBUG in compile definitions
 */
#ifndef DEBUG_H
#define DEBUG_H
#include <QDebug>
#ifdef SS_SCODE_DEBUG
#define sDebug QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#else
#define sDebug while (false) QMessageLogger().noDebug
#endif

#endif // DEBUG_H
