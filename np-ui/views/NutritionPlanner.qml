import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import NP 1.0
import components 1.0

Item {
    anchors.fill: parent

    GridLayout {
        id: grid
        columns: 3
        anchors.fill: parent

        Page {
            id: dietPage

            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: "Diets"
                    z: 1
                }

                Button {
                    Layout.fillWidth: true
                    text: "New Diet"
                    onClicked: masterController.addDiet()
                }
            }
            ListView {
                id: dietList
                anchors.fill: parent
                focus: true
                currentIndex: -1
                highlightFollowsCurrentItem: true
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.ui_name
                    highlighted: { if(ListView.isCurrentItem) true; else false}
                    onClicked: {
                        if(dietList.currentItem) {
                            dietList.currentItem.highlighted = false
                        }
                        dietList.currentIndex = index
                        masterController.ui_currentDiet = masterController.ui_diets[dietList.currentIndex]
                        foodDisplay.foodItem = masterController.ui_currentDiet.foodTotalEq
                        dietList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: masterController.ui_diets

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: dietList.currentIndex = -1
            }
        }

        StackView {
            id: nutrContent
            clip: true
            Layout.fillHeight: true
            Layout.preferredWidth: grid.width / 2
            Layout.rowSpan: 2
            initialItem: FoodItemDisplay {
                id: foodDisplay
            }
        }

        Page {
            id: dayPage
            property FoodItemList diet: masterController.ui_currentDiet
            visible: dayPage.diet ? true : false
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: "Days"
                    z: 1
                }

                Button {
                    Layout.fillWidth: true
                    text: "New Day"
                    onClicked: dayPage.diet.addSubFoodList()
                }
            }
            ListView {
                id: dayList
                anchors.fill: parent
                focus: true
                currentIndex: -1
                highlightFollowsCurrentItem: true
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.ui_name
                    highlighted: { if(ListView.isCurrentItem) true; else false}
                    onClicked: {
                        if(dayList.currentItem) {
                            dayList.currentItem.highlighted = false
                        }
                        dayList.currentIndex = index
                        masterController.ui_currentDay = dayPage.diet.ui_subFoodLists[dayList.currentIndex]
                        foodDisplay.foodItem = masterController.ui_currentDay.foodTotalEq
                        dayList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: dayPage.diet ? dayPage.diet.ui_subFoodLists : []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: dayList.currentIndex = -1
            }
        }

        Page {
            id: mealPage
            property FoodItemList day: masterController.ui_currentDay
            visible: mealPage.day ? true : false
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: "Meals"
                    z: 1
                }

                Button {
                    Layout.fillWidth: true
                    text: "New Meal"
                    onClicked: masterController.ui_currentDay.addSubFoodList()
                }
            }
            ListView {
                id: mealList
                anchors.fill: parent
                focus: true
                currentIndex: -1
                highlightFollowsCurrentItem: true
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.ui_name
                    highlighted: { if(ListView.isCurrentItem) true; else false}
                    onClicked: {
                        if(mealList.currentItem) {
                            mealList.currentItem.highlighted = false
                        }
                        mealList.currentIndex = index
                        masterController.ui_currentMeal = mealPage.day.ui_subFoodLists[mealList.currentIndex]
                        foodDisplay.foodItem = masterController.ui_currentMeal.foodTotalEq
                        mealList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: mealPage.day ? mealPage.day.ui_subFoodLists : []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: mealList.currentIndex = -1
            }
        }

        Page {
            id: foodPage
            property FoodItemList meal: masterController.ui_currentMeal
            visible: foodPage.meal ? true : false
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: foodPage.meal ? foodPage.meal.ui_name + " Ingredients" : ""
                    z: 1
                }

                Button {
                    Layout.fillWidth: true
                    text: "Get Food"
                    onClicked: {
                        nutrContent.push("qrc:/views/FoodSelection.qml")
                        nutrContent.currentItem.foodSelectionFinished.connect(nutrContent.pop)
                        nutrContent.currentItem.foodSelectionFinished.connect(reEnable)
                        nutrContent.currentItem.addFoodItem.connect(foodPage.appendFoodItem)
                        enabled = false
                    }

                    function reEnable() {
                        console.log("Renabled")
                        enabled = true
                    }
                }
            }

            function appendFoodItem(item) {
                foodPage.meal.appendFood(item)
                foodsList.currentIndex = foodPage.meal.foodItems.length - 1
                foodDisplay.foodItem = foodPage.meal.foodItems[foodsList.currentIndex]
            }

            ListView {
                id: foodsList
                anchors.fill: parent
                focus: true
                currentIndex: -1
                highlightFollowsCurrentItem: true
                delegate: ItemDelegate {
                    width: parent.width
                    text: model.ui_foodID.ui_longDesc
                    highlighted: { if(ListView.isCurrentItem) true; else false}
                    onClicked: {
                        if(foodsList.currentItem) {
                            foodsList.currentItem.highlighted = false
                        }
                        foodsList.currentIndex = index
                        foodDisplay.foodItem = foodPage.meal.foodItems[foodsList.currentIndex]
                        foodsList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: foodPage.meal ? foodPage.meal.foodItems : null

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: foodsList.currentIndex = -1
            }
        }
    }
}
