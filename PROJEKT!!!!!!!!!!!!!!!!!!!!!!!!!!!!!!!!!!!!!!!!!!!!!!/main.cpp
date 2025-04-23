#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Ustaw nazwę aplikacji
    app.setApplicationName("Monitor Jakości Powietrza");
    app.setOrganizationName("JPO");

    // Utwórz folder na dane, jeśli nie istnieje
    QDir dataDir;
    if (!dataDir.exists("data")) {
        dataDir.mkpath("data");
        qDebug() << "Utworzono katalog data/";
    }

    // Inicjalizuj silnik QML
    QQmlApplicationEngine engine;

    // Utwórz główne okno aplikacji
    MainWindow mainWindow;

    // Udostępnij obiekt MainWindow w kontekście QML
    engine.rootContext()->setContextProperty("mainWindow", &mainWindow);

    // Załaduj główny plik QML
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    // Połącz sygnał utworzenia obiektu z funkcją sprawdzania błędów
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    // Załaduj plik QML
    engine.load(url);

    // Uruchom pętlę zdarzeń aplikacji
    return app.exec();
}
