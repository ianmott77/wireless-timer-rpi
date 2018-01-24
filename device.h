#ifndef DEVICE_H
#define DEVICE_H
#include <QObject>


class Device: public QObject
{
    Q_OBJECT
public:
    Device(int, int);
    Device();
public slots:
    virtual int getID();
    virtual int getPosition();
    virtual void setID(int);
    virtual void setPosition(int);
private:
    int ID;
    int position;
};

#endif // DEVICE_H
