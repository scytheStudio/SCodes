import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


/*!
  Classic push button for configure, generate & save operations.
  */
Button {
  id: root

  Layout.alignment: Qt.AlignHCenter
  Layout.fillHeight: true
  Layout.fillWidth: true
  Layout.bottomMargin: 10

  checkable: true

  palette.buttonText: "#bdbdbd"

  background: Rectangle {
    radius: 10
    color: "#218165"
  }
}
