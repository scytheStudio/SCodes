import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Layouts 1.12
import com.scythestudio.scodes 1.0


/*!
  Barcode generator main page. All QML elements managing from here.
  */
ApplicationWindow {
  id: appWindow

  visible: true

  width: 400
  height: 800

  MouseArea {
    id: hideKeyboard
    anchors.fill: parent
    onClicked: {
      Qt.inputMethod.hide()
    }
  }

  ColorController {
    id: colorController
  }

  SBarcodeGenerator {
    id: barcodeGenerator

    onForegroundColorChanged: {
      image.source = ""
      barcodeGenerator.generate(textField.text)
    }

    onBackgroundColorChanged: {
      image.source = ""
      barcodeGenerator.generate(textField.text)
    }

    onGenerationFinished: function (error) {
      if (error === "") {
        console.log(barcodeGenerator.filePath)
        image.source = "file:///" + barcodeGenerator.filePath
      } else {
        generateLabel.text = error
        generatePopup.open()
      }
    }
  }

  Rectangle {
    id: dashboard

    anchors.fill: parent

    height: parent.height
    width: parent.width

    Rectangle {
      id: inputRect
      z: 100

      height: 40

      anchors {
        top: parent.top
        left: parent.left
        right: parent.right
      }

      CTextField {
        id: textField

        anchors.fill: parent

        selectByMouse: true

        placeholderText: qsTr("Input")
      }
    }

    Image {
      id: image

      width: parent.width
      height: image.width

      anchors {
        left: parent.left
        right: parent.right
        verticalCenter: parent.verticalCenter
      }

      cache: false
    }

    Rectangle {
      id: buttonsRect

      height: 40

      anchors {
        bottom: parent.bottom
        left: parent.left
        right: parent.right
      }

      RowLayout {
        id: buttonsLayout

        spacing: 5

        anchors.fill: parent

        CButton {
          id: settingsButton

          Layout.alignment: Qt.AlignHCenter
          Layout.bottomMargin: 10

          text: qsTr("Settings")

          onClicked: {
            settingsPopup.open()
          }
        }

        CButton {
          id: generateButton

          Layout.alignment: Qt.AlignHCenter
          Layout.bottomMargin: 10
          checkable: false

          text: qsTr("Generate")

          onClicked: {
            image.source = ""
            if (textField.text === "") {
              generateLabel.text = "Input is empty"
              generatePopup.open()
            } else {
              barcodeGenerator.generate(textField.text)
            }
          }
        }

        CButton {
          id: saveButton

          Layout.alignment: Qt.AlignHCenter
          Layout.bottomMargin: 10

          text: qsTr("Save")

          onClicked: {
            if (barcodeGenerator.saveImage()) {
              saveLabel.text = "File successfully saved"
            } else {
              saveLabel.text = "There was an error while saving file"
            }

            imageSavedPopup.open()
          }
        }
      }
    }

    Popup {
      id: generatePopup

      anchors.centerIn: parent

      dim: true

      modal: true

      Label {
        id: generateLabel

        anchors.centerIn: parent
      }

      onClosed: {
        generateButton.checked = false
      }
    }

    Popup {
      id: imageSavedPopup

      anchors.centerIn: parent

      dim: true

      modal: true

      Label {
        id: saveLabel

        anchors.centerIn: parent
      }

      onClosed: {
        saveButton.checked = false
      }
    }

    Popup {
      id: settingsPopup

      width: parent.width * 0.6
      height: parent.height * 0.6

      anchors.centerIn: parent

      dim: true

      modal: true

      ColumnLayout {
        anchors.fill: parent

        spacing: 5

        CTextField {
          id: widthField

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current width: " + barcodeGenerator.width

          onEditingFinished: function () {

            var parsedWidth = parseInt(text)

            if (isNaN(parsedWidth) != true && parsedWidth > 0) {
              barcodeGenerator.width = parsedWidth
            }
          }
        }

        CTextField {
          id: heightField

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current height: " + barcodeGenerator.height

          onEditingFinished: function () {

            var parsedHeight = parseInt(text)

            if (isNaN(parsedHeight) != true && parsedHeight > 0) {
              barcodeGenerator.height = parsedHeight
            }
          }
        }

        CTextField {
          id: marginField

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current margin: " + barcodeGenerator.margin

          onEditingFinished: function () {

            var parsedMargin = parseInt(text)

            if (isNaN(parsedMargin) != true) {
              barcodeGenerator.margin = parsedMargin
            }
          }
        }

        CTextField {
          id: eccLevelField

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current ECC Level: " + barcodeGenerator.eccLevel

          onEditingFinished: function () {
            barcodeGenerator.eccLevel = text
          }
        }

        CTextField {
          id: foregroundColorField

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current foreground color: " + barcodeGenerator.foregroundColor

          onTextChanged: function () {
            foregroundColorField.inputIsValid = colorController.checkColor(foregroundColorField.text)

            if (colorController.checkColor(foregroundColorField.text)) {
              barcodeGenerator.setForegroundColor(colorController.convertStringToColor(foregroundColorField.text))
            }
          }
        }

        CTextField {
          id: backgroundColorField

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current background color: " + barcodeGenerator.backgroundColor

          onTextChanged: function () {
            backgroundColorField.inputIsValid = colorController.checkColor(backgroundColorField.text)

            if (colorController.checkColor(backgroundColorField.text)) {
              barcodeGenerator.setBackgroundColor(colorController.convertStringToColor(backgroundColorField.text))
            }
          }
        }

        CComboBox {
          id: formatDropDown

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          model: ListModel {
            id: formats

            ListElement {
              text: "Aztec"
            }
            ListElement {
              text: "Codabar"
            }
            ListElement {
              text: "Code39"
            }
            ListElement {
              text: "Code93"
            }
            ListElement {
              text: "Code128"
            }
            ListElement {
              text: "DataMatrix"
            }
            ListElement {
              text: "EAN-8"
            }
            ListElement {
              text: "EAN-13"
            }
            ListElement {
              text: "ITF"
            }
            ListElement {
              text: "PDF417"
            }
            ListElement {
              text: "QRCode"
            }
            ListElement {
              text: "UPC-A"
            }
            ListElement {
              text: "UPC-E"
            }
          }
          onCurrentIndexChanged: function () {
            var formatAsText = formats.get(currentIndex).text
            // a separate method was used because of qml error
            // when try to use it as overloaded setter
            barcodeGenerator.setFormat(formatAsText)
          }
        }

        CComboBox {
          id: imageFormat

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          model: ListModel {
            id: extensions

            ListElement {
              text: "png"
            }

            ListElement {
              text: "jpg"
            }
          }
          onCurrentIndexChanged: function () {
            barcodeGenerator.extension = extensions.get(currentIndex).text
          }
        }

        CTextField {
          id: fileNameField

          text: qsTr(barcodeGenerator.fileName)

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          onEditingFinished: {
            barcodeGenerator.fileName = text
          }
        }

        CTextField {
          id: imagePathField

          text: qsTr(barcodeGenerator.imagePath)

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current image path: " + barcodeGenerator.imagePath

          onEditingFinished: {
            barcodeGenerator.imagePath = text
          }
        }

        CTextField {
          id: centerImageRatioField

          text: qsTr(barcodeGenerator.centerImageRatio.toString())

          implicitWidth: parent.width
          implicitHeight: parent.height / 10

          placeholderText: "Current center image ratio: " + barcodeGenerator.centerImageRatio

          onEditingFinished: {
            barcodeGenerator.centerImageRatio = parseInt(text)
          }
        }
      }

      onClosed: {
        settingsButton.checked = false
      }
    }
  }
}
