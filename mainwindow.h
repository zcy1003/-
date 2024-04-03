#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class TimelineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);

private slots:
    void onItemMoved(QGraphicsItem *item);

private:
    void paintEvent(QPaintEvent *event) override;

    QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
