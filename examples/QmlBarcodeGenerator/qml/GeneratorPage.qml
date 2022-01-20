import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Layouts 1.12
import com.scythestudio.scodes 1.0

ApplicationWindow {
  id: root
  visible: true
  width: 400
  height: 800

  SBarcodeGenerator {
    id: barcodeGenerator

    onGenerationFinished: {
      if (error == "") {
        console.log(barcodeGenerator.filePath)
        image.source = "file:///" + barcodeGenerator.filePath
      } else {
        generateLabel.text = error
        generatePopup.open()
      }
    }
  }

  Rectangle {
    anchors.fill: parent
    height: parent.height
    width: parent.width

    Rectangle {
      id: inputRect
      z: 100
      anchors {
        top: parent.top
        left: parent.left
        right: parent.right
      }
      height: 40

      TextField {
        id: textField
        anchors.fill: parent
        selectByMouse: true
        placeholderText: qsTr("Input")
      }
    }

    Rectangle {
      id: imageRect
      anchors {
        top: inputRect.bottom
        bottom: buttonsRect.top
        left: parent.left
        right: parent.right
      }

      Image {
        id: image
        anchors.centerIn: parent
        width: parent.width
        height: image.width
        cache: false
      }
    }

    Rectangle {
      id: buttonsRect
      anchors {
        bottom: parent.bottom
        left: parent.left
        right: parent.right
      }
      height: 40

      RowLayout {
        id: buttonsLayout
        anchors.fill: parent

        CButton {
          id: settingsButton
          text: qsTr("Settings")
          Layout.leftMargin: 5
          onClicked: {
            settingsPopup.open()
          }
        }

        CButton {
          id: generateButton
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
          text: qsTr("Save")
          Layout.rightMargin: 5
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

      onClosed: {
        generateButton.checked = false
      }

      Label {
        id: generateLabel
        anchors.centerIn: parent
      }
    }

    Popup {
      id: imageSavedPopup
      anchors.centerIn: parent
      dim: true
      modal: true

      onClosed: {
        saveButton.checked = false
      }

      Label {
        id: saveLabel
        anchors.centerIn: parent
      }
    }

    Popup {
      id: settingsPopup
      anchors.centerIn: parent
      width: parent.width * 0.6
      height: parent.height * 0.6
      dim: true
      modal: true

      onClosed: {
        settingsButton.checked = false
      }

      ColumnLayout {
        anchors.fill: parent
        spacing: 5

        CTextField {
          id: height
          placeholderText: "Current width: " + barcodeGenerator.width
          onEditingFinished: barcodeGenerator.width = parseInt(text)
        }

        CTextField {
          placeholderText: "Current height: " + barcodeGenerator.height
          onEditingFinished: barcodeGenerator.height = parseInt(text)
        }

        CTextField {
          placeholderText: "Current margin: " + barcodeGenerator.margin
          onEditingFinished: barcodeGenerator.margin = parseInt(text)
        }

        CTextField {
          placeholderText: "Current ECC Level: " + barcodeGenerator.eccLevel
          onEditingFinished: {
            barcodeGenerator.eccLevel = text
          }
        }

        CComboBox {
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
              text: "DataBar"
            }
            ListElement {
              text: "DataBarExpanded"
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
              text: "MaxiCode"
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
          onCurrentIndexChanged: {
            var formatAsText = formats.get(currentIndex).text
            // a separate method was used because of qml error
            // when try to use it as overloaded setter
            barcodeGenerator.setFormat(formatAsText)
          }
        }

        CComboBox {
          model: ListModel {
            id: extensions

            ListElement {
              text: "png"
            }

            ListElement {
              text: "jpg"
            }
          }
          onCurrentIndexChanged: {
            barcodeGenerator.extension = extensions.get(currentIndex).text
          }
        }

        CTextField {
          placeholderText: "Current file name: " + barcodeGenerator.fileName
          onEditingFinished: {
            barcodeGenerator.fileName = text
          }
        }
      }
    }
  }
}
