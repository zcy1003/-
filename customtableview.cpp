#include "customtableview.h"
#include "qheaderview.h"
#include "QStandardItem"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QScrollBar>


/*class CustomDelegate : public QStyledItemDelegate
{
public:
    CustomDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);

        if (index.column() == 1) {
            QAbstractItemModel *model = index.model();
            QRect rect = option.rect;

            // 获取矩形的位置
            int row = index.row();
            QRectF rectF = rectList.value(row).rect;

            // 根据行高度调整矩形的高度
            int rowHeight = option.rect.height();
            rectF.setHeight(rowHeight);

            // 绘制矩形
            painter->save();
            painter->setPen(Qt::black);
            painter->drawRect(rectF);
            painter->restore();
        }
    }
};*/

CustomTableView::CustomTableView(QWidget *parent)
    : QTableView{parent}
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setVisible(false);

    //CustomDelegate *delegate = new CustomDelegate(this);
    //setItemDelegateForColumn(1, delegate);

    // 假设 CustomPainter *painter 是您的自定义绘图类实例
    connect(this->horizontalScrollBar(), &QScrollBar::valueChanged,
            this, &CustomTableView::onScrollBarsValueChanged);
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &CustomTableView::onScrollBarsValueChanged);

    // 创建模型   QTableView设置行数与列数是下面这样设置的，setRowCount是QTableWidget设置的，不适应于QTableView  2024.3.29 13:36
    QStandardItemModel* model = new QStandardItemModel(10, 2, this);
    // 设置模型
    setModel(model);

    this->resize(1000,400);

    // 设置列宽
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->resizeSection(0, 200); // 第一列固定为100像素宽

    // 设置默认行高
    verticalHeader()->setDefaultSectionSize(50);

}

void CustomTableView::onScrollBarsValueChanged()
{
    // 假设矩形原本的全局坐标为 rectGlobalPos
    //QPointF rectGlobalPos = ...;  // 这里设置矩形在表格全局坐标系中的位置

    // 获取滚动条的新值
    int horizontalScrollBarValue = this->horizontalScrollBar()->value();
    int verticalScrollBarValue = this->verticalScrollBar()->value();

    // 计算矩形相对于视口的偏移量
    QPointF scrollOffset(horizontalScrollBarValue, verticalScrollBarValue);

    // 计算矩形在屏幕上的新位置
    //QPointF rectScreenPos = rectGlobalPos - scrollOffset;

    // 重绘矩形
    //painter->drawRectangleAt(rectScreenPos);
}

void CustomTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int clickedCol = columnAt(event->x());
        int clickedRow = rowAt(event->y());

        if (clickedCol == 0) {
            startPos = event->pos();
            resizing = true;
            rowIndex = clickedRow;
            colIndex = clickedCol;
            originalWidth = columnWidth(colIndex);
            originalHeight = rowHeight(rowIndex);
        }

        // 判断出鼠标在除第0行之外的第一列上时，就是要移动视频矩形的  2024.4.2 17:20--zcy
        if(clickedRow != 0 && clickedCol == 1)
        {
            // 获取鼠标点击的表格项的索引
            /*QModelIndex clickedIndex = indexAt(event->pos());
            // 获取当前表格项的矩形区域
            selectedRect = visualRect(clickedIndex);
            originalRect = selectedRect;
            dragging = true;*/

            for (const RectInfo& rectInfo : rectList) {
                if(rectInfo.row == clickedRow)
                {
                    dragStartPos = event->pos();
                    originalRect = rectInfo.rect;   // 使用鼠标点击时的行好获取对应的矩形信息  2024.4.3 9:50--zcy
                    selectedRect = rectInfo.rect;
                    dragging = true;
                }
            }
        }

        QTableView::mousePressEvent(event);
    }
}

// 拖动矩形时没有进到这个mouseMoveEvent里面，导致selectedRect矩形的位置没有更新，还有就是更新时要限制一下只绘制更新这一行的矩形  2024.4.2 18:06--zcy
void CustomTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (resizing && event->buttons() & Qt::LeftButton) {
        // 计算新的列宽度或行高度
        int xOffset = event->x() - startPos.x();
        int yOffset = event->y() - startPos.y();

        // 第0列表格的宽度固定住，不然拉伸宽度，只能拉伸高度，要不然拉伸宽度的话该行矩形的坐标、宽高都要随之改变，比较麻烦  2024.4.3 11:21--zcy
        /*if (colIndex >= 0) {
            int newWidth = originalWidth + xOffset;
            //限制拉伸的宽度范围
            if (newWidth > 100 && newWidth < 250) {
                setColumnWidth(colIndex, newWidth);
            }

        }*/
        if(rowIndex >= 0)
        {
            int newHeight = originalHeight  +yOffset;
            if(newHeight > 50 && newHeight < 100)
            {
                // 但是现在先更改了矩形的位置后，然后任意拉伸一行，矩形不会随着拉伸而改变高度  2024.4.3 13:43--zcy
                setRowHeight(rowIndex,newHeight);

                QModelIndex index = model()->index(rowIndex, 1);
                int startResizeRow = 0;
                QRect rect = visualRect(index);
                for (int i = 0; i < rectList.size(); ++i) {
                    if (rectList[i].row == rowIndex) {
                        startResizeRow = i + 1;
                        rectList[i].rect.setHeight(rect.height());
                        break;
                    }
                }
                // 还要注意一点：比如窗口界面足够展示所有的QTableView表格内容时，随意拉伸和拖动都不会有问题，当窗口界面比较小时，出现了滚动条时，即有部分的表格内容被隐藏了，再绘制矩形，拉伸和拖动时绘制的就会有问题，如矩形的部分线段被隐藏或者超出所在行等  2024.4.3 16:37--zcy
                for(int j = startResizeRow; j < rectList.size(); j++)
                {
                    // 这样更改后续行中矩形的y坐标位置不对，需要考虑到上面一行高度变化的影响  2024.4.3 14:28--zcy
                    // 不止要改变y坐标还要改变高度，只改变y坐标不改变其他的，随着拉伸矩形只会越来越小  2024.4.3 15:16--zcy
                    QModelIndex nextIndex = model()->index(j, 1);
                    QRect nextRect = visualRect(nextIndex);
                    // 设置拉伸行的后续行中的矩形时，先获取到上一行中底部坐标y，然后加十后将其设置为当前行中矩形的顶部  2024.4.3 16:21--zcy
                    rectList[j].rect.moveTop(rectList[j-1].rect.bottomLeft().y() + 2);
                    //rectList[j].rect =  QRect(nextRect.left(), nextRect.top() + 5, 200, nextRect.height() - 10);
                    //rectList[j].rect = rectList[j].rect.translated(0,yOffset);   //使用translated会偏移太多了  2024.4.3 15:45--zcy
                    //rectList[j].rect.setHeight(rectList[j].rect.height() + yOffset);
                }
            }
        }

        // 隔行拉伸的时候如何也让矩形保持在表格项内  2024.4.3 11:31--zcy
        /*for (int i = 0; i < rectList.size(); ++i) {
            if (rectList[i].row == rowAt(event->y())) {
                rectList[i].rect.setHeight(rectList[i].rect.height() + yOffset);
                break;
            }
        }*/
    }

    // 要限制一下只能在第一列表格项内进行拖动  2024.4.3 10:08--zcy
    if(dragging && event->buttons() && Qt::LeftButton && (columnAt(event->x()) == 1))
    {
        // 获取当前鼠标位置的偏移量
        int xOffset = event->pos().x() - dragStartPos.x();
        int yOffset = event->pos().y() - dragStartPos.y();

        // 更新被拖动的矩形的位置
        //selectedRect = originalRect.translated(xOffset, yOffset);
        selectedRect = originalRect.translated(xOffset, 0);
        // 检查拖动矩形是否超出表格项边界
        QModelIndex clickedIndex = indexAt(event->pos());
        QRect itemRect = visualRect(clickedIndex);
        if (!itemRect.contains(selectedRect)) {
            // 调整拖动矩形的位置，使其保持在表格项内
            if (selectedRect.left() < itemRect.left()) {
                selectedRect.moveLeft(itemRect.left());
            }
            if (selectedRect.right() > itemRect.right()) {
                selectedRect.moveRight(itemRect.right());
            }
            if (selectedRect.top() < itemRect.top()) {
                selectedRect.moveTop(itemRect.top());
            }
            if (selectedRect.bottom() > itemRect.bottom()) {
                selectedRect.moveBottom(itemRect.bottom());
            }
        }
        // 拖动后要更新一下rectList列表  2024.4.3 10:13--zcy
        for (int i = 0; i < rectList.size(); ++i) {
            if (rectList[i].row == rowAt(event->y())) {
                rectList[i].rect = selectedRect;
                break;
            }
        }


        // 刷新视图
        viewport()->update();
    }
    QTableView::mouseMoveEvent(event);
}

void CustomTableView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        resizing = false;
        // 这里暂时不要把拖动的变量设置为false，因为设置为false的话，进到paintEvent里面会再重新绘制新的每一行的矩形，这会导致刚拖动的矩形又回到(被绘制到)开始的位置   2024.4.3 10:47--zcy
        // 但是这样的话，改变表格项的高度的时候，绘制的矩形就会超出所在表格项的范围，因为没有跟着表格项一起改变  2024.4.3 11:04--zcy
        //dragging = false;
    }
    QTableView::mouseReleaseEvent(event);
}

void CustomTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);

    // 获取第0行第1列的表格项矩形区域
    QModelIndex topLeftIndex = model()->index(0, 1);
    QRect rect = visualRect(topLeftIndex);

    // 在表格项矩形区域内绘制时间线和时间数字
    QPainter painter(viewport());
    painter.setPen(Qt::black);
    int lineWidth = rect.width() / 12; // 每个大段的宽度
    int lineHeight = rect.height(); // 时间线的高度等于表格项的高度
    QFont font = painter.font();
    font.setPixelSize(10); // 设置字体大小
    painter.setFont(font);
    for (int i = 0; i < 12; ++i) {
        int x = rect.left() + i * lineWidth;
        painter.drawLine(x, rect.top(), x, rect.top() + lineHeight);
        for (int j = 1; j < 5; ++j) {
            int subX = x + j * (lineWidth / 5);
            painter.drawLine(subX, rect.top(), subX, rect.top() + lineHeight / 2);
        }
        QString timeText = QString::number(i * 5) + ":00"; // 计算时间文字
        QRect textRect = QRect(x, rect.top() + lineHeight / 2 + 5, 30, 15); // 时间文字的矩形区域
        painter.drawText(textRect, Qt::AlignCenter, timeText); // 绘制时间文字
    }



    if (dragging)
    {
        // 在拖动状态下绘制被拖动的矩形
        /*painter.setBrush(QColor(0, 0, 255, 100));
        painter.drawRect(selectedRect);*/

        painter.setPen(Qt::blue);
        int rowCount = model()->rowCount();
        for (int row = 1; row < rowCount; ++row) {
            painter.setBrush(Qt::transparent);

            // 根据rectList中矩形的信息来绘制每一行的矩形
            for (const RectInfo& rectInfo : rectList) {
                if (rectInfo.row == row) {
                    painter.setBrush(Qt::transparent);
                    painter.drawRect(rectInfo.rect);
                    break;
                }
            }
        }
    }
    else
    {
        rectList.clear();
        // 获取表格项的行数和列数
        painter.setPen(Qt::blue);
        int rowCount = model()->rowCount();
        int columnCount = model()->columnCount();
        // 遍历所有表格项
        for (int row = 1; row < rowCount; ++row) {
            // 获取当前表格项的矩形区域
            QModelIndex index = model()->index(row, 1);
            QRect rect = visualRect(index);

            RectInfo rectInfo;
            rectInfo.rect = QRect(rect.left(), rect.top(), 200, rect.height());
            rectInfo.row = row;
            rectList.append(rectInfo); // 添加到列表中

            painter.setBrush(Qt::transparent);
            painter.drawRect(rectInfo.rect);
        }
    }
}
