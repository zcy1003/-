#include "widget.h"

#include <QApplication>

#include "QGraphicsScene"
#include "QGraphicsItem"
#include "QGraphicsView"
#include "QHBoxLayout"
#include "timelinewidget.h"
#include <QGraphicsSceneMouseEvent>


/*class VideoSegmentItem : public QGraphicsRectItem {
public:
    VideoSegmentItem(int startTime, int duration, QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(parent), m_startTime(startTime), m_duration(duration) {
        setRect(0, 0, duration, 50); // 设置矩形大小
        setBrush(Qt::blue); // 设置矩形颜色
    }

    int getStartTime() const { return m_startTime; }
    int getDuration() const { return m_duration; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        m_dragStartPosition = event->pos(); // 记录拖动开始位置
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override {
        QPointF delta = event->pos() - m_dragStartPosition; // 计算鼠标移动的距离
        moveBy(delta.x(), 0); // 在水平方向上移动视频片段
        m_dragStartPosition = event->pos(); // 更新拖动开始位置
    }

private:
    int m_startTime;
    int m_duration;
    QPointF m_dragStartPosition;
};

class TimelineView : public QGraphicsView {
public:
    TimelineView(QWidget* parent = nullptr) : QGraphicsView(parent) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setScene(new QGraphicsScene(this));
    }

    void addVideoSegment(int startTime, int duration) {
        VideoSegmentItem* segmentItem = new VideoSegmentItem(startTime, duration);
        segmentItem->setPos(startTime, 0); // 设置视频片段位置
        scene()->addItem(segmentItem); // 添加视频片段到场景中
    }
};*/

#include "customtableview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();

    //TimelineWidget w;
    //w.show();


    /*TimelineView timelineView;
    timelineView.resize(800, 200);
    timelineView.show();

    // 添加几个视频片段到时间轴中
    timelineView.addVideoSegment(0, 100);
    timelineView.addVideoSegment(150, 120);
    timelineView.addVideoSegment(300, 80);*/

    CustomTableView tableView;
    tableView.show();

    return a.exec();
}
