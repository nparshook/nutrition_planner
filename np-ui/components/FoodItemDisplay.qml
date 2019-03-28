import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0;

Item {
    property FoodItem foodItem
    ColumnLayout {
        Label {
            id: longDescLabel
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pointSize: 28
            text: foodItem ? foodItem.ui_foodID.ui_longDesc : ''
        }
    }
}
