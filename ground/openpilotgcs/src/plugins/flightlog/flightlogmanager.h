/**
 ******************************************************************************
 *
 * @file       flightlogmanager.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2012.
 * @addtogroup [Group]
 * @{
 * @addtogroup FlightLogManager
 * @{
 * @brief [Brief]
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef FLIGHTLOGMANAGER_H
#define FLIGHTLOGMANAGER_H

#include <QObject>
#include <QList>
#include <QQmlListProperty>
#include <QSemaphore>

#include "uavobjectmanager.h"
#include "debuglogentry.h"
#include "debuglogstatus.h"
#include "debuglogcontrol.h"

class ExtendedDebugLogEntry : public DebugLogEntry {
    Q_OBJECT
    Q_PROPERTY(QString LogString READ getLogString WRITE setLogString NOTIFY LogStringUpdated)

public:
    explicit ExtendedDebugLogEntry();
    ~ExtendedDebugLogEntry();

    QString getLogString();
    UAVDataObject* uavObject() { return m_object; }
    void setObjectManager(UAVObjectManager *objectManager);

public slots:
    void setLogString(QString arg){ Q_UNUSED(arg); }

signals:
    void LogStringUpdated(QString arg);

private:
    UAVObjectManager *m_objectManager;
    UAVDataObject *m_object;
};

class FlightLogManager : public QObject {
    Q_OBJECT    
    Q_PROPERTY(DebugLogStatus *flightLogStatus READ flightLogStatus)
    Q_PROPERTY(QQmlListProperty<ExtendedDebugLogEntry> logEntries READ logEntries NOTIFY logEntriesChanged)

public:
    explicit FlightLogManager(QObject *parent = 0);
    ~FlightLogManager();

    QQmlListProperty<ExtendedDebugLogEntry> logEntries();

    DebugLogStatus* flightLogStatus() const
    {
        return m_flightLogStatus;
    }

signals:    
    void logEntriesChanged();

public slots:
    void clearAllLogs();
    void retrieveLogs(int flightToRetrieve = -1);
    void exportLogs();

private:
    UAVObjectManager *m_objectManager;
    DebugLogControl *m_flightLogControl;
    DebugLogStatus *m_flightLogStatus;
    DebugLogEntry *m_flightLogEntry;
    QList<ExtendedDebugLogEntry *> m_logEntries;

    static const int UAVTALK_TIMEOUT = 4000;

};

#endif // FLIGHTLOGMANAGER_H
