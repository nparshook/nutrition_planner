import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0

Dialog {
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
        anchors.fill: parent
        FoodSearcher {
            id: foodSearch
            width: parent.width / 4
            height: parent.height
        }

        FoodItemDisplay {
            id: foodDisplay
            width: parent.width / 4 * 3
            height: parent.height
        }
    }
    Connections {
        target: foodSearch
        onFoodItemSelected: foodDisplay.foodItem = item
    }
}
