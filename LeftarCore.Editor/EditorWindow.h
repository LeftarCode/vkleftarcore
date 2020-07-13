#pragma once
#include "VulkanEditorRenderer.h"
#include <QtWidgets/qwidget.h>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QPlainTextEdit;
class QLCDNumber;
QT_END_NAMESPACE

class EditorWindow : public QWidget {
	Q_OBJECT

public:
	explicit EditorWindow(VulkanWindow* w);

private:
	VulkanWindow* m_window;
	QTabWidget* m_infoTab;
	QPlainTextEdit* m_info;
	QLCDNumber* m_number;
};
