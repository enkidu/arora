#include "statusbar.h"
#include "qboxlayout.h"
#include "qprogressbar.h"
#include "qscrollarea.h"
#include <QAction>
#include <QScrollBar>
#include <QToolBar>

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{
    qDebug("created");
    m_messageTimer= new QTimer(this);
    m_messageTimer->setSingleShot(true);
    connect(m_messageTimer, SIGNAL(timeout()),
            this,           SLOT(clearMessage()));

    m_mainFrame = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout();
    m_messageLabel = new QLabel(QLatin1String("here should be fancy text"), m_mainFrame);
    QHBoxLayout *manager_layout = new QHBoxLayout();
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    QWidget *scrolled = new QWidget();
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    scroll->setFixedHeight(16);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0,0,0,0);
    for(int i = 0; i < 20; i++)
    {
        QProgressBar *pbar = new QProgressBar();
        pbar->setValue(25);
        pbar->setMaximum(100);
        pbar->setMaximumHeight(14);
        pbar->setFixedWidth(150);
        pbar->setFormat(QLatin1String("25 of 100 MB"));
        m_messageLabel->setTextFormat(Qt::PlainText);
        hlayout->addWidget(pbar);
    }
    vlayout->addWidget(m_messageLabel);
    scrolled->setLayout(hlayout);
    scroll->setWidget(scrolled);
    QAction *actionClose = new QAction(QIcon(QLatin1String(":/16x16/dialog-close.png")), QLatin1String(""), this);
    QAction *actionClean = new QAction(QIcon(QLatin1String(":/16x16/edit-clear.png")), QLatin1String(""), this);
    QToolBar *cont = new QToolBar();
    cont->addAction(actionClose);
    cont->addAction(actionClean);
    manager_layout->addWidget(cont);
    manager_layout->addWidget(scroll, 1);
    QScrollBar *scrollbar = new QScrollBar(Qt::Horizontal);
    scrollbar->setFixedSize(100,14);
    scroll->setHorizontalScrollBar(scrollbar);
    manager_layout->addWidget(scrollbar);
    vlayout->addLayout(manager_layout, 1);
    m_mainFrame->setLayout(vlayout);
    this->addPermanentWidget(m_mainFrame,1);
}

void StatusBar::sb_showMessage(const QString &message, int timeout)
{
    m_messageLabel->setText(message);
    qDebug("called 1");
    if (timeout != 0)
    {
        m_messageTimer->stop();
        m_messageTimer->setInterval(timeout);
        m_messageTimer->start();
    }
}

void StatusBar::sb_clearMessage()
{
    m_messageLabel->clear();
}
