#ifndef WEBWORKER_H
#define WEBWORKER_H

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qxmlstream.h>
#include <qrunnable.h>
#include <qeventloop.h>

class WebWorker :  public QObject, public QRunnable
{
    Q_OBJECT
public:
    WebWorker(QUrl);
    QString status;

public slots:
    void error(QNetworkReply::NetworkError);
   // void finished(QNetworkReply *reply);
    int Fetch();
    void readyRead();
    void readFinish();
    void metaDataChanged();

private:
    QNetworkAccessManager m_Manager;
    QNetworkReply *m_Reply;

    QEventLoop m_Looper;
    QByteArray m_data;
    QUrl m_Url;

    // QRunnable interface
public:
    void run();

signals:
    void WorkCompleted(QString s);
};

#endif // WEBWORKER_H
