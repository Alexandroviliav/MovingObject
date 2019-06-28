#include "goondistance.h"

GoOnDistance::GoOnDistance(QString str, QObject *parent) : QObject(parent)
{
    strString = str;
    object = enObject::noObject;
    direction = enDirection::noDirection;
    action = enAction::noAction;
    test = true;
}

GoOnDistance::GoOnDistance(const GoOnDistance &goOnDistance):
    object(goOnDistance.object),
    direction(goOnDistance.direction),
    point(goOnDistance.point)
{

}

//Нахождение числовых значений по {}
QPoint GoOnDistance::getPoint()
{
    QByteArray arrStr = strString.toLocal8Bit();
    const char *strChar = arrStr.data();

    for (int i = 0; i < strString.size(); i++)
    {
        if (strChar[i] == endCoor) {
            stateReadCoor = false;
            readY = false;
        }
        if (stateReadCoor == true && readY == true) {
            y = y * pow(10, 1) + int(strChar[i] - 48);
        }
        if (strChar[i] == ',' && stateReadCoor == true) {
            readY = true;
            readX = false;
        }
        if (stateReadCoor == true && readX == true) {
            x = x * pow(10, 1) + int(strChar[i] - 48);
        }

        if (strChar[i] == beginCoor) {
            stateReadCoor = true;
            readX = true;
        }
    }
    point.setX(x);
    point.setY(y);

    setDefaultSettings();

    return point;
}

//Нахождение направления по ключевым словам
int GoOnDistance::getDirection()
{
    //Cимволы - границы слов
    QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;]+");
    //Разбить строку на слова, пропуская пустые
    QStringList list = strString.split(reg, QString::SkipEmptyParts);
    int defineDirectionObject = 0;

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).contains("лево")) {
            checkWords(direction);
//            qDebug() << "Налево";
            direction = enDirection::left;
        }
        if (list.at(i).contains("право")) {
            checkWords(direction);
//            qDebug() << "Направо";
            direction = enDirection::right;
        }
        if (list.at(i).contains("вперед")) {
            checkWords(direction);
//            qDebug() << "Вперед";
            direction = enDirection::forward;
        }
        if (list.at(i).contains("зад")) {
            checkWords(direction);
//            qDebug() << "Назад";
            direction = enDirection::back;
        }
        if (list.at(i).contains("точк")) {
            checkWords(direction);
//            qDebug() << "В точку";
            direction = enDirection::toPoint;
        }
        if (list.at(i).contains("по")) {
//            qDebug() << "По";
            defineDirectionObject = 1;
        }
        if (list.at(i).contains("против")) {
//            qDebug() << "против";
            defineDirectionObject = 2;
        }
        if (list.at(i).contains("часовой")) {
            if (defineDirectionObject == 1) {
                checkWords(direction);
                direction = enDirection::clockwise;
//              qDebug() << "По часовой";
            }
            else if (defineDirectionObject == 2) {
                checkWords(direction);
                direction = enDirection::antiClockwise;
//              qDebug() << "Против часовой";
            }
            else {
                test = false;
            }
        }
    }
    return direction;
}

//Нахождение объекта по ключевым словам
int GoOnDistance::getObject()
{
    QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;]+");
    QStringList list = strString.split(reg, QString::SkipEmptyParts);

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).contains("источник")) {
            checkWords(object);
//            qDebug() << "Источник";
            object = enObject::source;
        }
        if (list.at(i).contains("приемник")) {
            checkWords(object);
//            qDebug() << "Приемник";
            object = enObject::receiver;
        }
        if (list.at(i).contains("объект")) {
            checkWords(object);
//            qDebug() << "Объект";
            object = enObject::objectOfStudy;
        }
    }
    return object;
}

//Нахождение действия по ключевым словам
int GoOnDistance::getAction()
{
    QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;]+");
    QStringList list = strString.split(reg, QString::SkipEmptyParts);

    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).contains("двинуть")) {
            checkWords(action);
            qDebug() << "Сдвинуть";
            action = enAction::shift;
        }
        if (list.at(i).contains("Получить") ||
            list.at(i).contains("получить")) {
            checkWords(action);
            qDebug() << "получить";
            action = enAction::getImage;
        }
        if (list.at(i).contains("Экспозиц") ||
            list.at(i).contains("экспозиц")) {
            checkWords(action);
            qDebug() << "Экспозиция";
            action = enAction::setExposure;
        }
        if (list.at(i).contains("Вкл") ||
            list.at(i).contains("вкл")) {
            checkWords(action);
            qDebug() << "Включить";
            action = enAction::switchOnXray;
        }
        if (list.at(i).contains("Выкл") ||
            list.at(i).contains("выкл")) {
            checkWords(action);
            qDebug() << "Выключить";
            action = enAction::switchOffXray;
        }
        if (list.at(i).contains("Повернуть") ||
            list.at(i).contains("повернуть")) {
            checkWords(action);
            qDebug() << "Повернуть";
            action = enAction::turn;
        }
        if (list.at(i).contains("Подождать") ||
            list.at(i).contains("подождать")) {
            checkWords(action);
            qDebug() << "Подождать";
            action = enAction::wait;
        }
    }
    return action;
}

//Проверка корректности введенных данных
bool GoOnDistance::testData()
{
    //Проверка, все ли необходимые слова пречислены в команде
    if (action == enAction::noAction ||
        (action == enAction::shift &&
        (object == enObject::noObject ||
        direction == enDirection::noDirection ||
        (point.x() == 0 &&
        point.y() == 0)))) {
        test = false;
//        qDebug() << 1;
    }

    //Если движение в точку, то должны быть заполнены обе координаты
    if (direction != enDirection::toPoint &&
            point.y() != 0) {
        test = false;
//        qDebug() << 2;
    }

    //Если не выполняется сдвиг объекта или поворот,
    //то не должны выполнятся выбор направления и объекта
    if ((action == enAction::getImage ||
         action == enAction::setExposure ||
         action == enAction::switchOnXray ||
         action == enAction::switchOffXray ||
         action == enAction::wait) &&
        (direction != enDirection::noDirection ||
         object != enObject::noObject)) {
        test = false;
//        qDebug() << 3;
    }

    //Если вкл - выкл рентгена или получение изображения,
    //то не должно быть введено координат, выбрано объектов
    //перемещения и направление
    if ((action == enAction::switchOnXray ||
        action == enAction::switchOffXray ||
        action == enAction::getImage) &&
        (point.x() != 0 ||
        point.y() != 0 ||
        object != enObject::noObject ||
        direction != enDirection::noDirection)) {
        test = false;
//        qDebug() << 4;
    }

    //Если выполняется поворот, то только объекта
    if (action == enAction::turn &&
        object != enObject::objectOfStudy) {
        test = false;
//        qDebug() << 5;
    }

    //Если экспозиция, то необхордима одна координата
    if (action == enAction::setExposure &&
        point.x() == 0 ||
        point.y() != 0) {
        test = false;
//        qDebug() << 6;
    }
    return test;
}

void GoOnDistance::setDefaultSettings()
{
    bool stateReadCoor = false;
    int x = 0;
    int y = 0;
    bool readX = false;
    bool readY = false;
}

void GoOnDistance::checkWords(int check)
{
    if (check != 0) {
        test = false;
    }

}
