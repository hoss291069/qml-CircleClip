#ifndef CIRCLECLIP_H
#define CIRCLECLIP_H

#include <QQuickItem>

class CircleClip : public QQuickItem
{
public:
    CircleClip();

signals:

protected:
    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData * pp);

};

#endif // CIRCLECLIP_H
