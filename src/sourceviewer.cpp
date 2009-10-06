/*
 * Copyright 2008 Christian Franke <cfchris6@ts2server.com>
 * Copyright 2008-2009 Benjamin C. Meyer <ben@meyerhome.net>
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

#include "sourceviewer.h"

//#include <qlayout.h>
//#include <qmenubar.h>
#include <qnetworkcookie.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
//#include <qplaintextedit.h>
#include <qshortcut.h>
//#include <qsettings.h>
#include <qwebframe.h>
#include <qwebpage.h>
#include <qtemporaryfile.h>
#include <qdesktopservices.h>

#include "browserapplication.h"
#include "networkaccessmanager.h"
//#include "plaintexteditsearch.h"
//#include "sourcehighlighter.h"

SourceViewer::SourceViewer(const QString &source, const QString &title,
                           const QUrl &url, QWidget *parent)
    : QObject(parent)
    , m_source(source)
{

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    m_reply = BrowserApplication::networkAccessManager()->get(request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(loadingFinished()));
    m_reply->setParent(this);
}

SourceViewer::~SourceViewer()
{
}

void SourceViewer::loadingFinished()
{
    QWebPage page;
    QByteArray response = m_reply->readAll();
    page.mainFrame()->setContent(response, QString(), m_reply->request().url());
    QTemporaryFile file(QLatin1String("page"));
    file.setAutoRemove(false);
    file.open();
    file.rename(file.fileName().append(QLatin1String(".arora.html.txt")));
    file.open();

    /* If original request was POST or a different problem is there, fall
       back to modified version of QWebFrame.toHtml() */
    if (page.mainFrame()->toHtml() != m_source)
    {
        file.write(m_source.toUtf8());
    }
    else
    {
        file.write(response);
    }
    file.close();
    QDesktopServices::openUrl(QUrl(file.fileName()));
    this->deleteLater();
}
