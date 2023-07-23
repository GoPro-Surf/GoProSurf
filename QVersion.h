//
// Created by svistunov on 23.07.23.
//

#ifndef GOPROSURF_QVERSION_H
#define GOPROSURF_QVERSION_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QMessageBox>

#define VERSION "0.1.0"

#if defined(_WIN32)
#define OS "windows"
#elif defined(__APPLE__)
#define OS "macos"
#elif defined(__linux__)
#define OS "linux"
#elif defined(__FreeBSD__)
#define OS "freebsd"
#else
#define OS "other"
#endif

void checkNewVersion(QWidget *parent) {
    auto nm = new QNetworkAccessManager();
    QUrlQuery query;
    query.addQueryItem("ver", VERSION);
    query.addQueryItem("os", OS);

    QUrl url(QString("https://checkver.goprosurf.qsoa.cloud/"));
    url.setQuery(query);

    QNetworkRequest request(url);
    auto reply = nm->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [nm, reply, parent]() {
        if (reply->error() == QNetworkReply::NoError &&
            reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QMessageBox::information(parent, "GoPro Surf",
                                     "We have a new version. Check https://github.com/GoPro-Surf");
        }

        reply->deleteLater();
        nm->deleteLater();
    });
}


#endif //GOPROSURF_QVERSION_H
