#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QThread>
#include <QQmlContext>
#include "uicontroller.h"
#include "Connection/SerialMonitor.h"
#include <thread>
#include <csignal>

void startMonitor(){

}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    ConnectionManager manager;
    Arduino  arduino(&manager);
    UIController  controller(&arduino, &engine);
    engine.rootContext()->setContextProperty("Controller", &controller);
    engine.rootContext()->setContextProperty("Arduino", &arduino);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    QObject * qml = engine.rootObjects().at(0);
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    controller.setQML(qml);
    if (engine.rootObjects().isEmpty())
        return -1;

    int r = app.exec();
    return r;
}
