#include "importwizard.h"

ImportWizard::ImportWizard(QWidget *parent) : QWizard(parent)
{
    introPage       = new IntroPage;
    selectPage      = new SelectPage;
    fxImportPage    = new FXImportPage;
    conclusionPage  = new ConclusionPage;

    importFF = false;

    setPage(WizardPage::Page_Intro, introPage);
    setPage(WizardPage::Page_Select, selectPage);
    setPage(WizardPage::Page_FX_Import, fxImportPage);
    setPage(WizardPage::Page_Conclusion, conclusionPage);

    connect(selectPage,     SIGNAL(importFF(bool)),
            this,           SLOT(setImportFF(bool)));

    connect(this,           SIGNAL(cookieCount(int)),
            fxImportPage,   SLOT(cookieCount(int)));
    connect(this,           SIGNAL(cookieEntryAdded()),
            fxImportPage,   SLOT(cookieEntryAdded()));
    connect(this,           SIGNAL(historyCount(int)),
            fxImportPage,   SLOT(historyCount(int)));
    connect(this,           SIGNAL(historyEntryAdded()),
            fxImportPage,   SLOT(historyEntryAdded()));

    setStartId(WizardPage::Page_Intro);
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(startImport(int)));
}
void ImportWizard::setImportFF(bool ok)
{
    importFF = ok;
}
bool ImportWizard::isFFPresent() const
{
    return ffPresent;
}
void ImportWizard::setFFPresent(bool ok)
{
    ffPresent = ok;
}
void ImportWizard::startImport(int id)
{
    if (id == WizardPage::Page_FX_Import)
        emit startFFImport();
}
bool ImportWizard::isImportFF() const
{
    return importFF;
}
void ImportWizard::setOperaPresent(bool ok)
{
    operaPresent = ok;
}
bool ImportWizard::isOperaPresent() const
{
    return operaPresent;
}
IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    topLabel = new QLabel(tr("This wizard will help you import history and cookies.<br>"
                             "<b>Please quit all your browsers.</b>"));
    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    setLayout(layout);
}

int IntroPage::nextId() const
{
    return WizardPage::Page_Select;
}

SelectPage::SelectPage(QWidget *parent/*, ImportManager *manager*/) : QWizardPage(parent)
{
    //this->manager = manager;
    setTitle(tr("Select Browsers"));
    topLabel = new QLabel(tr("Select browsers, from which you want to import your data"));
    topLabel->setWordWrap(true);
    layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    ffBox = new QCheckBox("Firefox / Iceweasel");
    setLayout(layout);
    connect(ffBox, SIGNAL(toggled(bool)), this, SIGNAL(importFF(bool)));
}
void SelectPage::setEnabled(int browser)
{
    if (browser == WizardPage::Firefox)
        layout->addWidget(ffBox);
}
int SelectPage::nextId() const
{
    if (ffBox->isChecked())
    {
        return WizardPage::Page_FX_Import;
    }
    else
        return WizardPage::Page_Conclusion;
}

FXImportPage::FXImportPage(QWidget *parent/*, ImportManager *manager*/) : QWizardPage(parent)
{
    setTitle(tr("Importing user data: Firefox"));
    topLabel = new QLabel(tr("Please wait while wizard is importing your data..."));
    topLabel->setWordWrap(true);
    layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    historyBar=new QProgressBar();
    historyBar->setValue(0);
    historyBar->setFormat(tr("%v/%m places"));
    layout->addWidget(historyBar);
    cookieBar=new QProgressBar();
    cookieBar->setValue(0);
    cookieBar->setFormat(tr("%v/%m cookies"));
    layout->addWidget(cookieBar);
    setLayout(layout);
}

int FXImportPage::nextId() const
{
    return WizardPage::Page_Conclusion;
}

void FXImportPage::historyCount(int count)
{
    historyBar->setMaximum(count);
}

void FXImportPage::historyEntryAdded()
{
    historyBar->setValue(historyBar->value()+1);
}

void FXImportPage::cookieCount(int count)
{
    cookieBar->setMaximum(count);
}

void FXImportPage::cookieEntryAdded()
{
    cookieBar->setValue(cookieBar->value()+1);
}
