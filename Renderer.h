#pragma once
#include "Entity.h"

class Renderer
{
public:
	static Renderer* GetInstance();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	static void DrawEntity(Entity entity);
private:
	static Renderer* instance;

	Renderer() = default;
	~Renderer() = default;
};