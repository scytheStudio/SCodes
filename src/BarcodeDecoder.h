#ifndef QR_DECODER_H
#define QR_DECODER_H

#include <QObject>
#include <QVideoFrame>

class BarcodeDecoder : public QObject
{
    Q_OBJECT

public:
    explicit BarcodeDecoder(QObject *parent = nullptr);

    void clean();

    bool isDecoding() const;
    QString captured() const;

    static QImage videoFrameToImage(QVideoFrame &videoFrame, const QRect &captureRect);
    static QImage imageFromVideoFrame(const QVideoFrame &videoFrame);

#if QT_VERSION >= 0x050150
    static void QT_FASTCALL qt_convert_YUYV_to_ARGB32(const QVideoFrame &frame, uchar *output);
#endif



public slots:
    void process(const QImage capturedImage);

signals:
    void isDecodingChanged(bool isDecoding);
    void capturedChanged(const QString &captured);

private:
    bool _isDecoding = false;
    QString _captured = "";

    void setCaptured(const QString &captured);
    void setIsDecoding(bool isDecoding);
};

#endif // QR_DECODER_H
