#include "importwizard.h"

ImportWizard::ImportWizard(QWidget *parent) : QWizard(parent)
{
    manager = new ImportManager();
    ffPresent = manager->firefoxPresent();
    setPage(Page_Intro, new IntroPage);
    setPage(Page_Select, new SelectPage(0, this));
    setPage(Page_FX_Import, new FXImportPage(0, manager));
    setPage(Page_Conclusion, new ConclusionPage);
    setStartId(Page_Intro);
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(startImport(int)));
}
void ImportWizard::setImportFf(bool ok)
{
    importFf = ok;
}
bool ImportWizard::isFfPresent()
{
    return ffPresent;
}
void ImportWizard::startImport(int id)
{
    switch (id)
    {
        case Page_FX_Import:
        manager->importFirefox();
    }
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
    return ImportWizard::Page_Select;
}

SelectPage::SelectPage(QWidget *parent, ImportWizard *wizard) : QWizardPage(parent)
{
    this->wizard = wizard;
    setTitle(tr("Select Browsers"));
    topLabel = new QLabel(tr("Select browsers, from which you want to import your data"));
    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    ffBox = new QCheckBox("Firefox / Iceweasel");
    if (wizard->isFfPresent())
    {
        layout->addWidget(ffBox);
    }
    setLayout(layout);
}

int SelectPage::nextId() const
{
    if (ffBox->isChecked())
    {
        wizard->setImportFf(true);
        return ImportWizard::Page_FX_Import;
    }
    else
        return ImportWizard::Page_Conclusion;
}

FXImportPage::FXImportPage(QWidget *parent, ImportManager *manager) : QWizardPage(parent)
{
    this->manager = manager;
    connect(manager, SIGNAL(historyCount(int)), this, SLOT(historyCount(int)));
    connect(manager, SIGNAL(historyEntryAdded()), this, SLOT(historyEntryAdded()));
    connect(manager, SIGNAL(cookieCount(int)), this, SLOT(cookieCount(int)));
    connect(manager, SIGNAL(cookieEntryAdded()), this, SLOT(cookieEntryAdded()));
    setTitle(tr("Importing user data: Firefox"));
    topLabel = new QLabel(tr("Please wait while wizard is importing your data..."));
    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    historyBar=new QProgressBar();
    historyBar->setValue(0);
    historyBar->setFormat(tr("%v/%m places"));
    layout->addWidget(historyBar);
    cookieBar=new QProgressBar();
    cookieBar->setValue(0);
    cookieBar->setFormat(tr("%v/%m places"));
    layout->addWidget(cookieBar);
    setLayout(layout);
}

int FXImportPage::nextId() const
{
    return ImportWizard::Page_Conclusion;
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
