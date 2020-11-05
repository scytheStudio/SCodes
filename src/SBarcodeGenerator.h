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
    Q_PROPERTY(int width MEMBER _width)
    Q_PROPERTY(int height MEMBER _height)
    Q_PROPERTY(int margin MEMBER _margin)
    Q_PROPERTY(int eccLevel MEMBER _eccLevel)
    Q_PROPERTY(ZXing::BarcodeFormat format MEMBER _format)
    Q_PROPERTY(QString extension MEMBER _extension)
    Q_PROPERTY(QString fileName MEMBER _fileName)
    Q_PROPERTY(QString filePath MEMBER _filePath)
    Q_PROPERTY(QString inputText MEMBER _inputText)
    QML_ELEMENT
public:
    SBarcodeGenerator();

public slots:
    bool process(const QString inputString);
    bool saveImage();
    void barcodeFormatFromQMLString(QString format);

signals:
    void processFinished();

private:
    int _width = 100;
    int _height = 100;
    int _margin = 10;
    int _eccLevel = -1;
    ZXing::BarcodeFormat _format = ZXing::BarcodeFormat::QR_CODE;
    QString _extension = "png";
    QString _fileName = "code";
    QString _filePath;
    QString _inputText;
    ZXing::Matrix<uint8_t> _bitmap;

};

#endif // SBARCODEGENERATOR_H
