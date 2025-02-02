#include "collapsiblesidebar.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>

CollapsibleSidebar::CollapsibleSidebar(QWidget *parent, int expandedWidth, int collapsedWidth)
    : QFrame(parent),
    m_expandedWidth(expandedWidth),
    m_collapsedWidth(collapsedWidth),
    m_isExpanded(true)
{
    setMinimumWidth(m_expandedWidth);
    setMaximumWidth(m_expandedWidth);

    // Creamos el layout vertical.
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(10);

    // Botón para colapsar/expandir.
    m_toggleButton = new QPushButton("☰", this);
    m_toggleButton->setMinimumHeight(30);
    m_toggleButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_layout->addWidget(m_toggleButton, 0);

    // Conectar la señal del botón al slot toggle.
    connect(m_toggleButton, &QPushButton::clicked, this, &CollapsibleSidebar::toggle);
}

void CollapsibleSidebar::addWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
}

void CollapsibleSidebar::addStretch(int stretch)
{
    m_layout->addStretch(stretch);
}

void CollapsibleSidebar::toggle()
{
    QPropertyAnimation *animMin = new QPropertyAnimation(this, "minimumWidth");
    QPropertyAnimation *animMax = new QPropertyAnimation(this, "maximumWidth");

    animMin->setDuration(300);
    animMax->setDuration(300);
    animMin->setEasingCurve(QEasingCurve::InOutQuad);
    animMax->setEasingCurve(QEasingCurve::InOutQuad);

    if (m_isExpanded) {
        animMin->setStartValue(m_expandedWidth);
        animMin->setEndValue(m_collapsedWidth);
        animMax->setStartValue(m_expandedWidth);
        animMax->setEndValue(m_collapsedWidth);

        // Cambia el cursor a uno que sugiera interacción
        setCursor(Qt::PointingHandCursor);
    } else {
        animMin->setStartValue(m_collapsedWidth);
        animMin->setEndValue(m_expandedWidth);
        animMax->setStartValue(m_collapsedWidth);
        animMax->setEndValue(m_expandedWidth);

        // Restablece el cursor al normal
        unsetCursor();
    }

    m_isExpanded = !m_isExpanded;

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    group->addAnimation(animMin);
    group->addAnimation(animMax);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void CollapsibleSidebar::enterEvent(QEnterEvent *event)
{
    if (!m_isExpanded) {  // Cambia el cursor solo cuando está colapsado
        setCursor(Qt::PointingHandCursor);
    }
    QFrame::enterEvent(event);
}

void CollapsibleSidebar::mousePressEvent(QMouseEvent *event) {
    if (!m_isExpanded) {  // Solo reacciona si está colapsado
        toggle();
    }
    QFrame::mousePressEvent(event);
}


