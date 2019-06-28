#ifndef GOONDISTANCE_H
#define GOONDISTANCE_H

#include <QObject>
#include <QPoint>
#include <QDebug>
#include <math.h>
#include <QRegExp>
#include <QChar>

class GoOnDistance : public QObject
{
    Q_OBJECT
public:
    explicit GoOnDistance(QString, QObject *parent = nullptr);
    GoOnDistance(const GoOnDistance &);

    QPoint getPoint();
    int getDirection();
    int getObject();
    int getAction();
    bool testData();

private:
    QString strString;

    int object;
    int direction;
    int action;
    QPoint point;

    const QChar beginCoor = '{';
    const QChar endCoor = '}';
    bool stateReadCoor = false;
    int x = 0;
    int y = 0;
    bool readX = false;
    bool readY = false;
    bool test;

    void setDefaultSettings();
    void checkWords(int);

    enum enAction{noAction,
               shift,
               getImage,
               setExposure,
               switchOnXray,
               switchOffXray,
               turn,
               wait};

    enum enDirection{noDirection,
               forward,
               right,
               back,
               left,
               toPoint,
               clockwise,
               antiClockwise};

    enum enObject{noObject,
               source,
               receiver,
               objectOfStudy};

signals:

public slots:
};

#endif // GOONDISTANCE_H
