#ifndef GOZBISAPICLIENT_H
#define GOZBISAPICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class GozbisApiClient : public QObject
{
    Q_OBJECT
public:
    explicit GozbisApiClient(QObject* parent = nullptr);

    /**
     * @brief GÖZBİS'e anlık ölçüm verisi (INSDATA) göndermek için kullanılan fonksiyon.
     * @param jsonData Gönderilecek JSON payload içeren QJsonObject.
     * @param testMode true ise test sunucuya, false ise production sunucuya gönderir.
     */
    void sendInsData(const QJsonObject &jsonData, bool testMode = false);

    /**
     * @brief GÖZBİS veritabanındaki extraData alanına veri yazmak için SetExtraDataApi'yi çağırır.
     * @param istCode İstasyon kodu
     * @param securityCode Güvenlik kodu
     * @param extraData Yazılacak veri
     * @param testMode true ise test sunucuya, false ise production sunucuya gönderir.
     */
    void setExtraDataApi(const QString &istCode,
                         const QString &securityCode,
                         const QString &extraData,
                         bool testMode = true);

    /**
     * @brief GÖZBİS veritabanındaki extraData alanından veri okumak için GetExtraDataApi'yi çağırır.
     * @param istCode İstasyon kodu
     * @param securityCode Güvenlik kodu
     * @param testMode true ise test sunucuya, false ise production sunucuya gönderir.
     */
    void getExtraDataApi(const QString &istCode,
                         const QString &securityCode,
                         bool testMode = true);

    /**
     * @brief GÖZBİS veritabanındaki son ölçüm verisini almak için GetLastDataApi'yi çağırır.
     * @param istCode İstasyon kodu
     * @param securityCode Güvenlik kodu
     * @param channelCategory
     * @param channelSubCategory
     * @param channelParameter
     * @param testMode true ise test sunucuya, false ise production sunucuya gönderir.
     */
    void getLastDataApi(const QString &istCode,
                        const QString &securityCode,
                        const QString &channelCategory,
                        const QString &channelSubCategory,
                        const QString &channelParameter,
                        bool testMode = true);

signals:
    /**
     * @brief İstek tamamlandığında sinyal gönderilir.
     * @param methodName Hangi fonksiyonun sonucu olduğu bilgisi (örn. "sendInsData")
     * @param responseBody Sunucudan alınan yanıtın JSON metni
     */
    void requestFinished(const QString &methodName, const QByteArray &responseBody);

    /**
     * @brief İstek başarısız olduğunda veya hata oluştuğunda sinyal gönderilir.
     * @param methodName Hangi fonksiyon sırasında hata oluştuğu
     * @param errorString Hata açıklaması
     */
    void requestError(const QString &methodName, const QString &errorString, const QByteArray &data);

private slots:
    /**
     * @brief İstek tamamlandığında (QNetworkReply::finished) çağrılır.
     */
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_networkManager;
    QSslConfiguration sslConfig;

    /**
     * @brief Ortak POST fonksiyonu. Verilen url ve payload ile POST yapar, cevapları slot ile işler.
     * @param url Çağrılacak tam URL
     * @param payload Gönderilecek JSON
     * @param methodName Tanımlayıcı isim (örneğin "sendInsData"), sinyallerde ayırt etmek için kullanıyoruz.
     */
    void postJson(const QUrl &url, const QJsonObject &payload, const QString &methodName);

    /**
     * @brief Sertifika dosyalarini yukleyen fonksiyon.
     */
    void loadCertificates();
};

#endif // GOZBISAPICLIENT_H
