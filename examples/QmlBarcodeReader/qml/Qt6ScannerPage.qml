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
         || Qt.platform.os === "ios" ? Screen.width : 1280 //camera.viewfinder.resolution.width
  height: Qt.platform.os === "android"
          || Qt.platform.os === "ios" ? Screen.height : 720 //camera.viewfinder.resolution.height

  SBarcodeScanner {
    id: barcodeScanner

    videoSink: videoOutput.videoSink

    captureRect: Qt.rect(parent.width / 4, parent.height / 4, parent.width / 2,
                         parent.height / 2)

    onCapturedChanged: function (captured) {
      scanResultText.text = captured
      resultScreen.visible = true
      barcodeScanner.pauseProcessing()
    }
  }
  //  Frame {
  //    id: frames
  //    videoSink: videoOutput.videoSink
  //  }

  //  Camera {
  //    id: camera

  //    Component.onCompleted: barcodeScanner.camera = camera

  //    focusMode: Camera.FocusModeAutoNear
  //    customFocusPoint: Qt.point(0.2, 0.2)
  //  }

  //  VideoOutput {
  //    id: videoOutput

  //    anchors.fill: parent

  //    fillMode: VideoOutput.PreserveAspectCrop

  //    onSourceRectChanged: {
  //      console.log(barcodeScanner.captureRect)
  //    }
  //  }
  VideoOutput {
    id: videoOutput

    anchors.fill: parent

    focus: visible
    width: root.width
    //    transform: [
    //      Scale {
    //        origin.x: root.width / 2
    //        origin.y: root.height / 2
    //        xScale: barcodeScanner.xScale
    //        yScale: barcodeScanner.yScale
    //      },
    //      Rotation {
    //        origin.x: root.width / 2
    //        origin.y: root.height / 2
    //        angle: barcodeScanner.angle
    //      }
    //    ]
  }

  Qt6ScannerOverlay {
    id: scannerOverlay

    anchors.fill: parent

    captureRect: Qt.rect(parent.width / 4, parent.height / 4, parent.width / 2,
                         parent.height / 2)
  }
  MouseArea {
    id: focusTouchArea

    anchors.fill: parent

    onClicked: {
      camera.focus.customFocusPoint = Qt.point(mouse.x / width,
                                               mouse.y / height)
      camera.focus.focusMode = CameraFocus.FocusMacro
      camera.focus.focusPointMode = CameraFocus.FocusPointCustom
    }
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
          barcodeScanner.continueProcessing()
        }
      }
    }
  }

  onHeightChanged: {
    barcodeScanner.captureRect = Qt.rect(width / 4, height / 4, width / 2,
                                         height / 2)
  }

  onWidthChanged: {
    barcodeScanner.captureRect = Qt.rect(width / 4, height / 4, width / 2,
                                         height / 2)
  }
}
