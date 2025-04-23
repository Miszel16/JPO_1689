Monitor Jakości Powietrza – README
==================================

📌 Opis
-------
Aplikacja desktopowa do monitorowania jakości powietrza w Polsce, oparta na danych z API GIOŚ (https://api.gios.gov.pl). Umożliwia pobieranie danych ze stacji pomiarowych, wyświetlanie wykresów pomiarów czujników, analizę statystyk oraz automatyczny zapis danych w tle.

🛠 Technologia
--------------
- Qt 5/6 (QML + C++)
- JSON API (GIOŚ)
- Wątkowość (QThread)
- Obsługa offline (lokalne zapisywanie danych)
- Własny system diagnostyki i logowania

📁 Struktura projektu
---------------------
- `main.cpp`, `main.qml` – punkt startowy aplikacji, GUI
- `mainwindow.h/.cpp` – logika główna aplikacji, integracja QML z C++
- `dataloaderworker.h/.cpp` – pobieranie danych w osobnym wątku
- `storage.h/.cpp` – zapisywanie i odczytywanie danych z plików JSON
- `analysis.h/.cpp` – analiza statystyczna pomiarów (min, max, średnia)
- `qml.qrc` – zasoby QML
- `Doxyfile` – konfiguracja dokumentacji Doxygen (jeśli potrzebna)

📦 Funkcje aplikacji
---------------------
- Przegląd listy stacji pomiarowych z możliwością filtrowania po mieście
- Szczegóły czujników i wykresy wartości w czasie
- Indeks jakości powietrza z kolorowym oznaczeniem poziomu
- Tryb offline (automatyczne wczytywanie danych z `data/data.json`)
- Automatyczny zapis danych co godzinę
- Ręczny zapis danych z poziomu GUI

🚀 Jak uruchomić
----------------
1. Skonfiguruj projekt w Qt Creator (QML + C++)
2. Upewnij się, że masz połączenie z Internetem (dla pierwszego uruchomienia)
3. Zbuduj i uruchom projekt

📂 Dane i zapis
--------------
- Dane zapisywane są do katalogu `data/`:
  - `data/data.json` – główny plik z listą stacji, czujników i indeksów
  - `data/sensor_XXX.json` – dane pomiarowe dla konkretnego czujnika

📈 Wykres
---------
- Interaktywny wykres w QML Canvas
- Możliwość wyboru zakresu czasu (6h, 12h, 24h, 48h, 7 dni)
- Obliczane statystyki: min, max, średnia

📄 Licencja
-----------
Projekt edukacyjny – bez ograniczeń w wykorzystaniu, z zachowaniem informacji o źródle API (GIOŚ).
