#include "EditorWindow.h"
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qplaintextedit.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qtablewidget.h>

EditorWindow::EditorWindow(VulkanWindow* w) : m_window(w) {
	QWidget* wrapper = QWidget::createWindowContainer(w);

	m_info = new QPlainTextEdit;
	m_info->setReadOnly(true);

	QPushButton* quitButton = new QPushButton(tr("&Quit"));
	quitButton->setFocusPolicy(Qt::NoFocus);

	connect(quitButton, &QPushButton::clicked, qApp, &QCoreApplication::quit);

	QVBoxLayout* layout = new QVBoxLayout;
	m_infoTab = new QTabWidget(this);
	m_infoTab->addTab(m_info, tr("Vulkan Info"));
	layout->addWidget(m_infoTab, 2);
	layout->addWidget(wrapper, 5);
	layout->addWidget(quitButton, 1);
	setLayout(layout);
}
