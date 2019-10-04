#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include "keyemitter.h"

KeyEmitter::KeyEmitter()
{
}

KeyEmitter::~KeyEmitter()
{

}

void KeyEmitter::emitKey(Qt::Key key)
{
    QObject* receiver = QGuiApplication::focusObject();
    if(!receiver)
    {
        return;
    }
    QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, QKeySequence(key).toString());
    QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
    QCoreApplication::sendEvent(receiver, &pressEvent);
    QCoreApplication::sendEvent(receiver, &releaseEvent);
}
