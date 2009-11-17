#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <QObject>
#include <qdatetime.h>
#include <qdebug.h>
#include <qdir.h>
#include <qfile.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qtextstream.h>
#include <qvariant.h>
#include <qsettings.h>
#include "historymanager.h"
#include "cookiejar.h"
#include "importwizard.h"

class ImportManager : public QObject
{
Q_OBJECT

public:
    ImportManager();
    ~ImportManager(){qDebug("clean exit");}
    bool firefoxPresent();
    //bool operaPresent();
public slots:
    void importFirefox();
    //void importOpera();
private:
    HistoryManager manager;
    CookieJar jar;
    ImportWizard *wizard;
    QString firefoxDir;
    QString operaDir;
signals:
    void historyStart();
    void historyCount(int);
    void historyEntryAdded();
    void firefoxDbaseError(QString);
    void cookieStart();
    void cookieCount(int);
    void cookieEntryAdded();
};

#endif // IMPORTMANAGER_H
