import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import NP 1.0
import components 1.0

Page {
    signal foodSelectionFinished()
    signal addFoodItem(FoodItem item)

    GridLayout {
        columns: 3
        anchors.fill: parent
        FoodSearcher {
            id: foodSearch
            Layout.preferredWidth: parent.width / 4
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 2
        }

        Button {
            text: "Add Food"
            Layout.fillWidth: true
            onClicked: {
                if(foodDisplay.foodItem) {
                    addFoodItem(foodDisplay.foodItem)
                }
            }
        }

        Button {
            text: "Finished"
            Layout.fillWidth: true
            onClicked: foodSelectionFinished()
        }

        FoodItemDisplay {
            id: foodDisplay
            Layout.preferredWidth: parent.width / 4 * 3
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
        }
    }
    Connections {
        target: foodSearch
        onFoodItemSelected: foodDisplay.foodItem = item
    }
}
