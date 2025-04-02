#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QPainterPath>

class ShadowDialog : public QWidget {
    Q_OBJECT
public:
    ShadowDialog(QWidget *parent = nullptr) : QWidget(parent) {
        // 无边框设置
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);

        // 阴影参数
        m_radius = 3;          // 圆角半径
        m_offset = QPoint(0, 0); // 阴影偏移
        m_blurRadius = 5;      // 阴影模糊半径
        m_color = QColor(0, 0, 0, 100); // 阴影颜色
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 绘制阴影
        QLinearGradient gradient(rect().topLeft(), rect().bottomRight());
        gradient.setColorAt(0,  m_color);
        gradient.setColorAt(1,  Qt::transparent);
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect().adjusted(-m_blurRadius,  -m_blurRadius, m_blurRadius, m_blurRadius));

        // 绘制圆角主体
        QPainterPath path;
        path.addRoundedRect(rect().adjusted(m_blurRadius,  m_blurRadius, -m_blurRadius, -m_blurRadius), m_radius, m_radius);
        painter.fillPath(path,  Qt::white);
    }

    void mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            m_dragPos = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) {
        if (event->buttons() & Qt::LeftButton) {
            move(event->globalPos() - m_dragPos);
            event->accept();
        }
    }

private:
    int m_radius;
    QPoint m_offset;
    int m_blurRadius;
    QColor m_color;
    QPoint m_dragPos;
};
