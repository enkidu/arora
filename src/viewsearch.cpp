/*
 * Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net>
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

#include "viewsearch.h"

#include <qevent.h>
#include <qshortcut.h>
#include <qtimeline.h>

#include <qdebug.h>

ViewSearch::ViewSearch(QWidget *parent)
    : QWidget(parent)
    , m_widget(0)
    , m_view(0)
    , m_timeLine(new QTimeLine(150, this))
{
    initializeSearchWidget();

    // we start off hidden
    setMaximumHeight(0);
    m_widget->setGeometry(0, -1 * m_widget->height(),
                          m_widget->width(), m_widget->height());
    hide();

    connect(m_timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(frameChanged(int)));

    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(animateHide()));
}

void ViewSearch::initializeSearchWidget()
{
    m_widget = new QWidget(this);
    m_widget->setContentsMargins(0, 0, 0, 0);
    ui.setupUi(m_widget);
    ui.previousButton->
        setText(m_widget->layoutDirection()
                == Qt::LeftToRight? QChar(9664): QChar(9654) );
    ui.nextButton->setText(m_widget->layoutDirection()
                == Qt::LeftToRight? QChar(9654): QChar(9664));
    ui.searchInfo->setText(QString());
    setMinimumWidth(m_widget->minimumWidth());
    setMaximumWidth(m_widget->maximumWidth());
    setMinimumHeight(m_widget->minimumHeight());
}

void ViewSearch::setView(QWidget *view)
{
    m_view = view;
}

QWidget *ViewSearch::getView() const
{
    return m_view;
}

void ViewSearch::clear()
{
    ui.searchLineEdit->setText(QString());
}

void ViewSearch::showFind()
{
    if (!isVisible()) {
        show();
        m_timeLine->setFrameRange(-1 * m_widget->height(), 0);
        m_timeLine->setDirection(QTimeLine::Forward);
        disconnect(m_timeLine, SIGNAL(finished()),
                   this, SLOT(hide()));
        m_timeLine->start();
    }
    ui.searchLineEdit->setFocus();
    ui.searchLineEdit->selectAll();
}

void ViewSearch::resizeEvent(QResizeEvent *event)
{
    if (event->size().width() != m_widget->width())
        m_widget->resize(event->size().width(), m_widget->height());
    QWidget::resizeEvent(event);
}

void ViewSearch::animateHide()
{
    m_timeLine->setDirection(QTimeLine::Backward);
    m_timeLine->start();
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(hide()));
}

void ViewSearch::frameChanged(int frame)
{
    if (!m_widget)
        return;
    m_widget->move(0, frame);
    int height = qMax(0, m_widget->y() + m_widget->height());
    setMinimumHeight(height);
    setMaximumHeight(height);
}