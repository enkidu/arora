#ifndef BSTATUSBAR_H
#define BSTATUSBAR_H

#include <QStatusBar>
#include <QLabel>
#include <QTimer>
#include <QObject>

class StatusBar : public QStatusBar
{

Q_OBJECT

public:
    StatusBar(QWidget *parent = 0);
    virtual ~StatusBar(){}
private:
    QWidget *m_mainFrame;
    QLabel *m_messageLabel;
    QTimer *m_messageTimer;
public slots:
    void sb_showMessage(const QString&, int timeout = 0);
    void sb_clearMessage();
};

#endif // BSTATUSBAR_H
