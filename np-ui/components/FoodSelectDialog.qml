import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0

Dialog {
    id: root
    title: "Select Food from Database"
    standardButtons: Dialog.Ok | Dialog.Cancel
    property FoodItem foodItem

    onAccepted: {
        //foodItem = foodDisplay.foodItem
        foodSearch.clear()
        //foodDisplay.clear()
    }

    onRejected: {
        foodSearch.clear()
        //foodDisplay.clear()
    }

    RowLayout
    {
        id: dialogContent
        anchors.fill: parent
        FoodSearcher {
            id: foodSearch
            width: parent.width / 4
            height: parent.width / 4
            Layout.maximumWidth: parent.width / 4
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        FoodItemDisplay {
            id: foodDisplay
            Layout.preferredWidth: parent.width / 4 * 3
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
    Connections {
        target: foodSearch
        onFoodItemSelected: foodDisplay.foodItem = item
    }
}
