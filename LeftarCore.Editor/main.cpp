#include <QtWidgets/qapplication.h>
#include <QVulkanInstance>
#include <QLoggingCategory>
#include "EditorWindow.h"
#include "VulkanEditorRenderer.h"

Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

	VulkanWindow* window = new VulkanWindow("TechDemo 0.1");
	EditorWindow mainWindow(window);
	mainWindow.resize(1024, 768);
	mainWindow.show();

	lc::RenderEngineCreateInfo createInfo;
	createInfo.engineMode = lc::eEditor;
	createInfo.size = { 1024, 768 };
	createInfo.title = "TechDemo 0.1";

	std::shared_ptr<lc::RenderingEngine> renderer = lc::RenderingEngine::create(createInfo);
	renderer->setRenderSurface(window);

	return app.exec();
}
