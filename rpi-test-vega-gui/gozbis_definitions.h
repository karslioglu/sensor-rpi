#ifndef GOZBIS_DEFINITIONS_H
#define GOZBIS_DEFINITIONS_H

typedef enum {
    ChannelCategory_Akarsu = 1,             //Nehir, Dere
    ChannelCategory_Gol,                    //Gol, Golet
    ChannelCategory_Baraj,                  //
    ChannelCategory_Kuyu,                   //YAS Kuyusu, Yuzey Suyu Gozlemi
    ChannelCategory_SulamaKanali,           //
    ChannelCategory_MeteorolojiIstasyonu,   //Cok parametreli meteoroloji istasyonu, kar gozlem vs
    ChannelCategory_SuKalitesi
}ChannelCategory;

typedef enum {
    ChannelSubCategory_NoSubCategory = 1,
    ChannelSubCategory_AnaCebriBoru,
    ChannelSubCategory_TarimsalSulama,
    ChannelSubCategory_SagSahilSulama,
    ChannelSubCategory_SolSahilSulama,
    ChannelSubCategory_DereyeDesarj,
    ChannelSubCategory_IcmeSuyu,
    ChannelSubCategory_TermikSantral
}ChannelSubCategory;

typedef enum {
    ChannelParameter_Unknown = 1,    //
    ChannelParameter_AH,
    ChannelParameter_AP,
    ChannelParameter_AT,
    ChannelParameter_DR,
    ChannelParameter_EC,
    ChannelParameter_ETo,
    ChannelParameter_EVO,
    ChannelParameter_GR,
    ChannelParameter_LW,
    ChannelParameter_PH,
    ChannelParameter_PR,
    ChannelParameter_SD,
    ChannelParameter_SM,
    ChannelParameter_SWD,
    ChannelParameter_WAA,
    ChannelParameter_WAF,
    ChannelParameter_WAL,
    ChannelParameter_WAS,
    ChannelParameter_WAT,
    ChannelParameter_WAV,
    ChannelParameter_WID,
    ChannelParameter_WIS,
    ChannelParameter_WFP,
    ChannelParameter_SV,
    ChannelParameter_NH4,
    ChannelParameter_NO3,
    ChannelParameter_DO,
    ChannelParameter_COD,
    ChannelParameter_TOC,
    ChannelParameter_SS,
    ChannelParameter_NTU,
    ChannelParameter_PTCO,
    ChannelParameter_CHL,
    ChannelParameter_OIL
}ChannelParameter;

typedef enum {
    ValueType_Unknown = 1,          // 1 - Bilinmeyen kayıt türü
    ValueType_AVG,                  // 2 - Ortalama. Tarih bilgisi kaydedilir. Saat bilgisi sabit 23:59 kaydedilir.
    ValueType_INS,                  // 3 - Anlık. Ölçüm yapılan tarih ve saat kaydedilir.
    ValueType_MAX,                  // 4 - Maksimum. Günün en yüksek değeri. Kaydın oluştuğu tam tarih ve saat bilgisiyle kaydedilir.
    ValueType_MIN,                  // 5 - Minimum. Günün en düşük değeri. Kaydın oluştuğu tam tarih ve saat bilgisiyle kaydedilir.
    ValueType_TOT24H,               // 6 - 24 saatlik toplam. Tarih bilgisi kaydedilir. Saat bilgisi sabit 23:59 kaydedilir.
    ValueType_TOT1H,                // 7 - 1 saatlik toplam. Eğer, kayıt aralığı 60 dakikadan küçükse 1 saatlik toplamları da bulmak için kullanılır.
    ValueType_ENDEX                 // 8 - Kullanılmıyor! Daha sonra lazım olabilir.
}ValueType;

typedef enum {
    MeasurementUnit_NoUnit = 1,             //  1
    MeasurementUnit_CubicMeter,             //  2
    MeasurementUnit_CubicMeterPerHour,      //  3
    MeasurementUnit_Centimeter,             //  4
    MeasurementUnit_Meter,                  //  5
    MeasurementUnit_CubicHectometer,        //  6
    MeasurementUnit_Milimeter,              //  7
    MeasurementUnit_Bar,                    //  8
    MeasurementUnit_Microsiemens,           //  9
    MeasurementUnit_DegreeCelsius,          // 10
    MeasurementUnit_MilligramPerLiter,      // 11
    MeasurementUnit_PH,                     // 12
    MeasurementUnit_NTU,                    // 13
    MeasurementUnit_InputOutput,            // 14
    MeasurementUnit_Percent,                // 15
    MeasurementUnit_LiterPerHour,           // 16
    MeasurementUnit_LiterPerSecond,         // 17
    MeasurementUnit_LiterPerMinute,         // 18
    MeasurementUnit_CubicMeterPerSecond,    // 19
    MeasurementUnit_MeterPerSecond          // 20
}MeasurementUnit;
#endif // GOZBIS_DEFINITIONS_H
