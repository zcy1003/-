#include "mainwindow.h"
#include "QGraphicsRectItem"

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
{
    view = new QGraphicsView(this);
    scene = new QGraphicsScene();

    view->setScene(scene);

    // 绘制时间轴线
  QPen pen;
    pen.setColor(Qt::black);
    scene->addLine(0, 0, 100, 0, pen);

    // 添加视频资源矩形
  QGraphicsRectItem *item = new QGraphicsRectItem(10, 0, 20, 20);
    item->setFlag(QGraphicsItem::ItemIsMovable);
  //connect(item, &QGraphicsRectItem::boundingRectChanged,this, &TimelineWidget::onItemMoved);
    scene->addItem(item);
}

void TimelineWidget::onItemMoved(QGraphicsItem *item){
    // 更新图形位置
    // 更新时长度等
}

void TimelineWidget::paintEvent(QPaintEvent *event)
{

}

