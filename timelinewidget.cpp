#include "timelinewidget.h"

#include <QPainter>
#include <QMouseEvent>

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent), m_clipHeight(50), m_clipSpacing(10), m_dragging(false), m_dragIndex(-1)
{
    resize(800, 200);  // 设置时间轴控件的大小

    // 添加视频矩形示例
    m_videoClips.append(QRectF(0, 0, 200, m_clipHeight));
    m_videoClips.append(QRectF(200, 50, 300, m_clipHeight));
}

void TimelineWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    // 绘制视频矩形
    painter.save();
    painter.translate(0, 50);

    for (int i = 0; i < m_videoClips.size(); i++) {
        const QRectF& rect = m_videoClips.at(i);
        painter.fillRect(rect, Qt::blue);
    }

    painter.restore();

    // 绘制时间线
    painter.drawLine(0, 0, width(), 0);

    // 绘制时间刻度
    int duration = 60 * 60; // 一小时的秒数
    int bigInterval = 5 * 60; // 5分钟的秒数
    int smallInterval = 60; // 1分钟的秒数
    int numBigTicks = duration / bigInterval;  // 时间线分为几个大段  2024.4.2 16:44--zcy

    for (int i = 0; i <= numBigTicks; i++) {
        int xPos = i * width() / numBigTicks;
        painter.drawLine(xPos, 0, xPos, 10);
        painter.drawText(xPos - 10, 25, QString::number(i * bigInterval / 60));

        if (i < numBigTicks) {
            int numSmallTicks = bigInterval / smallInterval;  // 每个大段中共有一个小段  2024.4.2 16:45--zcy
            int smallTickSpacing = width() / numBigTicks / numSmallTicks;

            for (int j = 1; j < numSmallTicks; j++) {
                int xPosSmall = xPos + j * smallTickSpacing;
                painter.drawLine(xPosSmall, 0, xPosSmall, 5);
            }
        }
    }
}

void TimelineWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 转换鼠标坐标为时间轴控件的坐标系
        QPointF pos = event->pos();
        int offsetY = height() - m_clipHeight / 2;
        pos.setY(pos.y() - offsetY);

        // 判断是否点击在视频矩形内
        for (int i = 0; i < m_videoClips.size(); i++) {
            const QRectF& rect = m_videoClips.at(i);
            if (rect.contains(pos)) {
                m_dragStartPosition = event->pos() - rect.topLeft();
                m_dragging = true;
                m_dragIndex = i; // 记录正在拖动的视频矩形的索引
                break;
            }
        }
    }
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && m_dragIndex != -1) {
        QPointF dragOffset = event->pos() - m_dragStartPosition;

        // 更新正在拖动的视频矩形的位置
        QRectF& rect = m_videoClips[m_dragIndex];
        rect.translate(dragOffset);

        m_dragStartPosition = event->pos();
        update();
    }
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        m_dragIndex = -1; // 重置拖动的视频矩形的索引
    }
}
