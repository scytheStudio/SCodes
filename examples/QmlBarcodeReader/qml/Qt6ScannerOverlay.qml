import QtQuick
import Qt5Compat.GraphicalEffects


/*!
  Area for scanning barcodes
  */
Item {
  id: root

  property rect captureRect

  Item {
    id: captureZoneCorners

    x: parent.width * root.captureRect.x
    y: parent.height * root.captureRect.y

    width: parent.width * root.captureRect.width
    height: parent.height * root.captureRect.height

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

      anchors {
        horizontalCenter: parent.horizontalCenter
      }

      width: parent.width
      height: 1

      color: Theme.borderColor

      SequentialAnimation {
        id: scanIndicatorAnimation

        loops: Animation.Infinite
        PropertyAnimation {
          id: toTopAnimation

          target: scanIndicator
          property: "y"
          duration: 2000
          to: captureZoneCorners.height
        }

        PropertyAnimation {
          id: toBottomAnimation

          target: scanIndicator
          property: "y"
          duration: 2000
          to: 0
        }
      }
    }

    RectangularGlow {
      id: effect

      anchors.centerIn: scanIndicator

      width: scanIndicator.width / 2
      height: scanIndicator.height

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
    scanIndicatorAnimation.start()
  }
}
