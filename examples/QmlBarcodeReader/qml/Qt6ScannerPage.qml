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

  width: Qt.platform.os == "android"
         || Qt.platform.os == "ios" ? Screen.width : camera.viewfinder.resolution.width
  height: Qt.platform.os == "android"
          || Qt.platform.os == "ios" ? Screen.height : camera.viewfinder.resolution.height

  TestClass {
    id: testclass

    videoSink: videoOutput.videoSink
  }

  Camera {
    id: camera

    Component.onCompleted: testclass.qcamera = camera

    focusMode: Camera.FocusModeAutoNear
    customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
  }

  //    videoOutput: videoOutput
  //  }
  VideoOutput {
    id: videoOutput
    anchors.fill: parent

    //    MouseArea {
    //      anchors.fill: parent
    //      onClicked: testclass.capture()
    //    }
    //Component.onCompleted: testclass.start()
  }
} //  VideoOutput {//    id: videoOutput//    anchors.fill: parent
//    //source: camera

//    //autoOrientation: true
//    fillMode: VideoOutput.PreserveAspectCrop

//    // add barcodeFilter to videoOutput's filters to enable catching barcodes
//    //filters: [barcodeFilter]
//    onSourceRectChanged: {
//      barcodeFilter.captureRect = videoOutput.mapRectToSource(
//            videoOutput.mapNormalizedRectToItem(Qt.rect(0.25, 0.25, 0.5, 0.5)))
//    }

//    Qt6ScannerOverlay {
//      id: scannerOverlay

//      anchors.fill: parent

//      captureRect: videoOutput.mapRectToItem(barcodeFilter.captureRect)
//    }

//    // used to get camera focus on touched point
//    MouseArea {
//      id: focusTouchArea

//      anchors.fill: parent

//      onClicked: {
//        camera.focus.customFocusPoint = Qt.point(mouse.x / width,
//                                                 mouse.y / height)
//        camera.focus.focusMode = CameraFocus.FocusMacro
//        camera.focus.focusPointMode = CameraFocus.FocusPointCustom
//      }
//    }
//  }

//  //  SBarcodeFilter {
//  //    id: barcodeFilter

//  //    // you can adjust capture rect (scan area) ne changing these Qt.rect() parameters
//  //    captureRect: videoOutput.mapRectToSource(
//  //                   videoOutput.mapNormalizedRectToItem(Qt.rect(0.25, 0.25,
//  //                                                               0.5, 0.5)))

//  //    onCapturedChanged: {
//  //      active = false
//  //      console.log("captured: " + captured)
//  //    }
//  //  }
//  Rectangle {
//    id: resultScreen

//    anchors.fill: parent

//    visible: !barcodeFilter.active

//    Column {
//      anchors.centerIn: parent

//      spacing: 20

//      Text {
//        id: scanResultText

//        anchors.horizontalCenter: parent.horizontalCenter

//        text: barcodeFilter.captured
//      }

//      Button {
//        id: scanButton

//        anchors.horizontalCenter: parent.horizontalCenter

//        text: qsTr("Scan again")

//        onClicked: {
//          barcodeFilter.active = true
//        }
//      }
//    }
//  }
//}

