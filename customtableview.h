#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QWidget>
#include <QTableView>

struct RectInfo {
    QRect rect;
    int row;
};

class CustomTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CustomTableView(QWidget *parent = nullptr);

signals:

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent* event) override;

private:
    bool resizing = false;
    int rowIndex = -1;
    int colIndex = -1;
    int originalWidth = 0;
    int originalHeight = 0;
    QPoint startPos;

    QRect selectedRect;
    bool dragging = false;
    QRect originalRect;
    QList<RectInfo> rectList;
    QPoint dragStartPos;

};

#endif // CUSTOMTABLEVIEW_H
