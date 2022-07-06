import QtQuick
import QtQuick.Controls
import QtMultimedia
import com.scythestudio.scodes 1.0


/*!
  Barcode scanner main page. All QML elements managing from here.
  */
ApplicationWindow {
  id: root

  visible: true

  width: Qt.platform.os === "android"
         || Qt.platform.os === "ios" ? Screen.width : 1280
  height: Qt.platform.os === "android"
          || Qt.platform.os === "ios" ? Screen.height : 720

  SBarcodeFilter {
    id: barcodeFilter

    videoSink: videoOutput.videoSink

    captureRect: Qt.rect(root.width / 4, root.height / 4, root.width / 2,
                         root.height / 2)

    onCapturedChanged: function (captured) {
      scanResultText.text = captured
      resultScreen.visible = true
      barcodeFilter.pauseProcessing()
    }
  }

  VideoOutput {
    id: videoOutput

    width: root.width

    anchors.fill: parent

    focus: visible

    fillMode: VideoOutput.PreserveAspectCrop
  }

  Qt6ScannerOverlay {
    id: scannerOverlay

    anchors.fill: parent

    captureRect: barcodeFilter.captureRect
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

        implicitWidth: 100
        implicitHeight: 50

        anchors.horizontalCenter: parent.horizontalCenter

        Text {
          anchors.centerIn: parent

          text: qsTr("Scan again")

          color: Theme.textColor
        }

        onClicked: {
          resultScreen.visible = false
          barcodeFilter.continueProcessing()

        }
      }
    }
  }

  onHeightChanged: {
    barcodeFilter.captureRect = Qt.rect(width / 4, height / 4, width / 2,
                                         height / 2)
  }

  onWidthChanged: {
    barcodeFilter.captureRect = Qt.rect(width / 4, height / 4, width / 2,
                                         height / 2)
  }
}
