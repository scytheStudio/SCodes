import QtQuick 2.12
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Layouts 1.12
import com.scythestudio.sbarcodereader 1.0

Item {
  id: root

  anchors.fill: parent

  //  width: Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.width: camera.viewfinder.resolution.width
  SBarcodeGenerator {
    id: barcodeGenerator

    onProcessFinished: {
      console.log(barcodeGenerator.filePath)
      image.source = "file:///" + barcodeGenerator.filePath
    }
  }

  Rectangle {
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

      TextField {
        id: textField

        anchors.fill: parent

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

        width: parent.width
        height: image.width

        anchors.centerIn: parent

        cache: false
      }
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

        anchors.fill: parent

        CButton {
          id: settingsButton

          text: qsTr("Settings")

          onClicked: {
            !checked ? settings.visible = false : settings.visible = true
          }
        }

        CButton {
          id: generateButton

          text: qsTr("Generate")

          onClicked: {
            image.source = ""
            barcodeGenerator.process(textField.text)
          }
        }

        CButton {
          id: saveButton

          text: qsTr("Save")

          onClicked: {
            barcodeGenerator.saveImage()
          }
        }

        CButton {
          id: backButton

          text: qsTr("Back")

          onClicked: {
            root.visible = false
          }
        }
      }
    }

    Rectangle {
      id: settings

      width: parent.width * 0.6
      height: parent.height * 0.6

      anchors.centerIn: parent

      visible: false

      border {
        color: "#333"
        width: 1
      }

      Column {
        anchors.fill: parent

        spacing: 2

        CTextField {
          placeholderText: "Current width: " + barcodeGenerator.width

          onEditingFinished: barcodeGenerator.width = text
        }

        CTextField {
          placeholderText: "Current height: " + barcodeGenerator.height

          onEditingFinished: barcodeGenerator.height = text
        }

        CTextField {
          placeholderText: "Current margin: " + barcodeGenerator.margin

          onEditingFinished: barcodeGenerator.margin = text
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
              text: "QR_CODE"
            }

            ListElement {
              text: "DATA_MATRIX"
            }

            ListElement {
              text: "CODE_128"
            }
          }
          onCurrentIndexChanged: {
            barcodeGenerator.barcodeFormatFromQMLString(formats.get(
                                                          currentIndex).text)
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
