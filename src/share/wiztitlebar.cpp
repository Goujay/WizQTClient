#include "wiztitlebar.h"

#include <QPixmap>
#include <QStyle>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMouseEvent>



CWizTitleBar::CWizTitleBar(QWidget *parent, QWidget* window, QWidget* shadowContainerWidget)
    : QWidget(parent)
    , m_window(window)
    , m_shadowContainerWidget(shadowContainerWidget)
    , m_oldContentsMargin(10, 10, 10, 10)
    , m_canResize(true)
{
    // 不继承父组件的背景色
    setAutoFillBackground(true);

    m_minimize = new QToolButton(this);
    m_maximize = new QToolButton(this);
    m_close = new QToolButton(this);

    // 设置按钮图像的样式
    QPixmap pix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    m_close->setIcon(pix);

    m_maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
    m_maximize->setIcon(m_maxPix);

    pix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    m_minimize->setIcon(pix);

    m_restorePix = style()->standardPixmap(QStyle::SP_TitleBarNormalButton);

    m_minimize->setMinimumHeight(20);
    m_close->setMinimumHeight(20);
    m_maximize->setMinimumHeight(20);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setText("");
    m_window->setWindowTitle("");

    QHBoxLayout *hbox = new QHBoxLayout(this);

    hbox->addWidget(m_titleLabel);
    hbox->addWidget(m_minimize);
    hbox->addWidget(m_maximize);
    hbox->addWidget(m_close);

    hbox->insertStretch(1, 500);
    hbox->setSpacing(0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_maxNormal = false;

    connect(m_close, SIGNAL( clicked() ), m_window, SLOT(close() ) );
    connect(m_minimize, SIGNAL( clicked() ), this, SLOT(showSmall() ) );
    connect(m_maximize, SIGNAL( clicked() ), this, SLOT(showMaxRestore() ) );
    //
}

void CWizTitleBar::showSmall()
{
    m_window->showMinimized();
}

void CWizTitleBar::showMaxRestore()
{
    if (!m_canResize)
        return;
    //
    if (m_maxNormal) {
        //
        m_shadowContainerWidget->setContentsMargins(m_oldContentsMargin);
        m_window->showNormal();
        m_maxNormal = !m_maxNormal;
        m_maximize->setIcon(m_maxPix);
        //
    } else {
        //
        m_oldContentsMargin = m_shadowContainerWidget->contentsMargins();
        m_shadowContainerWidget->setContentsMargins(0, 0, 0, 0);
        m_window->showMaximized();
        m_maxNormal = !m_maxNormal;
        m_maximize->setIcon(m_restorePix);
    }
}

void CWizTitleBar::mousePressEvent(QMouseEvent *me)
{
    m_startPos = me->globalPos();
    m_clickPos = mapTo(m_window, me->pos());
}
void CWizTitleBar::mouseMoveEvent(QMouseEvent *me)
{
    if (m_maxNormal)
        return;
    m_window->move(me->globalPos() - m_clickPos);
}

void CWizTitleBar::mouseDoubleClickEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton)
    {
        showMaxRestore();
    }
}
void CWizTitleBar::setCanResize(bool b)
{
    m_canResize = b;
    //
    m_maximize->setEnabled(b);
    m_minimize->setEnabled(b);
}
void CWizTitleBar::setText(QString title)
{
    m_titleLabel->setText(title);
}

QString CWizTitleBar::text() const
{
    return m_titleLabel->text();
}