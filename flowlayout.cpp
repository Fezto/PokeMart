// flowlayout.cpp
#include "flowlayout.h"
#include <QWidget>   // Añadir
#include <QStyle>    // Añadir
#include <QMargins>  // Añadir

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}


FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout() {
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item) {
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const {
    return m_hSpace >= 0 ? m_hSpace : smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const {
    return m_vSpace >= 0 ? m_vSpace : smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const {
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const {
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index) {
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const {
    return Qt::Horizontal;
}

bool FlowLayout::hasHeightForWidth() const {
    return true;
}

int FlowLayout::heightForWidth(int width) const {
    return doLayout(QRect(0, 0, width, 0), true);
}

void FlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const {
    return minimumSize();
}

QSize FlowLayout::minimumSize() const {
    QSize size;
    for (QLayoutItem *item : std::as_const(itemList)) // <-- std::as_const
        size = size.expandedTo(item->minimumSize());

    QMargins margins = contentsMargins(); // <-- Obtener márgenes
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;
    QList<QLayoutItem*> currentRow;  // Almacena los items de la fila actual

    // Define spaceX y spaceY fuera del bucle
    int spaceX = horizontalSpacing();
    int spaceY = verticalSpacing();

    for (QLayoutItem *item : std::as_const(itemList)) {
        QWidget *wid = item->widget();
        if (!wid || !wid->isVisible()) continue;

        int nextX = x + item->sizeHint().width() + spaceX;

        // Si el item no cabe en la fila actual
        if (nextX - spaceX > effectiveRect.right() && !currentRow.isEmpty()) {
            // Centrar la fila actual
            int totalWidth = x - effectiveRect.x() - spaceX;
            int offset = (effectiveRect.width() - totalWidth) / 2;

            // Ajustar posiciones de los items en esta fila
            for (QLayoutItem *rowItem : currentRow) {
                QRect geometry = rowItem->geometry();
                geometry.moveLeft(geometry.x() + offset);
                if (!testOnly) rowItem->setGeometry(geometry);
            }

            currentRow.clear();
            x = effectiveRect.x();
            y += lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        // Agregar el item a la fila actual
        currentRow.append(item);

        if (!testOnly) {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }

    // Centrar la última fila
    if (!currentRow.isEmpty()) {
        int totalWidth = x - effectiveRect.x() - spaceX;
        int offset = (effectiveRect.width() - totalWidth) / 2;

        for (QLayoutItem *rowItem : currentRow) {
            QRect geometry = rowItem->geometry();
            geometry.moveLeft(geometry.x() + offset);
            if (!testOnly) rowItem->setGeometry(geometry);
        }
    }

    return y + lineHeight - rect.y() + bottom;
}


    int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
        QWidget *parent = this->parentWidget();
        if (!parent)
            return -1;
        return parent->style()->pixelMetric(pm, nullptr, parent);
    }
