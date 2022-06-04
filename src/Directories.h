#pragma once

// Asset Folder Dir (VisualStudio)
#ifndef ASSETS_DIR
#define ASSETS_DIR  "E:/Projects/SlimeCube/Assets"
#endif

namespace Dir
{
	static std::string FromAsset(const char* file)
	{
		std::string dir;
		dir += ASSETS_DIR;
		dir += file;
		return dir;
	}
}
