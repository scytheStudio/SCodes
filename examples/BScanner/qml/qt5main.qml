import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
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
  }
  VideoOutput {
    source: camera
    focus: visible
    anchors.fill: parent

    MouseArea {
      anchors.fill: parent
      hoverEnabled: true
      onClicked: testclass.capture()
    }
  }
}
