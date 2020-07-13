#include "../LeftarCore.Graphics.Core/RenderingEngine.h"
#include "../LeftarCore.Graphics.Core/RenderWindow.h"
#include "../LeftarCore.Graphics.Core/Shader.h"
#include "../LeftarCore.Core/Base.h"

using namespace lc;

int main() {
	  lc::Logger::getInstance()->warn("WARN");
	lc::Logger::getInstance()->error("ERROR");

	RenderEngineCreateInfo createInfo;
	createInfo.engineMode = eGame;
	createInfo.size = { 1024, 768 };
	createInfo.title = "TechDemo 0.1";

	std::shared_ptr<RenderingEngine> renderer = RenderingEngine::create(createInfo);
	renderer->setRenderSurface(new RenderWindow(1024, 768, "TechDemo 0.1"));
	renderer->init();

	std::shared_ptr<IRenderSurface> window = renderer->getWindow();

	std::vector<std::shared_ptr<Shader>> shaders;
	shaders.push_back(
			std::shared_ptr<Shader>(new Shader("./res/shaders/compiled/simple.vspv", eVertex)));
	shaders.push_back(
			std::shared_ptr<Shader>(new Shader("./res/shaders/compiled/simple.fspv", eFragment)));

	RenderViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = renderer->getWindow()->getExtent().width;
	viewport.height = renderer->getWindow()->getExtent().height;
	viewport.minDepth = 0;
	viewport.maxDepth = 1.0f;

	RasterizationSettings settings;
	settings.cullMode = CullMode::eBack;
	settings.frontFace = FrontFace::eClockwise;
	settings.primitiveMode = PrimitiveMode::eFillMode;
	settings.primitiveType = PrimitiveType::eTriangle;
	settings.viewport = viewport;

	std::shared_ptr<RenderPass> renderPass(new RenderPass());
	std::shared_ptr<RenderPipeline> pipeline(new RenderPipeline(shaders, renderPass, settings));

	const std::vector<Vertex> vertexBufferData = {
		{ { -0.5f, -0.5f, 0.0f }, { 1, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { 0.5f, -0.5f, 0.0f }, { 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { 0.5f, 0.5f, 0.0f }, { 0, 1 }, { 0, 0, 0 }, { 0, 0, 0 } },
		{ { -0.5f, 0.5f, 0.0f }, { 1, 1 }, { 0, 0, 0 }, { 0, 0, 0 } },
	};

	const std::vector<uint16_t> indexBufferData = { 0, 1, 2, 2, 3, 0 };

	std::shared_ptr<Buffer> vertexBuffer(new Buffer((void*)vertexBufferData.data(), sizeof(Vertex),
																									vertexBufferData.size(),
																									BufferUsage::eVertexBuffer));
	std::shared_ptr<Buffer> indexBuffer(new Buffer((void*)indexBufferData.data(), sizeof(uint16_t),
																								 indexBufferData.size(),
																								 BufferUsage::eIndexBuffer));

	window->initFramebuffers(renderPass);
	renderer->initCommandBuffers(renderPass, pipeline, vertexBuffer, indexBuffer);

	while (window->isOpen()) {
		renderer->draw();
		window->pollEvents();
	}

	return 0;
}