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

  //width: Qt.platform.os === "android"
  //        || Qt.platform.os === "ios" ? Screen.width : camera.viewfinder.resolution.width
  //height: Qt.platform.os === "android"
  //        || Qt.platform.os === "ios" ? Screen.height : camera.viewfinder.resolution.height
  TestClass {
    id: testclass

    videoSink: videoOutput.videoSink

    captureRect: Qt.rect(width / 4, height / 4, width / 2, height / 2)
    // you can adjust capture rect (scan area) ne changing these Qt.rect() parameters
    // captureRect: videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(0.25, 0.25,  0.5, 0.5)))
    onCapturedChanged: {
      //active = false
      console.log("captured: " + captured)
    }
  }

  Camera {
    id: camera

    Component.onCompleted: testclass.qcamera = camera

    focusMode: Camera.FocusModeAutoNear
    customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
  }

  VideoOutput {
    id: videoOutput

    anchors.fill: parent

    fillMode: VideoOutput.PreserveAspectCrop

    onSourceRectChanged: {

      testclass.captureRect = Qt.rect(parent.width / 4, parent.height / 4,
                                      parent.width / 2, parent.height / 2)
      //testclass.captureRect = videoOutput.mapRectToSource(videoOutput.mapNormalizedRectToItem(Qt.rect(0.25, 0.25, 0.5, 0.5)))
      console.log(testclass.captureRect)
    }
  }

  Qt6ScannerOverlay {
    id: scannerOverlay

    anchors.fill: parent

    captureRect: Qt.rect(
                   parent.width / 4, parent.height / 4, parent.width / 2,
                   parent.height / 2) //videoOutput.mapRectToItem(testclass.captureRect)
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

    visible: false //!barcodeFilter.active

    Column {
      anchors.centerIn: parent

      spacing: 20

      Text {
        id: scanResultText

        anchors.horizontalCenter: parent.horizontalCenter

        //text: testclass.captured()
      }

      Button {
        id: scanButton

        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("Scan again")

        onClicked: {

          //barcodeFilter.active = true
        }
      }
    }
  }

  onHeightChanged: {
    testclass.captureRect = Qt.rect(
          width / 4, height / 4, width / 2,
          height / 2) //videoOutput.mapRectToItem(testclass.captureRect)
  }

  onWidthChanged: {
    testclass.captureRect = Qt.rect(
          width / 4, height / 4, width / 2,
          height / 2) //videoOutput.mapRectToItem(testclass.captureRect)
  }
}
