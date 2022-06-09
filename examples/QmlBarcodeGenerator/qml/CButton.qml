import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12


/*!
  Classic push button for configure, generate & save operations.
  */
Button {
  id: root

  height: 100
  implicitWidth: appWindow.width / 4

  checkable: true

  palette.buttonText: Theme.textColor

  background: Rectangle {
    radius: 10
    color: root.pressed ? Qt.darker(
                            Theme.backgroundColor) : Theme.backgroundColor
  }
}
