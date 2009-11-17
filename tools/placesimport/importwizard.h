#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QWizard>
#include <QWidget>
#include <QObject>
#include <QtGui>

class WizardPage
{

public:
    enum {Page_Intro, Page_Select, Page_FX_Import, Page_Conclusion};
    enum {Firefox, Opera};
    WizardPage(){}
    ~WizardPage(){}
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
    SelectPage(QWidget *parent = 0);
    int nextId() const;
    void setEnabled(int);
private:
    QVBoxLayout *layout;
    QLabel *topLabel;
    QCheckBox *ffBox;
    QCheckBox *operaBox;
signals:
    void importFF(bool);
};

class FXImportPage : public QWizardPage
{
Q_OBJECT

public:
    FXImportPage(QWidget *parent = 0/*, ImportManager *manager = 0*/);
    int nextId() const;
private:
    /*ImportManager *manager;*/
    QLabel *topLabel;
    QProgressBar *historyBar;
    QProgressBar *cookieBar;
    QVBoxLayout *layout;
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
    ConclusionPage(QWidget *parent = 0) : QWizardPage(parent){}
};

class ImportWizard : public QWizard
{
Q_OBJECT
public:
    ImportWizard(QWidget *parent = 0);
    ~ImportWizard(){qDebug("wizard deleted");}
    void setFFPresent(bool);

    void setOperaPresent(bool);
    bool isFFPresent() const;
    bool isOperaPresent() const;
    bool isImportFF() const;
    //bool isOperaPresent();
signals:
    void startFFImport();
    void historyStart();
    void historyCount(int);
    void historyEntryAdded();
    void firefoxDbaseError(QString);
    void cookieStart();
    void cookieCount(int);
    void cookieEntryAdded();
private:
    IntroPage *introPage;
    SelectPage *selectPage;
    FXImportPage *fxImportPage;
    ConclusionPage *conclusionPage;
    bool ffPresent;
    bool importFF;
    bool operaPresent;
private slots:
    void startImport(int);
    void setImportFF(bool);
};

#endif // IMPORTWIZARD_H
