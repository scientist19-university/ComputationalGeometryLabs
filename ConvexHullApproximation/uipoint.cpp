#include "uipoint.h"

#include <QCursor>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

#include <QDrag>
#include <QString>


UIPoint::UIPoint(int i_x, int i_y) :
    QGraphicsEllipseItem (0, 0, 5, 5)
{
    setPos(i_x - 2, i_y - 2);

    setBrush(Qt::SolidPattern);
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void UIPoint::mousePressEvent(QGraphicsSceneMouseEvent *i_event)
{
    setCursor(Qt::ClosedHandCursor);
}

void UIPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *i_event)
{
    setPos(i_event->scenePos());
    setPen(QPen(QColor(0, 0, 0, 20)));

    emit positionChanged(scenePos());
}

void UIPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *i_event)
{
    setPen(QPen(Qt::black));
    setCursor(Qt::OpenHandCursor);
}
