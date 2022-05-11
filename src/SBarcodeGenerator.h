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
    ~SBarcodeGenerator() override { };

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

private:
    int m_width    = 500;
    int m_height   = 500;
    int m_margin   = 10;
    int m_eccLevel = -1;

    QString m_extension = "png";
    QString m_fileName  = "code";
    QString m_filePath  = "";
    QString m_inputText = "";

    SCodes::SBarcodeFormat m_format = SCodes::SBarcodeFormat::Code128;

    ZXing::Matrix<uint8_t> _bitmap = ZXing::Matrix<uint8_t>();
};

#endif // SBARCODEGENERATOR_H
