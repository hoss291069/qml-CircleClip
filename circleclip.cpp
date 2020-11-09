#include "circleclip.h"
#include <QSGClipNode>
#include <QSGGeometryNode>
#include <QSGSimpleRectNode>
#define _USE_MATH_DEFINES
#include <math.h>

#define steps 1000
static bool initialized=false;
static QSGGeometry::Point2D qsgUintCircle[steps+1];

#define M_PIF      static_cast<float>(M_PI)
#define M_PIF_2    static_cast<float>(M_PI_2)


CircleClip::CircleClip()
{

    setFlag(ItemHasContents, true);

    if(initialized==false)
    {
        double d=2.0f/steps*M_PIF;
        double t=M_PIF_2;
        for(int i=0;i<=steps;++i,t-=d)
        {
            qsgUintCircle[i].set(cosf(t),sinf(t));
        }
        initialized=true;
    }

    QObject::connect(this,&QQuickItem::widthChanged,this,&QQuickItem::update);
    QObject::connect(this,&QQuickItem::heightChanged,this,&QQuickItem::update);
}


QSGNode* CircleClip::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGTransformNode*  rootNode;
    QSGClipNode *      clipNode;
    QSGGeometry*       geometry;
    QSGSimpleRectNode*  rectNode;



    if (!oldNode) {
        rootNode= new QSGTransformNode();
        clipNode= new QSGClipNode();
        rectNode= new QSGSimpleRectNode();
        rectNode->setColor("brown");
        clipNode->setIsRectangular(false);
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 1000*3+12);
        clipNode->setGeometry(geometry);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        clipNode->setFlag(QSGNode::OwnsGeometry);
        rootNode->appendChildNode(clipNode);
        clipNode->appendChildNode(rectNode);

    } else {

        rootNode = static_cast<QSGTransformNode *>(oldNode);
        clipNode = static_cast<QSGClipNode *>(rootNode->firstChild());
        rectNode = static_cast<QSGSimpleRectNode *>(clipNode->firstChild());
        geometry = clipNode->geometry();
    }

    QMatrix4x4 n{1,0,0,static_cast<float>(width()/2.0),
                 0,-1,0,static_cast<float>(height()/2.0),
                 0,0,1,0,
                 0,0,0,1
                };

    rootNode->setMatrix(n);
    rootNode->markDirty(QSGNode::DirtyMatrix);

    rectNode->setRect(QRect(-width()/2,-height()/2,width(),height()));
    rectNode->markDirty(QSGNode::DirtyGeometry);

    QSGGeometry::Point2D* points = geometry->vertexDataAsPoint2D();

    int r_px=width()>height() ? height()/2.0 : width()/2.0;
    int k=0;
    for(int i=0;i<250;i++,k+=3)
    {
        points[k+0].set(width()/2.0,height()/2.0);
        points[k+1].set(qsgUintCircle[i].x *r_px  , qsgUintCircle[i].y * r_px);
        points[k+2].set(qsgUintCircle[i+1].x *r_px  , qsgUintCircle[i+1].y * r_px);
    }

    for(int i=250;i<500;i++,k+=3)
    {
        points[k+0].set(width()/2.0,-height()/2.0);
        points[k+1].set(qsgUintCircle[i].x *r_px  , qsgUintCircle[i].y * r_px);
        points[k+2].set(qsgUintCircle[i+1].x *r_px  , qsgUintCircle[i+1].y * r_px);
    }

    for(int i=500;i<750;i++,k+=3)
    {
        points[k+0].set(-width()/2.0,-height()/2.0);
        points[k+1].set(qsgUintCircle[i].x *r_px  , qsgUintCircle[i].y * r_px);
        points[k+2].set(qsgUintCircle[i+1].x *r_px  , qsgUintCircle[i+1].y * r_px);
    }

    for(int i=750;i<1000;i++,k+=3)
    {
        points[k+0].set(-width()/2.0,height()/2.0);
        points[k+1].set(qsgUintCircle[i].x *r_px  , qsgUintCircle[i].y * r_px);
        points[k+2].set(qsgUintCircle[i+1].x *r_px  , qsgUintCircle[i+1].y * r_px);
    }

    points[k++].set(width()/2.0,height()/2.0);
    points[k++].set(r_px  ,0);
    points[k++].set(width()/2.0 ,-height()/2.0);

    points[k++].set(width()/2.0,-height()/2.0);
    points[k++].set(0  ,-r_px);
    points[k++].set(-width()/2.0 ,-height()/2.0);

    points[k++].set(-width()/2.0,height()/2.0);
    points[k++].set(0  ,r_px);
    points[k++].set(width()/2.0 ,height()/2.0);

    points[k++].set(-width()/2.0,height()/2.0);
    points[k++].set(-r_px  ,0);
    points[k++].set(-width()/2.0 ,-height()/2.0);

    return rootNode;
}
