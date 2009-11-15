#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QWizard>
#include <QWidget>
#include <QObject>
#include "importmanager.h"
#include <QtGui>

class ImportWizard : public QWizard
{
Q_OBJECT

public:
    enum {Page_Intro, Page_Select, Page_FX_Import, Page_Conclusion};
    ImportWizard(QWidget *parent = 0);
    bool isFfPresent();
    void setImportFf(bool);
    //bool isOperaPresent();
private:
    ImportManager *manager;
    bool ffPresent;
    bool importFf;
    //bool operaPresent;
private slots:
    void startImport(int);
};

class IntroPage : public QWizardPage
{
Q_OBJECT

public:
     IntroPage(QWidget *parent = 0);
     int nextId() const;
private:
     QLabel *topLabel;
};

class SelectPage : public QWizardPage
{
Q_OBJECT

public:
    SelectPage(QWidget *parent = 0, ImportWizard *wizard = 0);
    int nextId() const;
private:
    ImportWizard *wizard;
    QLabel *topLabel;
    QCheckBox *ffBox;
    QCheckBox *operaBox;
};

class FXImportPage : public QWizardPage
{
Q_OBJECT

public:
    FXImportPage(QWidget *parent = 0, ImportManager *manager = 0);
    int nextId() const;
private:
    ImportManager *manager;
    QLabel *topLabel;
    QProgressBar *historyBar;
    QProgressBar *cookieBar;
public slots:
    void historyCount(int);
    void historyEntryAdded();
    void cookieCount(int);
    void cookieEntryAdded();
};

class ConclusionPage : public QWizardPage
{
Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0){}
    int nextId() const {return -1;}
};

#endif // IMPORTWIZARD_H
