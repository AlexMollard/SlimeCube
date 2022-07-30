#pragma once

#include "ResourceManager.h"
#include "pch.h"

template <class T>
class ResourceMap
{
  private:
    /////////////////////////////////////////////////////////
    // find in all the map the value requested
    bool IsValNonUnique(const std::string& filename)
    {
        // if duplicates are allowed , then return alwasy true
        if (Duplicates)
            return true;

        // else , check if element by value is already present
        // if it is found, then rturn treu, else exit with false
        std::unordered_map<std::string, T*>::iterator it = Map.begin();
        while (it != Map.end()) {
            if ((it->second->GetResourceFileName() == filename))
                return false;

            ++it;
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////
    // private data
    std::string Name;

    // m_name for this resource mapper
    int Verbose;

    // flag for debugging messages
    int Duplicates;

    // allows or disallwos duplicated filenames for resources
    CResourceManager* ResourceManager;

    // attached resource manager
    std::unordered_map<std::string, T*> Map;

    // resource mapper
    // copy constructor and = operator are kept
  private

    ResourceMap(const ResourceMap&){};

    ResourceMap& operator=(const ResourceMap&) { return *this; }

  public:
    //////////////////////////////////////////////////////////////////////////////////////
    // adds a new element
    T* Add(const std::string& resourcename, const std::string& filename, void* args = 0)
    {
        if (ResourceManager == NULL)
            Log::Error("DataBase cannot be NULL (5)");

        if (filename.empty())
            Log::Error("%s : filename cannot be null", Name.c_str());

        if (resourcename.empty())
            Log::Error("%s : resourcename cannot be null", Name.c_str());

        std::string ResourceName = CStringFormatter::TrimAndLower(resourcename);

        // looks in the hashmap to see if the
        // resource is already loaded
        std::unordered_map<std::string, T*>::iterator it = Map.find(ResourceName);

        if (it == Map.end()) {
            std::string FileName = CStringFormatter::TrimAndLower(filename);

            // if duplicates flag is set to true , duplicated mapped values
            // are allowed, if duplicates flas is set to false, duplicates won't be allowed
            if (IsValNonUnique(FileName)) {
                T* resource = ResourceManager->Load(FileName, args);

                // allocate new resource using the raii paradigm
                Map.insert(std::pair<std::string, T*>(ResourceName, resource));
                return resource;
            } else {
                // if we get here and duplicates flag is set to false
                // the filename id duplicated
                Log::Error("Filename m_name %s must be unique\n", FileName.c_str());
            }
        }

        // if we get here means that resource m_name is duplicated
        Log::Error("Resource m_name %s must be unique\n", ResourceName.c_str());
        return nullptr;
    }

    /////////////////////////////////////////////////////////
    // delete element using resourcename
    bool Remove(const std::string& resourcename)
    {
        if (ResourceManager == NULL)
            Log::Error("DataBase cannot be NULL (4)");

        if (resourcename.empty())
            Log::Error("%s : resourcename cannot be null", Name.c_str());

        std::string ResourceName = CStringFormatter::TrimAndLower(resourcename);

        if (Verbose)
            Log::Trace("%-64s: Removal proposal for : %s\n", Name.c_str(), ResourceName.c_str());

        // do we have this item ?
        std::unordered_map<std::string, T*>::iterator it = Map.find(ResourceName);

        // yes, delete element, since it is a reference counted pointer,
        // the reference count will be decreased
        if (it != Map.end()) {
            // save resource m_name
            std::string filename = (*it).second->GetResourceFileName();

            // erase from this map
            Map.erase(it);

            // check if it is unique and erase it eventually
            ResourceManager->Unload(filename);
            return true;
        }

        // if we get here , node couldn't be found
        // so , exit with an error
        Log::Error("%s : couldn't delete %s\n", Name.c_str(), ResourceName.c_str());
        return false;
    }

    //////////////////////////////////////////////////////////
    // clear all elements from map
    void Clear()
    {
        std::unordered_map<std::string, T*>::iterator it = Map.begin();

        // walk trhough all the map
        while (it != Map.end()) {
            // save resource m_name
            std::string filename = (*it).second->GetResourceFileName();

            // clear from this map
            it = Map.erase(it);

            // check if it is unique and erase it eventually
            ResourceManager->Unload(filename);
        }
    }

    //////////////////////////////////////////////////////////
    // dummps database content to a
    string std::string

    Dump()
    {
        if (ResourceManager == NULL)
            Log::Error("DataBase cannot be NULL (3)");

        std::string str = CStringFormatter::Format("\nDumping database %s\n\n", Name.c_str());

        for (std::unordered_map<std::string, T*>::iterator it = Map.begin(); it != Map.end(); ++it) {
            str += CStringFormatter::Format(
                "resourcename : %s , %s\n", (*it).first.c_str(), (*it).second->GetResourceFileName().c_str());
        }

        return str;
    }

    /////////////////////////////////////////////////////////
    // getters
    /////////////////////////////////////////////////////////
    // gets arrays m_name
    const std::string& GetName() const { return Name; }

    const int Size() const { return Map.size(); }

    //////////////////////////////////////////////////////////
    // gets const reference to resource manager const
    CResourceManager* GetResourceManager() { return ResourceManager; }

    /////////////////////////////////////////////////////////
    // gets element using resourcename, you should use this
    // as a debug feature or to get shared pointer and later
    // use it , using it in a section where performance is
    // needed might slow down things a bit
    T* Get(const std::string& resourcename)
    {
        if (ResourceManager == NULL)
            Log::Error("DataBase cannot be NULL (2)");

        if (resourcename.empty())
            Log::Error("%s : resourcename cannot be null", Name.c_str());

        std::string ResourceName = CStringFormatter::TrimAndLower(resourcename);
        std::unordered_map<std::string, T*>::iterator it;

        if (Verbose) {
            Log::Trace("%-64s: %s\n", Name.c_str(), CStringFormatter::Format("Looking for %s", ResourceName.c_str()).c_str());
        }

        // do we have this item ?
        it = Map.find(ResourceName);

        // yes, return pointer to element
        if (it != Map.end())
            return it->second;

        // if we get here , node couldn't be found thus , exit with a throw
        Log::Error("%s : couldn't find %s", Name.c_str(), ResourceName.c_str());

        // this point is never reached in case of failure
        return nullptr;
    }

    /////////////////////////////////////////////////////////
    // setters
    void AllowDuplicates() { Duplicates = true; }

    void DisallowDuplicates() { Duplicates = false; }

    void SetVerbose() { Verbose = true; }

    void SetQuiet() { Verbose = false; }

    ////////////////////////////////////////////////////////////
    // initialise resource mapper
    void Initialise(const std::string& name, ResourceManager* resourcemanager, bool verbose, bool duplicates)
    {
        if (resourcemanager == NULL)
            Log::Error("DataBase cannot be NULL 1");

        if (name.empty())
            Log::Error("Array m_name cannot be null");

        Name = name;

        // normalized m_name
        std::string ResourceManager = resourcemanager;

        // copy manager pointer
        // setting up verbose or quiet mode
        Verbose = verbose;

        // setting up allowing or disallowing duplicates
        Duplicates = duplicates;

        // emit debug info
        if (Verbose) {
            if (Duplicates)
                Log::Trace("%-64s: Allows duplicates\n", Name.c_str());
            else if (!Duplicates)
                Log::Trace("%-64s: Disallows duplicates\n", Name.c_str());
        }
    }

    /////////////////////////////////////////////////////////
    // ctor / dtor
    ResourceMap()
    {
        Verbose = -1;    // undetermined state
        Duplicates = -1; // undetermined state

        ResourceManager = NULL;
        // no resource manager assigned
    }

    ~ResourceMap()
    {
        if (Verbose)
            Log::Trace("%-64s: Releasing\n", Name.c_str());

        Clear();

        // remove elements if unique
    }
};