#include "webworker.h"
#include <qdebug.h>
#include <QEventLoop>

WebWorker::WebWorker(QUrl url)
{
    m_Url=url;
    m_Reply=NULL;
}

void WebWorker::error(QNetworkReply::NetworkError e)
{
    qWarning("error retrieving ..");
    QString err(" Error : \n");
    switch(e)
    {
        case 1   : err.append("\n the remote server refused the connection (the server is not accepting requests)");
        case 2   : err.append("\n the remote server closed the connection prematurely, before the entire reply was received and processed");
        case 3   : err.append("\n the remote host name was not found (invalid hostname)");
        case 4   : err.append("\n the connection to the remote server timed out");
        case 5   : err.append("\n the operation was canceled via calls to abort() or close() before it was finished.");
        case 6   : err.append("\n the SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted.");
        case 7   : err.append("\n the connection was broken due to disconnection from the network, however the system has initiated roaming to another access point. The request should be resubmitted and will be processed as soon as the connection is re-established.");
        case 8   : err.append("\n the connection was broken due to disconnection from the network or failure to start the network.");
        case 9   : err.append("\n the background request is not currently allowed due to platform policy.");
        case 10  : err.append("\n  while following redirects, the maximum limit was reached. The limit is by default set to 50 or as set by QNetworkRequest::setMaxRedirectsAllowed(). (This value was introduced in 5.6.)");
        case 11  : err.append("\n  while following redirects, the network access API detected a redirect from a encrypted protocol (https) to an unencrypted one (http). (This value was introduced in 5.6.)");
        case 101 : err.append("\n   the connection to the proxy server was refused (the proxy server is not accepting requests)");
        case 102 : err.append("\n   the proxy server closed the connection prematurely, before the entire reply was received and processed");
        case 103 : err.append("\n   the proxy host name was not found (invalid proxy hostname)");
        case 104 : err.append("\n   the connection to the proxy timed out or the proxy did not reply in time to the request sent");
        case 105 : err.append("\n   the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)");
        case 201 : err.append("\n   the access to the remote content was denied (similar to HTTP error 401)");
        case 202 : err.append("\n   the operation requested on the remote content is not permitted");
        case 203 : err.append("\n   the remote content was not found at the server (similar to HTTP error 404)");
        case 204 : err.append("\n   the remote server requires authentication to serve the content but the credentials provided were not accepted (if any)");
        case 205 : err.append("\n   the request needed to be sent again, but this failed for example because the upload data could not be read a second time.");
        case 206 : err.append("\n   the request could not be completed due to a conflict with the current state of the resource.");
        case 207 : err.append("\n   the requested resource is no longer available at the server.");
        case 401 : err.append("\n   the server encountered an unexpected condition which prevented it from fulfilling the request.");
        case 402 : err.append("\n   the server does not support the functionality required to fulfill the request.");
        case 403 : err.append("\n   the server is unable to handle the request at this time.");
        case 301 : err.append("\n   the Network Access API cannot honor the request because the protocol is not known");
        case 302 : err.append("\n   the requested operation is invalid for this protocol");
        case 99  : err.append("\n  an unknown network-related error was detected");
        case 199 : err.append("\n   an unknown proxy-related error was detected");
        case 299 : err.append("\n   an unknown error related to the remote content was detected");
        case 399 : err.append("\n   a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)");
        case 499 : err.append("\n   an unknown error related to the server response was detected");
            default: err.append("\n   an unknown error ");
    }
    err.append(e);
    emit WorkCompleted(err);
     m_Reply->disconnect(this);
     //m_Reply->deleteLater();
     //m_Reply = 0;
}

int WebWorker::Fetch()
{
    QNetworkRequest request(m_Url);
    m_Reply = m_Manager.get(request);
    connect(m_Reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_Reply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    connect(m_Reply, SIGNAL(readChannelFinished()), this, SLOT(readFinish()));    
    connect(m_Reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));\
    m_Reply->waitForReadyRead(60000);
}

void WebWorker::readyRead()
{
    int statusCode = m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300)
    {
        m_data.append(m_Reply->readAll());
    }
}

void WebWorker::readFinish()
{
    emit WorkCompleted(m_data);
}

void WebWorker::metaDataChanged()
{
    QUrl redirectionTarget = m_Reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {
        m_Url=redirectionTarget;
        Fetch();
    }
}

void WebWorker::run()
{
    Fetch();
    QEventLoop m_Looper;
     connect(m_Reply, SIGNAL(finished()), &m_Looper, SLOT(quit()));
    m_Looper.processEvents();
    m_Looper.exec();
}
