import QtQuick 2.12
import QtGraphicalEffects 1.12


/*!
  Area for scanning barcodes
  */
Item {
  id: root

  property rect captureRect

  Item {
    id: captureZoneCorners

    x: root.captureRect.x
    y: root.captureRect.y

    width: root.captureRect.width
    height: root.captureRect.height

    Rectangle {
      id: topLeftCornerH

      anchors {
        top: parent.top
        left: parent.left
      }

      width: 20
      height: 5

      color: Theme.borderColor
      radius: height / 2
    }

    Rectangle {
      id: topLeftCornerV
      anchors {
        top: parent.top
        left: parent.left
      }

      width: 5
      height: 20

      color: Theme.borderColor
      radius: width / 2
    }

    // ----------------------
    Rectangle {
      id: bottomLeftCornerH

      anchors {
        bottom: parent.bottom
        left: parent.left
      }

      width: 20
      height: 5

      color: Theme.borderColor
      radius: height / 2
    }

    Rectangle {
      id: bottomLeftCornerV

      anchors {
        bottom: parent.bottom
        left: parent.left
      }

      width: 5
      height: 20

      color: Theme.borderColor
      radius: width / 2
    }

    // ----------------------
    Rectangle {
      id: topRightCornerH

      anchors {
        top: parent.top
        right: parent.right
      }

      width: 20
      height: 5

      color: Theme.borderColor
      radius: height / 2
    }

    Rectangle {
      id: topRightCornerV

      anchors {
        top: parent.top
        right: parent.right
      }

      width: 5
      height: 20

      color: Theme.borderColor
      radius: width / 2
    }

    // ----------------------
    Rectangle {
      id: bottomRightCornerH

      anchors {
        bottom: parent.bottom
        right: parent.right
      }

      width: 20
      height: 5

      color: Theme.borderColor
      radius: height / 2
    }

    Rectangle {
      id: bottomRightCornerV

      anchors {
        bottom: parent.bottom
        right: parent.right
      }

      width: 5
      height: 20

      color: Theme.borderColor
      radius: width / 2
    }

    Rectangle {
      id: scanIndicator

      //y: captureZoneCorners.height/2
      width: parent.width
      height: 1

      anchors {
        horizontalCenter: parent.horizontalCenter
      }

      color: Theme.borderColor

      SequentialAnimation {
        id: scanIndicatorAnimation

        loops: Animation.Infinite

        PropertyAnimation {
          id: toTopAnimation
          target: scanIndicator
          property: "y"
          duration: 2000
        }

        PropertyAnimation {
          id: toBottomAnimation
          target: scanIndicator
          property: "y"
          duration: 2000
        }
      }
    }

    RectangularGlow {
      id: effect

      width: scanIndicator.width / 2
      height: scanIndicator.height

      anchors.centerIn: scanIndicator

      glowRadius: 50
      spread: 0.2
      color: Theme.borderColor
      cornerRadius: glowRadius
    }
  }

  Text {
    id: scanCapsuleText

    anchors {
      verticalCenter: captureZoneCorners.bottom
      horizontalCenter: captureZoneCorners.horizontalCenter
    }

    text: qsTr("Scan barcode")
    color: Theme.borderColor
  }

  onCaptureRectChanged: {
    toTopAnimation.to = 5
    toBottomAnimation.to = captureRect.height - 5
    scanIndicatorAnimation.start()
  }
}
