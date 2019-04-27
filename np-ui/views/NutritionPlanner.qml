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
                    onClicked:
                    {
                        masterController.addDiet()
                        nutrContent.replace("qrc:/components/DietDisplay.qml")
                        nutrContent.currentItem.diet = masterController.ui_currentDiet
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Delete Selected Diet"
                    enabled: dietList.currentItem ? true : false
                    onClicked:
                    {
                        masterController.removeCurrentDiet()
                        //dietList.model = masterController.ui_diets
                        dayPage.clearModel()
                        nutrContent.clear()
                    }
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
                        nutrContent.replace("qrc:/components/DietDisplay.qml")
                        nutrContent.currentItem.diet = masterController.ui_currentDiet
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
            property Diet diet: masterController.ui_currentDiet
            enabled: dayPage.diet ? true : false
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            function clearModel() {
                dayList.model = []
                mealPage.clearModel()
            }

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: "Days"
                    z: 1
                }

                Button {
                    Layout.fillWidth: true
                    text: "New Day"
                    onClicked: {
                        masterController.ui_currentDay = dayPage.diet.newDay()
                        nutrContent.replace("qrc:/components/DayDisplay.qml")
                        nutrContent.currentItem.day = masterController.ui_currentDay
                        dayList.model = dayPage.diet.ui_days
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Delete Selected Day"
                    enabled: dayList.currentItem ? true : false
                    onClicked:
                    {
                        dayPage.diet.removeDay(dayPage.diet.ui_days[dayList.currentIndex])
                        dayList.model = dayPage.diet.ui_days
                        mealPage.clearModel()
                        masterController.clearCurrentDay()
                    }
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
                        masterController.ui_currentDay = dayPage.diet.ui_days[dayList.currentIndex]
                        nutrContent.replace("qrc:/components/DayDisplay.qml")
                        nutrContent.currentItem.day = masterController.ui_currentDay
                        dayList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: dayList.currentIndex = -1


            }
            onDietChanged: if(dayPage.diet) {dayList.model = dayPage.diet.ui_days} else {dayList.model = []}
        }

        Page {
            id: mealPage
            property Day day: masterController.ui_currentDay
            enabled: mealPage.day ? true : false
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            function clearModel() {
                mealList.model = []
                foodPage.clearModel()
            }

            onDayChanged: if(mealPage.day) {mealList.model = mealPage.day.ui_meals} else {mealList.model = []}

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: "Meals"
                    z: 1
                }

                Button {
                    Layout.fillWidth: true
                    text: "New Meal"
                    onClicked: {
                        masterController.ui_currentMeal = mealPage.day.newMeal()
                        nutrContent.replace("qrc:/components/MealDisplay.qml")
                        nutrContent.currentItem.meal = masterController.ui_currentMeal
                        mealList.model = mealPage.day.ui_meals
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Delete Selected Meal"
                    enabled: mealList.currentItem ? true : false
                    onClicked:
                    {
                        mealPage.day.removeMeal(mealPage.day.ui_meals[mealList.currentIndex])
                        mealList.model = mealPage.day.ui_meals
                        foodPage.clearModel()
                        masterController.clearCurrentMeal()
                    }
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
                        masterController.ui_currentMeal = mealPage.day.ui_meals[mealList.currentIndex]
                        nutrContent.replace("qrc:/components/MealDisplay.qml")
                        nutrContent.currentItem.meal = masterController.ui_currentMeal
                        nutrContent.currentItem.weight = true
                        mealList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: mealList.currentIndex = -1
            }
        }

        Page {
            id: foodPage
            property Meal meal: masterController.ui_currentMeal
            enabled: foodPage.meal ? true : false
            Layout.preferredHeight: grid.height / 2
            Layout.preferredWidth: grid.width / 4

            onMealChanged: if(foodPage.meal) {foodsList.model = meal.ui_foods} else {foodsList.model = []}

            function clearModel() {
                foodsList.model = []
            }

            header: RowLayout {
                Text {
                    Layout.fillWidth: true
                    text: foodPage.meal ? foodPage.meal.ui_name + " Ingredients" : ""
                    z: 1
                }

                Button {
                    id: getFoodBtn
                    Layout.fillWidth: true
                    text: "Get Food"
                    onClicked: {
                        nutrContent.push("qrc:/views/FoodSelection.qml")
                        nutrContent.currentItem.forceActiveFocus()
                        nutrContent.currentItem.foodSelectionFinished.connect(nutrContent.pop)
                        nutrContent.currentItem.foodSelectionFinished.connect(reEnable)
                        nutrContent.currentItem.foodSelectionFinished.connect(setModel)
                        nutrContent.currentItem.addFoodItem.connect(foodPage.appendFoodItem)
                        foodPage.enabled = false
                        dietPage.enabled = false
                        dayPage.enabled = false
                        mealPage.enabled = false
                    }

                    function setModel() {
                        foodsList.model = foodPage.meal.ui_foods
                    }

                    function reEnable() {
                        foodPage.enabled = true
                        dietPage.enabled = true
                        dayPage.enabled = true
                        mealPage.enabled = true
                    }
                }
                Button {
                    Layout.fillWidth: true
                    text: "Delete Selected Food"
                    enabled: foodsList.currentItem ? true : false
                    onClicked:
                    {
                        foodPage.meal.removeFood(foodPage.meal.ui_foods[foodsList.currentIndex])
                        foodsList.model = foodPage.meal.ui_foods
                    }
                }
            }

            function appendFoodItem(item) {
                foodPage.meal.appendFood(item)
                foodsList.currentIndex = foodPage.meal.ui_foods.length - 1
                foodDisplay.foodItem = foodPage.meal.ui_foods[foodsList.currentIndex]
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
                        nutrContent.replace("qrc:/components/FoodItemDisplay.qml")
                        nutrContent.currentItem.servings = true
                        nutrContent.currentItem.foodItem = foodPage.meal.ui_foods[foodsList.currentIndex]
                        foodsList.currentItem.highlighted = true
                    }

                    onHoveredChanged: {
                        if (!ListView.isCurrentItem)
                            highlighted = !highlighted
                    }
                }

                model: []

                ScrollIndicator.vertical: ScrollIndicator {}

                onModelChanged: foodsList.currentIndex = -1
            }
        }
    }
}
