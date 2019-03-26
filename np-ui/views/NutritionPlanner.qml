import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import NP 1.0
import components 1.0

Item {
    anchors.fill: parent

    FoodSelectDialog {
        id: getFoodDialog
        modal: true
        x: (parent.width - width) / 2
        y: parent.height / 6
        width: parent.width / 3 * 2
        height: parent.height / 3 * 2
    }

    GridLayout {
        id: grid
        columns: 3
        anchors.fill: parent

        Rectangle {
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4
            color: "steelblue"
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: grid.width / 2
            Layout.rowSpan: 2
            color: "steelblue"
        }

        Rectangle {
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4
            color: "steelblue"
        }

        Rectangle {
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4
            color: "steelblue"
        }

        Page {
            id: foodPage
            property FoodItemList meal
            visible: true
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            header: RowLayout {
                Layout.fillWidth: true
                Text {
                    text: foodPage.meal ? foodPage.meal.name + " Ingredients" : ""
                    z: 1
                }

                Button {
                    text: "Get Food"
                    onClicked: {
                        getFoodDialog.focus = true
                        getFoodDialog.open()
                    }
                }
            }

            Connections {
                target: getFoodDialog
                onAccepted: {
                    foodPage.meal.appendFood(selectFoodDialog.foodItem)
                    foodsList.currentIndex = foodPage.meal.foodItems.length - 1
                    foodDisplay.foodItem = foodPage.meal.foodItems[foodsList.currentIndex]
                    foodDisplay.servings = true
                    foodDisplay.blank = false
                }
            }

            ListView {
                id: foodsList
                anchors.fill: parent
                focus: true
                currentIndex: -1
                highlightFollowsCurrentItem: true
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.foodID.longDesc
                    highlighted: { if(ListView.isCurrentItem) true; else false}
                    onClicked: {
                        if(foodsList.currentItem && index != mealList.currentIndex) {
                            foodsList.currentItem.highlighted = false
                        }
                        foodsList.currentIndex = index
                        foodDisplay.foodItem = foodPage.meal.foodItems[foodsList.currentIndex]
                        foodDisplay.servings = true
                        foodDisplay.blank = false
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: foodPage.meal ? foodPage.meal.foodItems : null

                ScrollIndicator.vertical: ScrollIndicator { }

                onModelChanged: foodsList.currentIndex = -1
            }
        }
    }
}
