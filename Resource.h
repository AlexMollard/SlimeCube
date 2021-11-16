#pragma once
class Resource
{
	template <class T> friend class ResourceManager;
public:
	const std::string& GetResourceFileName() const { return ResourceFileName; }

	Resource(const std::string& resourcefilename, void* args)
	{
		// exit with an error if filename is empty 
		if (resourcefilename.empty()) Log::Error("Empty filename not allowed");

		ResourceFileName = resourcefilename;
	}

	virtual ~Resource() { }

protected:
	Resource(const Resource& object) { }
	Resource& operator=(const Resource& object) { return *this; }

	// resource filename 
	std::string ResourceFileName;

	Resource() {}
};

