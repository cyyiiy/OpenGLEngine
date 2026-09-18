#pragma once
#include "assetManagerType.h"
#include "assetInterface.h"
#include "cyassetDocument.h"
#include <Utils/hashUtils.h>
#include <ServiceLocator/locator.h>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <unordered_map>


namespace AssetUtils
{
    struct AssetKey
    {
        std::string typeName;
        std::string assetName;
        
        bool operator==(const AssetKey& other) const
        {
            return typeName == other.typeName && assetName == other.assetName;
        }
    };
}

template<>
struct std::hash<AssetUtils::AssetKey>
{
    std::size_t operator()(const AssetUtils::AssetKey& assetKey) const noexcept
    {
        std::size_t seed = 0;
        HashUtils::HashCombine(seed, assetKey.typeName);
        HashUtils::HashCombine(seed, assetKey.assetName);
        return seed;
    }
};


/**
 * The global static Asset Manager.
 * Use its static methods to load, unload or get an asset.
 * 
 * The Asset Manager is responsible for all existing assets in the engine.
 */
class AssetManager
{    
private:
    static std::unordered_map<std::string, IAssetManagerType*> typedAssetManagers;
    static std::unordered_map<AssetUtils::AssetKey, std::weak_ptr<IAsset>> transversalAssetMap;
    
    static std::optional<std::string> currentLoadingGroup;
    static std::unordered_map<std::string, std::vector<AssetUtils::AssetKey>> assetGroups;
    
public:
    /**
     * Load an asset of the given type from load parameters and insert it into the asset storage.
     * 
     * @tparam T The type of the asset to load.
     * @param name Custom name of the asset in the storage. Use it to retrieve it anytime.
     * @param loadParams The load parameters for this type of asset.
     * @return A shared pointer of the loaded asset, or nullptr if the loading failed.
     */
    template <class T>
    static std::shared_ptr<T> LoadAsset(const std::string& name, const typename T::LoadParams& loadParams)
    {
        // 1. Manage the loading group and check if the asset already exists in the storage
        std::shared_ptr<T> asset = LoadAssetPartOne<T>(name);
        if (asset)
        {
            // Note: If the returned "asset" is null, it means that the asset doesn't already exist
            return asset;
        }
        
        // 2. Create the asset, insert it in the storage and return it
        asset = LoadAssetPartTwo<T>(name, loadParams);
        if (!asset)
        {
            // Note: An error has already been logged by 'LoadAssetPartTwo'
            return nullptr;
        }
        
        return asset;
    }
    
    template<class T>
    static bool LoadDefaultAsset(const typename T::LoadParams& loadParams)
    {
        std::shared_ptr<T> asset;
        
        try
        {
            asset = T::Create(loadParams);
        }
        catch (std::exception& e)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Failed to create default asset of class '" + T::GetTypeName() + "'. | " + e.what(), LogCategory::Error);
            return false;
        }
        
        if (!asset)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Failed to create default asset of class '" + T::GetTypeName() + "'.", LogCategory::Error);
            return false;
        }
        
        TypedManager<T>().defaultAsset = asset;
        return true;
    }

    /**
     * Load an asset of the given type from a .cyasset file and insert it into the storage.
     * 
     * @tparam T The type of the asset to load.
     * @param name Custom name of the asset in the storage. Use it to retrieve it anytime.
     * @param cyassetPath The path to the cyasset file.
     * @return A shared pointer of the loaded asset, or nullptr if the loading failed.
     */
    template <class T>
    static std::shared_ptr<T> LoadFromCyasset(const std::string& name, const std::filesystem::path& cyassetPath)
    {
        // 1. Manage the loading group and check if the asset already exists in the storage
        std::shared_ptr<T> asset = LoadAssetPartOne<T>(name);
        if (asset)
        {
            // Note: If the returned "asset" is null, it just means that the asset doesn't already exist
            return asset;
        }
        
        // 2. Load the cyasset file (will check if it's of the correct type)
        std::optional<CyassetDocument> cyasset = CyassetDocument::LoadCyassetDocument(cyassetPath, T::GetTypeName());
        if (!cyasset.has_value())
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Failed to load the cyasset document at path \"" + cyassetPath.string() + "\".", LogCategory::Error);
            return nullptr;
        }
        
        // 3. Construct the load params (will throw exceptions if the cyasset isn't correctly formatted)
        typename T::LoadParams load_params;
        try
        {
            load_params = T::ParseCyasset(cyasset.value());
        }
        catch (std::exception& e)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Failed to parse the data of the cyasset document \"" + cyassetPath.string() + "\" for class '" + T::GetTypeName() + "'. | " + e.what(), LogCategory::Error);
            return nullptr;
        }
        
        // 4. Create the asset and insert it in the storage
        asset = LoadAssetPartTwo<T>(name, load_params);
        if (!asset)
        {
            // Note: An error has already been logged by 'LoadAssetPartTwo'
            return nullptr;
        }
        
        // 5. Set the field 'cyassetSourcePath' on the created asset
        IAsset* asset_interface = static_cast<IAsset*>(asset.get());
        asset_interface->cyassetSourcePath = std::filesystem::canonical(cyassetPath);
        
        return asset;
    }

    /**
     * Variant of 'LoadFromCyasset' made to load the dependencies of a higher .cyasset.
     * 
     * Ensure that the returned shared pointer is valid
     * (if the dependency loading failed, this function will throw an exception).
     * 
     * @tparam T The type of the asset to load.
     * @param cyassetPath The path to the cyasset file.
     * @return A shared pointer of the loaded asset.
     * @throw std::runtime_error If there was an error while loading the cyasset dependency.
     */
    template <class T>
    static std::shared_ptr<T> LoadCyassetDependency(const std::filesystem::path& cyassetPath)
    {
        if (!std::filesystem::exists(cyassetPath))
        {
            throw std::runtime_error("Cyasset dependency not found: " + cyassetPath.string());
        }
        
        std::shared_ptr<T> asset = LoadFromCyasset<T>(std::filesystem::canonical(cyassetPath).string(), cyassetPath);
        if (!asset)
        {
            throw std::runtime_error("Failed to load cyasset dependency: " + cyassetPath.string());
        }
        
        return asset;
    }

    /**
     * Unload an asset with the given name from the storage.
     * Note: The asset must be unused for the unload to work.
     * 
     * @tparam T The type of the asset to unload.
     * @param name The custom name of the asset to unload.
     * @return True if the asset was successfully unloaded, false otherwise.
     */
    template <class T>
    static bool UnloadAsset(const std::string& name)
    {
        // 1. Retrieve the manager for the given asset type
        AssetManagerType<T>& typed_manager = TypedManager<T>();
        auto iter = typed_manager.assetMap.find(name);
        
        // 2. Check if the given name exists in the storage
        if (iter == typed_manager.assetMap.end())
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Couldn't find an asset of class '" + T::GetTypeName() + "' with the name \"" + name + "\".", LogCategory::Error);
            return false;
        }
        
        // 3. Check if the asset is still used
        if (iter->second.use_count() > 1)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Couldn't unload the asset \"" + name + "\" of class '" + T::GetTypeName() + "' as it is still used by the game.", LogCategory::Error);
            return false;
        }
        
        // 4. Remove the asset key from the transversal map
        const AssetUtils::AssetKey asset_key{ T::GetTypeName(), name };
        transversalAssetMap.erase(asset_key);
        
        // 5. Erase the last use count of the shared pointer -> Call the asset destructor
        typed_manager.assetMap.erase(iter);
        
        return true;
    }

    /**
     * Retrieve an asset with the given name from the storage.
     * 
     * @tparam T The type of the asset to retrieve.
     * @param name The custom name of the asset to retrieve.
     * @return A shared pointer of the retrieved asset.
     */
    template <class T>
    static std::shared_ptr<T> GetAsset(const std::string& name)
    {
        // 1. Retrieve the manager for the given asset type
        AssetManagerType<T>& typed_manager = TypedManager<T>();
        auto iter = typed_manager.assetMap.find(name);
        
        // 2. Return the asset if it exists in the storage
        if (iter != typed_manager.assetMap.end())
        {
            return iter->second;
        }
        
        // 3. The asset doesn't exist, try to return the default asset
        if (typed_manager.defaultAsset)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Couldn't find an asset of class '" + std::string(typeid(T).name()) + "' with the name \"" + name + "\". Returning the default asset of this class instead.", LogCategory::Warning);
            return typed_manager.defaultAsset;
        }

        Locator::getLog().LogMessage_Category("Asset Manager: Couldn't find an asset of class '" + std::string(typeid(T).name()) + "' with the name \"" + name + "\".", LogCategory::Error);
        return nullptr;
    }


    /**
     * Open a loading group with a custom group name.
     * All assets loaded while the loading group is open will be tagged with the custom group name,
     * allowing a grouped unload of all the assets.
     * 
     * @param groupName The custom group name of the loading group.
     * @return True if the loading group was successfully opened, False otherwise.
     */
    static bool OpenLoadingGroup(const std::string& groupName)
    {
        if (currentLoadingGroup.has_value())
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Could not open loading group \"" + groupName + "\" while another one is already open.", LogCategory::Error);
            return false;
        }
        
        auto [iter, inserted] = assetGroups.try_emplace(groupName);
        if (!inserted)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Could not open loading group \"" + groupName + "\", a loading group with the same name is already registered.", LogCategory::Error);
            // Note: This situation could also happen if a level was reloaded, but it's up to the level system to not
            // reload the assets of the current level, when only its entities and components need to be recreated
            return false;
        }
        
        currentLoadingGroup.emplace(groupName);
        return true;
    }

    /**
     * Close the currently opened loading group.
     * 
     * @return True if the loading group was successfully closed, False otherwise.
     */
    static bool CloseLoadingGroup()
    {
        if (!currentLoadingGroup.has_value())
        {
            Locator::getLog().LogMessage_Category("Asset Manager: There is no currently opened loading group, therefore it couldn't be closed.", LogCategory::Warning);
            return false;
        }
        
        currentLoadingGroup.reset();
        return true;
    }

    /**
     * Try to unload all assets tagged with the given group name.
     * 
     * Note: If assets are still used, they won't be unloaded.
     * 
     * @param groupName The custom group name of the loading group.
     */
    static void TryUnloadAssetsOfGroup(const std::string& groupName)
    {
        // 1. Check if the given group name exists
        auto iter = assetGroups.find(groupName);
        if (iter == assetGroups.end())
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Could not unload assets of group \"" + groupName + "\", this group name isn't registered.", LogCategory::Warning);
            return;
        }
        
        // 2. Get the vector of assets registered under the given loading group
        std::vector<AssetUtils::AssetKey>& asset_group = iter->second;
        
        // 3. Iterate the vector in reverse to unload the higher level assets first
        for (auto asset_iter = asset_group.rbegin(); asset_iter != asset_group.rend(); ++asset_iter)
        {
            AssetUtils::AssetKey& asset_key = *asset_iter;
            const bool unload_success = typedAssetManagers.at(asset_key.typeName)->tryUnloadAsset(asset_key.assetName);
            
            // 4. If unload was successful, remove the asset key from the transversal map
            if (unload_success)
            {
                transversalAssetMap.erase(asset_key);
            }
        }
        
        // 5. Remove the vector from the asset groups
        asset_group.clear();
        assetGroups.erase(iter);
    }
    
    
private:
    template <class T>
    static AssetManagerType<T>& TypedManager()
    {
        static AssetManagerType<T> typed_asset_manager;
        
        // Static local variable 'registered' allow the lambda to be executed only once for each component class
        static bool registered = []
        {
            typedAssetManagers.emplace(T::GetTypeName(), &typed_asset_manager);
            return true;
        }();
        
        (void)registered; // Tell the compiler that it's normal to keep 'registered' unused
        
        return typed_asset_manager;
    }
    
    /** Internal part of the asset loading process that manages the loading group and check for the name being already used. */
    template <class T>
    static std::shared_ptr<T> LoadAssetPartOne(const std::string& name)
    {
        // Check if the asset already exists (custom name already registered)
        AssetManagerType<T>& typed_manager = TypedManager<T>();
        auto iter = typed_manager.assetMap.find(name);
        
        if (iter == typed_manager.assetMap.end())
        {
            // The asset doesn't already exist, return null so the asset loading pipeline can create it
            return nullptr;
        }
        
        // If a loading group is currently opened, add the asset key to the loading group
        const AssetUtils::AssetKey asset_key{ T::GetTypeName(), name };
        if (currentLoadingGroup.has_value())
        {
            assetGroups.at(currentLoadingGroup.value()).push_back(asset_key);
        }
        
        // Return the existing asset that have been found
        return iter->second;
    }
    
    /** Internal part of the asset loading process that manages the real creation of the asset and its insertion in the maps. */
    template <class T>
    static std::shared_ptr<T> LoadAssetPartTwo(const std::string& name, const typename T::LoadParams& loadParams)
    {
        // Create the asset using the static create function (required on all asset classes)
        std::shared_ptr<T> asset;

        try
        {
            asset = T::Create(loadParams);
        }
        catch (std::exception& e)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Failed to create the asset \"" + name + "\" of class '" + T::GetTypeName() + "'. | " + e.what(), LogCategory::Error);
            return nullptr;
        }
        
        if (!asset)
        {
            Locator::getLog().LogMessage_Category("Asset Manager: Failed to create the asset \"" + name + "\" of class '" + T::GetTypeName() + "'.", LogCategory::Error);
            return nullptr;
        }
        
        // Insert the created asset into the map of the typed manager
        AssetManagerType<T>& typed_manager = TypedManager<T>();
        typed_manager.assetMap.emplace(name, asset);
        
        // Convert the asset to its interface and insert it in the transversal map
        const AssetUtils::AssetKey asset_key{ T::GetTypeName(), name };
        std::weak_ptr<IAsset> asset_interface = std::static_pointer_cast<IAsset>(asset);
        transversalAssetMap.emplace(asset_key, asset_interface);
        
        // If a loading group is currently opened, add the asset key to the loading group
        if (currentLoadingGroup.has_value())
        {
            assetGroups.at(currentLoadingGroup.value()).push_back(asset_key);
        }
        
        // Return the created asset
        return asset;
    }
    
    
    friend class Engine; // This function must only be called from a safe source
    static void ClearAllAssets()
    {
        // Note: This function is called after all game elements have been destroyed (so we can assume the asset manager is the last owner of the pointers)
        // Because this function is called before OpenGL, audio, etc. resources are destroyed, assets can be properly unload without any crash
        
        for (const std::pair<const std::string, IAssetManagerType*>& pair : typedAssetManagers)
        {
            pair.second->clearAllAssets();
        }
        
        transversalAssetMap.clear();
    }
    
    
// ----- Statistic parts -----
public:
    /** Print the number of assets currently registered in the Asset Manager. */
    static void PrintAssetsNumber();
    
    /** Print all assets currently registered in the Asset Manager. */
    static void PrintAllAssets();
    
    /** Print all currently existing assets groups. */
    static void PrintAssetGroups();
    
    /** Print the memory (RAM & GPU) used by all assets currently registered in the Asset Manager. */
    static void PrintMemoriesUsage();
    
private:
    static std::map<std::string, std::vector<AssetUtils::AssetKey>> GetAssetsGroupedByType();
};