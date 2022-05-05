import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


/*!
  Classic push button for configure, generate & save operations.
  */
Button {
  id: root

  checkable: true

  palette.buttonText: theme.textColor

  background: Rectangle {
    radius: 10
    color: root.pressed ? Qt.darker(
                            theme.backgroundColor) : theme.backgroundColor
  }
}
