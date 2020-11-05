#include <QStandardPaths>
#include "SBarcodeGenerator.h"
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

SBarcodeGenerator::SBarcodeGenerator()
{

}

bool SBarcodeGenerator::process(const QString inputString)
{
    if (inputString.isEmpty()){
        return 0;
    }
    else {
    ZXing::MultiFormatWriter writer = ZXing::MultiFormatWriter(_format).setMargin(_margin).setEccLevel(_eccLevel);
    _bitmap = ZXing::ToMatrix<uint8_t>(writer.encode(ZXing::TextUtfEncoding::FromUtf8(inputString.toStdString()), _width, _height));

    _filePath = QDir::tempPath() + "/" + _fileName + "." + _extension;

    if (_extension == "png") {
        stbi_write_png(_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 1, _bitmap.data(), 0);
    }
    else if (_extension == "jpg" || _extension == "jpeg") {
        stbi_write_jpg(_filePath.toStdString().c_str(), _bitmap.width(), _bitmap.height(), 1, _bitmap.data(), 0);
    }

    qDebug().noquote().nospace() << _filePath;

    emit processFinished();

    return 1;
    }
}



bool SBarcodeGenerator::saveImage()
{
//    const uint8_t* data = _bitmap.data();
//    QImage image = QImage(data, _bitmap.width(), _bitmap.height(), 1, QImage::Format_Mono);

    if (_filePath.isEmpty()) {
        return 0;
    }
    else {
    QString docFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + _fileName + "." + _extension;

    qDebug().noquote().nospace() << _filePath << "\n" << docFolder;

    QFile::copy(_filePath, docFolder);

    return 1;
    }
}

void SBarcodeGenerator::barcodeFormatFromQMLString(QString format)
{
    _format = ZXing::BarcodeFormatFromString(format.toStdString());
}
