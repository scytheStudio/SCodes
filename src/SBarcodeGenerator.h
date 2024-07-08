#ifndef SBARCODEGENERATOR_H
#define SBARCODEGENERATOR_H

#include <QDir>
#include <QImage>
#include <qqml.h>
#include <QQuickItem>
#include <QObject>

#include "BitMatrix.h"
#include "ByteMatrix.h"

#include "SBarcodeFormat.h"

/*!
 * \brief The SBarcodeGenerator class allows you to configure, generate & save barcodes.
 */
class SBarcodeGenerator : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int width MEMBER m_width NOTIFY widthChanged)
    Q_PROPERTY(int height MEMBER m_height NOTIFY heightChanged)
    Q_PROPERTY(int margin MEMBER m_margin NOTIFY marginChanged)
    Q_PROPERTY(int eccLevel MEMBER m_eccLevel NOTIFY eccLevelChanged)
    Q_PROPERTY(QString fileName MEMBER m_fileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString extension MEMBER m_extension)
    Q_PROPERTY(QString filePath MEMBER m_filePath)
    Q_PROPERTY(QString inputText MEMBER m_inputText)
    Q_PROPERTY(SCodes::SBarcodeFormat format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(int centerImageRatio READ centerImageRatio WRITE setCenterImageRatio NOTIFY centerImageRatioChanged)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:

    /*!
     * \fn explicit SBarcodeGenerator(QObject *parent)
     * \brief Constructor.
     * \param QObject *parent - a pointer to the parent object.
     */
    explicit SBarcodeGenerator(QQuickItem *parent = nullptr);

    /*!
     * \fn ~SBarcodeGenerator() override
     * \brief Destructor.
     */
    ~SBarcodeGenerator() override { }

    /*!
     * \fn SCodes::SBarcodeFormat format() const
     * \brief Returns current barcode format.
     */
    SCodes::SBarcodeFormat format() const;

    /*!
     * \fn void setFormat(SCodes::SBarcodeFormat format)
     * \brief Sets the barcode format.
     * \param SCodes::SBarcodeFormat format - barcode format.
     */
    void setFormat(SCodes::SBarcodeFormat format);

    /*!
     * \fn QString imagePath() const
     * \brief Returns the center image path.
     */
    QString imagePath() const;

    /*!
     * \fn void setImagePath(const QString &imagePath)
     * \brief Sets the center image path.
     * \param const QString &imagePath - new image path.
     */
    void setImagePath(const QString &imagePath);

    /*!
     * \fn int centerImageRatio() const
     * \brief Returns the center image ratio.
     */
    int centerImageRatio() const;

    /*!
     * \fn void setCenterImageRatio(int centerImageRatio)
     * \brief Sets the center image ratio.
     * \param const int &centerImageRatio - new image ratio.
     */
    void setCenterImageRatio(int centerImageRatio);
    
    /*!
     * \fn QColor foregroundColor() const
     * \brief Returns foregroundColor.
     */
    QColor foregroundColor() const;


    /*!
     * \fn QColor backgroundColor() const
     * \brief Returns backgroundColor.
     */
    QColor backgroundColor() const;

public slots:

    /*!
     * \fn bool generate(const QString &inputString)
     * \brief Function for generating barcodes from given string
     * \param const QString &inputString - string of the barcode
     */
    bool generate(const QString &inputString);

    /*!
     * \fn void setFormat(const QString &formatName)
     * \brief Sets the barcode format.
     * \param const QString &formatName - barcode format string.
     */
    void setFormat(const QString &formatName);

    /*!
     * \fn bool saveImage()
     * \brief Saves the generated barcode image.
     */
    bool saveImage();

    /*!
     * \fn void setForegroundColor(const QColor &newForegroundColor)
     * \brief Sets the foreground.
     * \param const QColor &foreground - foreground color.
     */
    void setForegroundColor(const QColor &foregroundColor);

    /*!
     * \fn void setBackgroundColor(const QColor &backgroundColor)
     * \brief Sets the backgroundColor.
     * \param const QColor &backgroundColor - background color.
     */
    void setBackgroundColor(const QColor &backgroundColor);

signals:

    /*!
     * \brief This signal is emitted when barcode generation is finished. If any error happens, sends the error string to QML.
     * \param const QString &error - error string.
     */
    void generationFinished(const QString &error = "");

    /*!
     * \brief This signal is emitted to send width to QML.
     * \param int width - width.
     */
    void widthChanged(int width);

    /*!
     * \brief This signal is emitted to send height to QML.
     * \param int height - height.
     */
    void heightChanged(int height);

    /*!
     * \brief This signal is emitted to send margin to QML.
     * \param int margin - margin.
     */
    void marginChanged(int margin);

    /*!
     * \brief This signal is emitted to send eccLevel to QML.
     * \param int eccLevel - error correction code level.
     */
    void eccLevelChanged(int eccLevel);

    /*!
     * \brief This signal is emitted to send fileName to QML.
     * \param const QString &fileName - fileName.
     */
    void fileNameChanged(const QString &fileName);

    /*!
     * \brief This signal is emitted to send barcode format to QML.
     * \param SCodes::SBarcodeFormat format - barcode format.
     */
    void formatChanged(SCodes::SBarcodeFormat format);

    /*!
     * \brief This signal is emitted to send image path to QML.
     */
    void imagePathChanged();

    /*!
     * \brief This signal is emitted to send center image ratio to QML.
     */
    void centerImageRatioChanged();

    /*!
     * \brief This signal is emitted to send foregroundColor to QML.
     * \param const QColor &foregroundColor - foregroundColor.
     */
    void foregroundColorChanged();

    /*!
     * \brief This signal is emitted to send backgroundColor to QML.
     * \param const QColor &backgroundColor - backgroundColor.
     */
    void backgroundColorChanged();

private:
    int m_width    = 500;
    int m_height   = 500;
    int m_margin   = 10;
    int m_eccLevel = -1;

    // centerImageRatio defines the ratio by which the center image is smaller than the QR code.
    int m_centerImageRatio = 5;

    QString m_extension = "png";
    QString m_fileName  = "code";
    QString m_filePath  = "";
    QString m_inputText = "";
    QString m_imagePath = "";

    SCodes::SBarcodeFormat m_format = SCodes::SBarcodeFormat::Code128;

    /*!
     * \brief This method draws Rectangle and `imageRatio` smaller Image in the center of that Rectangle.
     * \param QImage *parentImage - Image parent. It is used for Painter constructor.
     * \param QString imagePath - Image path.
     * \param QSize imageSize - Image size.
     * \param int x - X coordinate where Image should be painted.
     * \param Qint y - Y coordinate where Image should be painted.
     */
    void drawCenterImage(QImage *parentImage, const QString &imagePath, QSize imageSize, int x, int y);

    /*!
     * \fn void setEccLvel(int eccLevel)
     * \brief Sets the QR code ecc level.
     * \param int eccLevel - QR code ecc level.
     */
    void setEccLvel(int eccLevel);

    QColor m_foregroundColor = "black";
    QColor m_backgroundColor = "white";
};

#endif // SBARCODEGENERATOR_H
