import QtQuick
import QtMultimedia
import com.scythestudio.scodes 1.0

Window {
  width: 640
  height: 480
  visible: true
  title: qsTr("Hello World")

  TestClass {
    id: testclass
  }

  Camera {
    id: camera

    Component.onCompleted: testclass.qcamera = camera

    focusMode: Camera.FocusModeAutoNear

    customFocusPoint: Qt.point(0.2, 0.2)
  }

  VideoOutput {
    id: videoOutput
    anchors.fill: parent
  }
}
