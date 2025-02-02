#ifndef COLLAPSIBLESIDEBAR_H
#define COLLAPSIBLESIDEBAR_H

#include <QFrame>

class QPushButton;
class QVBoxLayout;

/**
 * @brief La clase CollapsibleSidebar encapsula la funcionalidad
 * de un sidebar colapsable con animación.
 */
class CollapsibleSidebar : public QFrame
{
    Q_OBJECT
public:

    explicit CollapsibleSidebar(QWidget *parent = nullptr,
                                int expandedWidth = 200,
                                int collapsedWidth = 50);

    /// Permite agregar widgets al layout interno del sidebar.
    void addWidget(QWidget *widget);

    /// Agrega un stretch al layout interno (útil para alinear elementos).
    void addStretch(int stretch = 1);

private slots:
    /// Slot que se ejecuta al pulsar el botón toggle.
    void toggle();

private:
    int m_expandedWidth;
    int m_collapsedWidth;
    bool m_isExpanded;       ///< Estado actual: true = expandido, false = colapsado.
    QPushButton *m_toggleButton;
    QVBoxLayout *m_layout;

    void enterEvent(QEnterEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // COLLAPSIBLESIDEBAR_H
