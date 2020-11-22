#ifndef SBARCODEGENERATOR_H
#define SBARCODEGENERATOR_H

#include <QQuickItem>
#include <QObject>
#include <QImage>
#include <QDir>
#include <qqml.h>

#include "BarcodeFormat.h"
#include "BitMatrix.h"
#include "ByteMatrix.h"

class SBarcodeGenerator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int width MEMBER _width NOTIFY widthChanged)
    Q_PROPERTY(int height MEMBER _height NOTIFY heightChanged)
    Q_PROPERTY(int margin MEMBER _margin NOTIFY marginChanged)
    Q_PROPERTY(int eccLevel MEMBER _eccLevel NOTIFY eccLevelChanged)
    Q_PROPERTY(QString fileName MEMBER _fileName NOTIFY fileNameChanged)
    Q_PROPERTY(ZXing::BarcodeFormat format MEMBER _format)
    Q_PROPERTY(QString extension MEMBER _extension)
    Q_PROPERTY(QString filePath MEMBER _filePath)
    Q_PROPERTY(QString inputText MEMBER _inputText)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
        QML_ELEMENT
#endif

public:
    SBarcodeGenerator();

public slots:
    bool process(const QString &inputString);
    bool saveImage();
    void barcodeFormatFromQMLString(const QString &format);

signals:
    void processFinished();

    void widthChanged(int width);

    void heightChanged(int height);

    void marginChanged(int margin);

    void eccLevelChanged(int eccLevel);

    void fileNameChanged(QString fileName);

private:
    int _width = 500;
    int _height = 500;
    int _margin = 10;
    int _eccLevel = -1;
    ZXing::BarcodeFormat _format = ZXing::BarcodeFormat::QR_CODE;
    QString _extension = "png";
    QString _fileName = "code";
    QString _filePath = "";
    QString _inputText = "";
    ZXing::Matrix<uint8_t> _bitmap = ZXing::Matrix<uint8_t>();
};

#endif // SBARCODEGENERATOR_H
