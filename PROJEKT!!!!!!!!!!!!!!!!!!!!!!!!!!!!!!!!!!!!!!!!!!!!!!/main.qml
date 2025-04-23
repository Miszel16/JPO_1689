import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 600
    height: 700
    title: "Monitor Jakości Powietrza"

    ColumnLayout {
        anchors.fill: parent
        spacing: 12
        anchors.margins: 10

        // Pasek statusu
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#f2f2f2"
            radius: 8

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 15

                Label {
                    text: mainWindow.status
                    font.bold: true
                    Layout.fillWidth: true
                }

                Label {
                    text: "🕒 " + (mainWindow.lastUpdate ? mainWindow.lastUpdate : "Brak aktualizacji")
                    font.pixelSize: 12
                }

                RowLayout {
                    spacing: 5
                    Button {
                        text: "💾 Zapisz teraz"
                        onClicked: mainWindow.saveDataToFileOnly()
                        enabled: !mainWindow.status.includes("Zapisywanie")
                    }

                    Button {
                        text: "📊 Zapisz wszystko"
                        onClicked: mainWindow.saveTestDataForAllStations()
                        enabled: !mainWindow.status.includes("Zapisywanie")
                    }
                }

            }
        }

        // Lista stacji
        Item {
            visible: !mainWindow.sensorViewVisible && !mainWindow.chartViewVisible
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Label {
                    text: "📋 Lista stacji pomiarowych"
                    font.pixelSize: 20
                    font.bold: true
                }

                RowLayout {
                    Layout.fillWidth: true

                    TextField {
                        id: cityFilter
                        Layout.fillWidth: true
                        placeholderText: "Filtruj po nazwie miasta..."
                    }

                    Button {
                        text: "🔍 Szukaj"
                        onClicked: mainWindow.filterStationsByCity(cityFilter.text)
                    }

                    Button {
                        text: "❌ Wyczyść"
                        onClicked: {
                            cityFilter.text = "";
                            mainWindow.filterStationsByCity("");
                        }
                    }
                }

                ListView {
                    id: stationsList
                    model: mainWindow.stations
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 8
                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }

                    delegate: Rectangle {
                        width: stationsList.width - 20
                        height: 100
                        color: "#f0f0f0"
                        border.color: "#ccc"
                        radius: 6

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5

                            Text {
                                text: "📍 " + modelData.stationName
                                font.bold: true
                                font.pixelSize: 16
                            }

                            Text {
                                text: " " + modelData.city
                                font.pixelSize: 14
                            }

                            Text {
                                text: " " + (modelData.address || "Brak adresu")
                                font.pixelSize: 14
                            }

                            Text {
                                text: "🆔 Stacja #" + modelData.stationId
                                font.pixelSize: 12
                                color: "#666"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                mainWindow.setSelectedStation(modelData)
                                mainWindow.fetchSensors(modelData.stationId)
                            }
                        }
                    }
                }
            }
        }

        // Widok czujników
        Item {
            visible: mainWindow.sensorViewVisible && !mainWindow.chartViewVisible
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Button {
                    text: "← Wróć do listy stacji"
                    onClicked: mainWindow.goBackToStationList()
                    icon.source: "qrc:/icons/back.png"
                }

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Stacja: " + (mainWindow.selectedStation.stationName || "")
                        font.pixelSize: 18
                        font.bold: true
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "📌 " + (mainWindow.selectedStation.city || "")
                        font.pixelSize: 14
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 70
                    radius: 8
                    color: {
                        const level = mainWindow.airQualityIndex.stIndexLevel
                        if (level === "Bardzo dobry") return "#2ecc71"
                        if (level === "Dobry") return "#27ae60"
                        if (level === "Umiarkowany") return "#f1c40f"
                        if (level === "Dostateczny") return "#e67e22"
                        if (level === "Zły") return "#e74c3c"
                        if (level === "Bardzo zły") return "#c0392b"
                        return "#bdc3c7"
                    }

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 4

                        Text {
                            text: "Indeks jakości powietrza"
                            color: "white"
                            font.pixelSize: 16
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Text {
                            text: mainWindow.airQualityIndex.stIndexLevel || "Brak danych"
                            color: "white"
                            font.pixelSize: 22
                            font.bold: true
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Text {
                            text: "Aktualizacja: " + (mainWindow.airQualityIndex.stCalcDate ? mainWindow.airQualityIndex.stCalcDate.substring(0, 16).replace("T", " ") : "Brak danych")
                            color: "white"
                            font.pixelSize: 12
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }

                ListView {
                    id: sensorsList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: mainWindow.sensors
                    clip: true
                    spacing: 8
                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }

                    delegate: Rectangle {
                        width: sensorsList.width - 20
                        height: 120
                        color: "#ffffff"
                        border.color: "#ddd"
                        radius: 6

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 6

                            Text {
                                text: "🧪 " + modelData.paramName
                                font.bold: true
                                font.pixelSize: 16
                            }

                            Text {
                                text: "Wzór: " + modelData.paramFormula
                                font.pixelSize: 14
                            }

                            RowLayout {
                                Text {
                                    text: "Wartość: "
                                    font.pixelSize: 14
                                }

                                Text {
                                    text: typeof modelData.lastValue === 'number' ?
                                          modelData.lastValue.toString() :
                                          (modelData.lastValue || "Ładowanie...")
                                    font.bold: true
                                    font.pixelSize: 14
                                }

                                Text {
                                    visible: modelData.lastDate !== undefined
                                    text: modelData.lastDate ? "(" + modelData.lastDate.substring(0, 16).replace("T", " ") + ")" : ""
                                    font.pixelSize: 12
                                    color: "#666"
                                }
                            }

                            Text {
                                text: "📊 Kliknij, aby zobaczyć wykres"
                                color: "#3498db"
                                font.pixelSize: 12
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                mainWindow.openSensorChart(modelData)
                            }
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                }
            }
        }

        // Widok wykresu
        Item {
            visible: mainWindow.chartViewVisible
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Button {
                    text: "← Wróć do czujników"
                    onClicked: mainWindow.goBackToSensorList()
                }

                RowLayout {
                    Layout.fillWidth: true

                    Label {
                        text: "Wykres: " + (mainWindow.selectedSensor.paramName || "")
                        font.pixelSize: 18
                        font.bold: true
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "Formula: " + (mainWindow.selectedSensor.paramFormula || "")
                        font.pixelSize: 14
                    }
                }

                ComboBox {
                    id: timeRange
                    model: ["6 godzin", "12 godzin", "24 godziny", "48 godzin", "7 dni"]
                    currentIndex: 2
                    Layout.preferredWidth: 150

                    onCurrentIndexChanged: {
                        chartCanvas.requestPaint()
                    }
                }

                Canvas {
                    id: chartCanvas
                    Layout.fillWidth: true
                    height: 350

                    onPaint: {
                        const ctx = getContext("2d")
                        ctx.clearRect(0, 0, width, height)

                        const values = chartBox.filteredValues()
                        if (!values || values.length < 2) {
                            ctx.fillStyle = "#666"
                            ctx.font = "16px sans-serif"
                            ctx.fillText("Brak danych do wykresu", width/2 - 80, height/2)
                            return
                        }

                        const margin = 50
                        const chartW = width - margin * 2
                        const chartH = height - margin * 2

                        // Znajdź min i max wartości
                        const valueArray = values.map(v => v.value)
                        const min = Math.min(...valueArray)
                        const max = Math.max(...valueArray)

                        // Zaokrąglij min i max do ładnych wartości
                        const padding = (max - min) * 0.1 || 0.5 // 10% padding lub 0.5 jeśli min==max
                        const yMin = Math.floor((min - padding) * 10) / 10
                        const yMax = Math.ceil((max + padding) * 10) / 10

                        // Rysuj osie
                        ctx.strokeStyle = "#ccc"
                        ctx.fillStyle = "#333"
                        ctx.lineWidth = 1
                        ctx.font = "12px sans-serif"

                        // Oś Y
                        ctx.beginPath()
                        ctx.moveTo(margin, margin)
                        ctx.lineTo(margin, height - margin)
                        ctx.stroke()

                        // Oś X
                        ctx.beginPath()
                        ctx.moveTo(margin, height - margin)
                        ctx.lineTo(width - margin, height - margin)
                        ctx.stroke()

                        // Siatka pozioma i etykiety osi Y
                        const yStep = (yMax - yMin) / 5
                        for (let i = 0; i <= 5; i++) {
                            const yVal = yMin + i * yStep
                            const y = height - margin - i * chartH / 5

                            ctx.beginPath()
                            ctx.moveTo(margin, y)
                            ctx.lineTo(width - margin, y)
                            ctx.strokeStyle = "#eee"
                            ctx.stroke()

                            ctx.fillStyle = "#333"
                            ctx.fillText(yVal.toFixed(2), 5, y + 4)
                        }

                        // Etykiety osi X (daty)
                        const timeLabels = 6 // liczba etykiet czasu
                        for (let i = 0; i <= timeLabels; i++) {
                            const idx = Math.floor(i * (values.length - 1) / timeLabels)
                            if (idx >= values.length) continue

                            const x = margin + idx * chartW / (values.length - 1)
                            const date = new Date(values[idx].date.replace(" ", "T"))

                            ctx.beginPath()
                            ctx.moveTo(x, height - margin)
                            ctx.lineTo(x, height - margin + 5)
                            ctx.strokeStyle = "#333"
                            ctx.stroke()

                            // Sformatuj datę
                            const label = date.toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'})

                            ctx.save()
                            ctx.translate(x, height - margin + 15)
                            ctx.rotate(Math.PI / 4)
                            ctx.fillText(label, 0, 0)
                            ctx.restore()
                        }

                        // Rysuj linię danych
                        ctx.beginPath()
                        values.forEach((val, i) => {
                            const x = margin + i * chartW / (values.length - 1)
                            const y = height - margin - ((val.value - yMin) / (yMax - yMin)) * chartH

                            if (i === 0) ctx.moveTo(x, y)
                            else ctx.lineTo(x, y)
                        })

                        ctx.strokeStyle = "#2980b9"
                        ctx.lineWidth = 2
                        ctx.stroke()

                        // Obszar pod linią
                        ctx.lineTo(margin + chartW, height - margin)
                        ctx.lineTo(margin, height - margin)
                        ctx.closePath()
                        ctx.fillStyle = "rgba(52, 152, 219, 0.2)"
                        ctx.fill()

                        // Rysuj punkty danych
                        values.forEach((val, i) => {
                            const x = margin + i * chartW / (values.length - 1)
                            const y = height - margin - ((val.value - yMin) / (yMax - yMin)) * chartH

                            ctx.beginPath()
                            ctx.arc(x, y, 3, 0, 2 * Math.PI)
                            ctx.fillStyle = "#2980b9"
                            ctx.fill()
                        })

                        // Tytuł wykresu
                        ctx.fillStyle = "#333"
                        ctx.font = "14px sans-serif"
                        ctx.fillText(mainWindow.selectedSensor.paramName + " - " + timeRange.currentText, width/2 - 100, 20)
                    }

                    Connections {
                        target: mainWindow
                        function onSensorMeasurementsChanged() {
                            chartCanvas.requestPaint()
                        }
                    }
                }

                // Statystyki
                Rectangle {
                    Layout.fillWidth: true
                    height: 120
                    color: "#f8f8f8"
                    radius: 8
                    border.color: "#ddd"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        Label {
                            text: "📊 Statystyki dla okresu: " + timeRange.currentText
                            font.bold: true
                            font.pixelSize: 14
                        }

                        GridLayout {
                            columns: 2
                            Layout.fillWidth: true

                            Text {
                                text: "📉 Min: "
                                font.bold: true
                            }

                            Text {
                                text: chartBox.stats ?
                                      chartBox.stats.minValue + " (" + formatDate(chartBox.stats.minDate) + ")" :
                                      "Brak danych"
                            }

                            Text {
                                text: "📈 Max: "
                                font.bold: true
                            }

                            Text {
                                text: chartBox.stats ?
                                      chartBox.stats.maxValue + " (" + formatDate(chartBox.stats.maxDate) + ")" :
                                      "Brak danych"
                            }

                            Text {
                                text: "📊 Średnia: "
                                font.bold: true
                            }

                            Text {
                                text: chartBox.stats ? chartBox.stats.avgValue : "Brak danych"
                            }
                        }
                    }
                }
            }
        }
    }

    // Funkcja formatująca datę
    function formatDate(dateStr) {
        if (!dateStr) return "Brak danych";
        const date = new Date(dateStr.replace(" ", "T"));
        return date.toLocaleString([], {
            day: '2-digit',
            month: '2-digit',
            hour: '2-digit',
            minute: '2-digit'
        });
    }

    // Obiekt do obsługi danych wykresu
    QtObject {
        id: chartBox

        property var filteredValues: function () {
            const all = mainWindow.sensorMeasurements;
            if (!all || all.length === 0) return [];

            const now = new Date();
            const cutoff = new Date();

            switch (timeRange.currentText) {
                case "6 godzin": cutoff.setHours(now.getHours() - 6); break;
                case "12 godzin": cutoff.setHours(now.getHours() - 12); break;
                case "24 godziny": cutoff.setHours(now.getHours() - 24); break;
                case "48 godzin": cutoff.setHours(now.getHours() - 48); break;
                case "7 dni": cutoff.setDate(now.getDate() - 7); break;
                default: cutoff.setHours(now.getHours() - 24); break;
            }

            return all.filter(e => {
                if (!e.date) return false;
                return new Date(e.date.replace(" ", "T")) > cutoff;
            });
        }

        readonly property var stats: {
            const data = filteredValues();
            if (!data || data.length === 0) return null;

            let min = data[0], max = data[0], sum = 0;

            for (let i = 0; i < data.length; i++) {
                const v = data[i];
                if (v.value < min.value) min = v;
                if (v.value > max.value) max = v;
                sum += v.value;
            }

            return {
                minValue: min.value.toFixed(2),
                maxValue: max.value.toFixed(2),
                avgValue: (sum / data.length).toFixed(2),
                minDate: min.date,
                maxDate: max.date
            };
        }
    }
}
