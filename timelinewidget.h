#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QList<QRectF> m_videoClips;
    int m_clipHeight;
    int m_clipSpacing;
    bool m_dragging;
    QPointF m_dragStartPosition;
    QPointF m_dragOffset;
    int m_dragIndex;
};

#endif // TIMELINEWIDGET_H
