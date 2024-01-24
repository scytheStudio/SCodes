import QtQuick
import QtQuick.Controls
import QtMultimedia
import com.scythestudio.scodes 1.0


/*!
  Barcode scanner main page. All QML elements managing from here.
  */
ApplicationWindow {
  id: root

  width: Qt.platform.os === "android"
         || Qt.platform.os === "ios" ? Screen.width : 1280
  height: Qt.platform.os === "android"
          || Qt.platform.os === "ios" ? Screen.height : 720

  visible: true

  SBarcodeScanner {
    id: barcodeScanner

    forwardVideoSink: videoOutput.videoSink

    captureRect: Qt.rect(1 / 4, 1 / 4, 1 / 2, 1 / 2)

    onCapturedChanged: function (captured) {
      scanResultText.text = captured
      resultScreen.visible = true
      barcodeScanner.setProcessing(0)
    }
  }

  VideoOutput {
    id: videoOutput

    anchors.fill: parent

    width: root.width

    focus: visible
    fillMode: VideoOutput.PreserveAspectCrop
  }

  Qt6ScannerOverlay {
    id: scannerOverlay

    anchors.fill: parent

    captureRect: barcodeScanner.captureRect
  }

  Rectangle {
    id: resultScreen

    anchors.fill: parent

    visible: false

    Column {
      anchors.centerIn: parent

      spacing: 20

      Text {
        id: scanResultText

        anchors.horizontalCenter: parent.horizontalCenter

        color: Theme.textColor
      }

      Button {
        id: scanButton

        anchors.horizontalCenter: parent.horizontalCenter

        implicitWidth: 100
        implicitHeight: 50

        Text {
          anchors.centerIn: parent

          text: qsTr("Scan again")
          color: Theme.textColor
        }

        onClicked: {
          resultScreen.visible = false
          barcodeScanner.setProcessing(1)
          barcodeScanner.continueProcessing()
        }
      }
    }
  }
}
