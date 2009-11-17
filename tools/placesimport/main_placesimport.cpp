/*
 * Copyright 2008-2009 Benjamin K. Stuhl <bks24@cornell.edu>
 * Copyright 2009 Benjamin C. Meyer <ben@meyerhome.net>
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


#include "singleapplication.h"
#include "importmanager.h"
#include <QMessageBox>

int main(int argc, char **argv)
{
    SingleApplication application(argc, argv);
    QCoreApplication::setOrganizationDomain(QLatin1String("arora-browser.org"));
    QCoreApplication::setApplicationName(QLatin1String("Arora"));

    if (application.sendMessage(QByteArray())) {
        QMessageBox::warning(0, QObject::tr("Arora running"), QObject::tr("Please exit Arora before running this wizard"));
        return 1;
    }
    ImportManager manager;
   /* QFile file(QDir::homePath().append(QLatin1String("/.opera/global.dat")));
    if ( file.open(QIODevice::ReadOnly) )
    {
        HistoryManager manager;
        QList<HistoryEntry> history = manager.history();
        QTextStream t( &file );
        while ( !t.atEnd() )
        {
            QString title = t.readLine();
            QString url = t.readLine();
            quint64 time = t.readLine().toUInt();
            t.readLine(); //unknown parameter
            HistoryEntry entry = formatEntry(url.toUtf8(), title.toUtf8(), time * 1000000);
            history.append(entry);
        }
        manager.setHistory(history);
    file.close();
    }
    else
        qDebug("no Opera history file");

*/
    return application.exec();
}

