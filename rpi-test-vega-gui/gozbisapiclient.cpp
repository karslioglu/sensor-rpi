#include "gozbisapiclient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QFile>
#include <QSslKey>

GozbisApiClient::GozbisApiClient(QObject* parent)
    : QObject(parent)
{
    // QNetworkAccessManager sinyalleri ile kendi slotumuzu ilişkilendiriyoruz.
    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, &GozbisApiClient::onReplyFinished);

    //loadCertificates();
}

void GozbisApiClient::sendInsData(const QJsonObject &jsonData, bool testMode)
{
    // GÖZBİS'e anlık veri gönderme URL (INSDATA)
    // Test adresi: https://gozbisapitest.dsi.gov.tr/OlcumEkleApi/v2
    // Production: https://gozbisapi.dsi.gov.tr/OlcumEkleApi/v2
    QString baseUrl = testMode
            ? QStringLiteral("https://gozbisapitest.dsi.gov.tr/OlcumEkleApi/v2")
            : QStringLiteral("https://gozbisapi.dsi.gov.tr/OlcumEkleApi/v2");

    QUrl url(baseUrl);
    postJson(url, jsonData, QStringLiteral("sendInsData"));
}

void GozbisApiClient::setExtraDataApi(const QString &istCode,
                                      const QString &securityCode,
                                      const QString &extraData,
                                      bool testMode)
{
    // Test adresi: https://gozbisapitest.dsi.gov.tr/SetExtraDataApi
    // Production: https://gozbisapi.dsi.gov.tr/SetExtraDataApi
    QString baseUrl = testMode
            ? QStringLiteral("https://gozbisapitest.dsi.gov.tr/SetExtraDataApi")
            : QStringLiteral("https://gozbisapi.dsi.gov.tr/SetExtraDataApi");

    QUrl url(baseUrl);

    QJsonObject jsonObj{
        { "istCode",       istCode       },
        { "securityCode",  securityCode  },
        { "extraData",     extraData     }
    };

    postJson(url, jsonObj, QStringLiteral("setExtraDataApi"));
}

void GozbisApiClient::getExtraDataApi(const QString &istCode,
                                      const QString &securityCode,
                                      bool testMode)
{
    // Test adresi: https://gozbisapitest.dsi.gov.tr/GetExtraDataApi
    // Production: https://gozbisapi.dsi.gov.tr/GetExtraDataApi
    QString baseUrl = testMode
            ? QStringLiteral("https://gozbisapitest.dsi.gov.tr/GetExtraDataApi")
            : QStringLiteral("https://gozbisapi.dsi.gov.tr/GetExtraDataApi");

    QUrl url(baseUrl);

    QJsonObject jsonObj{
        { "istCode",       istCode      },
        { "securityCode",  securityCode }
    };

    postJson(url, jsonObj, QStringLiteral("getExtraDataApi"));
}

void GozbisApiClient::getLastDataApi(const QString &istCode,
                                     const QString &securityCode,
                                     const QString &channelCategory,
                                     const QString &channelSubCategory,
                                     const QString &channelParameter,
                                     bool testMode)
{
    // Test adresi: https://gozbisapitest.dsi.gov.tr/GetLastDataApi
    // Production: https://gozbisapi.dsi.gov.tr/GetLastDataApi
    QString baseUrl = testMode
            ? QStringLiteral("https://gozbisapitest.dsi.gov.tr/GetLastDataApi")
            : QStringLiteral("https://gozbisapi.dsi.gov.tr/GetLastDataApi");

    QUrl url(baseUrl);

    QJsonObject jsonObj{
        { "istCode",            istCode        },
        { "securityCode",       securityCode   },
        { "channelCategory",    channelCategory},
        { "channelSubCategory", channelSubCategory},
        { "channelParameter",   channelParameter}
    };

    postJson(url, jsonObj, QStringLiteral("getLastDataApi"));
}

void GozbisApiClient::postJson(const QUrl &url, const QJsonObject &payload, const QString &methodName)
{
    // Ortak POST isteği oluşturalım
    QNetworkRequest request(url);

    //request.setSslConfiguration(sslConfig);

    // JSON formatında gideceğini belirtiyoruz
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    // JSON dokümanı oluşturuyoruz
    QJsonDocument doc(payload);

    // POST isteğini gönderiyoruz. finished sinyali gelince onReplyFinished() tetiklenecek.
    QNetworkReply* reply = m_networkManager.post(request, doc.toJson());
    // reply->setProperty() ile methodName'i saklayarak geri geldiğinde ayırt edebiliriz
    reply->setProperty("methodName", methodName);
}

void GozbisApiClient::loadCertificates()
{
    QFile caFile(QStringLiteral("cacert.pem"));
    if (!caFile.open(QIODevice::ReadOnly)) {
        qWarning() << "CA dosyası açılamadı!";
        return;
    }

    QSslCertificate caCert(&caFile, QSsl::Pem);
    caFile.close();

    QFile clientCertFile(QStringLiteral("clientcert.pem"));
    if (!clientCertFile.open(QIODevice::ReadOnly)) {
        qWarning() << "İstemci sertifikası dosyası açılamadı!";
        return;
    }

    QSslCertificate clientCert(&clientCertFile, QSsl::Pem);
    clientCertFile.close();

    QFile keyFile(QStringLiteral("clientkey.pem"));
    if (!keyFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Özel anahtar dosyası açılamadı!";
        return;
    }

    QSslKey privateKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
    keyFile.close();

    sslConfig = QSslConfiguration::defaultConfiguration();

    QList<QSslCertificate> caList = sslConfig.caCertificates();
    caList.append(caCert);
    sslConfig.setCaCertificates(caList);

    sslConfig.setLocalCertificate(clientCert);
    sslConfig.setPrivateKey(privateKey);

    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
}

void GozbisApiClient::onReplyFinished(QNetworkReply* reply)
{
    if (!reply)
        return;

    QString methodName = reply->property("methodName").toString();

    if (reply->error() != QNetworkReply::NoError) {
        QString errString = reply->errorString();
        QByteArray data = reply->readAll();
        emit requestError(methodName, errString, data);
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    //qDebug() << "Response for" << methodName << ":" << responseData;
    emit requestFinished(methodName, responseData);

    // JSON parse vb.
    reply->deleteLater();
}
