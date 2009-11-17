
/*
 * Copyright 2008-2009 Benjamin K. Stuhl <bks24@cornell.edu>
 * Copyright 2009 Benjamin C. Meyer <ben@meyerhome.net>
 * Copyright 2009 Michal "Enkidu" Wisniewski <enkidu@enkidu.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "importmanager.h"

static HistoryEntry formatEntry(QByteArray url, QByteArray title, qlonglong prdate)
{
    QDateTime dateTime = QDateTime::fromTime_t(prdate / 1000000);
    dateTime.addMSecs((prdate % 1000000) / 1000);
    HistoryEntry entry(url, dateTime, title);
    return entry;
}

ImportManager::ImportManager()
{
    wizard = new ImportWizard();
    wizard->setFFPresent(firefoxPresent());
    connect(wizard, SIGNAL(startFfImport()), this, SLOT(importFirefox()));
    connect(this, SIGNAL(cookieCount(int)), wizard, SIGNAL(cookieCount(int)));
    connect(this, SIGNAL(cookieEntryAdded()), wizard, SIGNAL(cookieEntryAdded()));
    connect(this, SIGNAL(historyCount(int)), wizard, SIGNAL(historyCount(int)));
    connect(this, SIGNAL(historyEntryAdded()), wizard, SIGNAL(historyEntryAdded()));
    wizard->show();
}

bool ImportManager::firefoxPresent()
{
#ifdef Q_OS_WIN32
    QString ffdir("%APPDATA\\ff");
#endif
#ifdef Q_OS_LINUX
    QString ffdir = QDir::homePath().append( QLatin1String("/.mozilla/firefox/"));
#endif
#ifdef Q_OS_MAC
    QString ffdir = QDir::homePath().append( QLatin1String("/Library/firefox/"));
#endif

    QString tmpDir = ffdir;
    QFileInfo fi(tmpDir.append("profiles.ini"));
    if (!fi.exists())
        return false;

    QSettings FFSettings(fi.absoluteFilePath(),
                         QSettings::IniFormat);

    FFSettings.beginGroup(QLatin1String("Profile0"));

    QString profileDir = FFSettings.value(QLatin1String("Path"), QLatin1String("")).toString();
    if (profileDir.isEmpty())
        return false;
    firefoxDir = ffdir.append(profileDir).append(QDir::separator());
    qDebug(firefoxDir.toAscii());
    return true;
}

void ImportManager::importFirefox()
{
    //following block _must_ stay in own "scope", otherwise _will_not_work_
    {
        QSqlDatabase placesDatabase = QSqlDatabase::addDatabase("QSQLITE");
        placesDatabase.setDatabaseName(firefoxDir + QLatin1String("places.sqlite"));
        placesDatabase.open();
        {
            QSqlQuery historyCountQuery("SELECT count(id) FROM moz_places;");
            historyCountQuery.setForwardOnly(true);
            if (!historyCountQuery.exec())
                emit firefoxDbaseError(historyCountQuery.lastError().text());
            else
            {
                emit historyStart();
                historyCountQuery.first();
                int count = historyCountQuery.value(0).toInt();
                emit historyCount(count);
            }
        }
        {
            QSqlQuery historyQuery("SELECT moz_places.url, moz_places.title, moz_historyvisits.visit_date "
                                   "FROM moz_places, moz_historyvisits "
                                   "WHERE moz_places.id = moz_historyvisits.place_id;");
            historyQuery.setForwardOnly(true);

            if (historyQuery.exec())
            {
                QList<HistoryEntry> history = manager.history();
                while (historyQuery.next())
                {
                    QByteArray url = historyQuery.value(0).toByteArray();
                    QByteArray title = historyQuery.value(1).toByteArray();
                    qlonglong prdate = historyQuery.value(2).toLongLong();
                    HistoryEntry entry = formatEntry(url, title, prdate);
                    history.append(entry);
                    emit historyEntryAdded();
                }
                manager.setHistory(history);
            }
        }
    }
    //following block _must_ stay in own "scope", otherwise _will_not_work_
    {
        QSqlDatabase cookiesDatabase = QSqlDatabase::addDatabase("QSQLITE");
        cookiesDatabase.setDatabaseName(firefoxDir + QLatin1String("cookies.sqlite"));
        cookiesDatabase.open();
        {
            QSqlQuery cookieCountQuery("SELECT count(id) FROM moz_places;");
            cookieCountQuery.setForwardOnly(true);
            if (!cookieCountQuery.exec())
                emit firefoxDbaseError(cookieCountQuery.lastError().text());
            else
            {
                emit cookieStart();
                cookieCountQuery.first();
                int count = cookieCountQuery.value(0).toInt();
                emit cookieCount(count);
            }
        }
        {
            QSqlQuery cookieQuery("SELECT moz_cookies.name, moz_cookies.value, moz_cookies.host, moz_cookies.path, "
                                  "moz_cookies.expiry, moz_cookies.isSecure, moz_cookies.isHttpOnly "
                                  "FROM moz_cookies "
                                  "WHERE id > 0;");
            cookieQuery.setForwardOnly(true);

            if (cookieQuery.exec())
            {
                QList<QNetworkCookie> cookies = jar.cookies();
                while (cookieQuery.next())
                {
                    QNetworkCookie cookie(cookieQuery.value(0).toByteArray(),
                                          cookieQuery.value(1).toByteArray());
                    cookie.setDomain(cookieQuery.value(2).toString());
                    cookie.setPath(cookieQuery.value(3).toString());
                    cookie.setExpirationDate(QDateTime::fromTime_t(cookieQuery.value(4).toUInt()));
                    cookie.setSecure(cookieQuery.value(5).toBool());
                    cookie.setHttpOnly(cookieQuery.value(6).toBool());
                    cookies.append(cookie);
                    emit cookieEntryAdded();
                }
                jar.setCookies(cookies);
            }
        }
    }
}

