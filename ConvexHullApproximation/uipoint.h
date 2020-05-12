#ifndef UIPOINT_H
#define UIPOINT_H

#include <QGraphicsEllipseItem>

class UIPoint : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    UIPoint(int i_x, int i_y);

signals:
    void positionChanged(QPointF i_new_point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *i_event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *i_event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *i_event) override;
};

#endif // UIPOINT_H
