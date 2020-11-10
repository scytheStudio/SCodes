#include <QStandardPaths>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include "SBarcodeGenerator.h"
#include "MultiFormatWriter.h"
#include "TextUtfEncoding.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

SBarcodeGenerator::SBarcodeGenerator()
{

}

bool SBarcodeGenerator::process(const QString &inputString)
{
    if (inputString.isEmpty()){
        return false;
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

    emit processFinished();

    return true;
    }
}



bool SBarcodeGenerator::saveImage()
{
    if (_filePath.isEmpty()) {
        return false;
    }

#ifdef Q_OS_ANDROID
    if (QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")) == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.WRITE_EXTERNAL_STORAGE"}));
        if (resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }
#endif

    QString docFolder = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + _fileName + "." + _extension;

    QFile::copy(_filePath, docFolder);

    return true;
}

void SBarcodeGenerator::barcodeFormatFromQMLString(const QString &format)
{
    _format = ZXing::BarcodeFormatFromString(format.toStdString());
}
