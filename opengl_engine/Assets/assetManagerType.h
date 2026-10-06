#pragma once
#include "assetInterface.h"
#include "assetTraits.h"
#include <unordered_map>
#include <string>
#include <memory>


/**
 * Interface for all typed asset managers, so the global Asset Manager can call methods on all the typed managers.
 */
class IAssetManagerType
{
public:
    virtual ~IAssetManagerType() = default;
    
    virtual bool tryUnloadAsset(const std::string& name) = 0;
    virtual void clearAllAssets() = 0;
};


/**
 * Typed asset manager.
 * Stores a map of all assets of its type.
 * 
 * @tparam T The asset type of this manager.
 */
template <class T>
class AssetManagerType : public IAssetManagerType
{
    static_assert(std::is_base_of_v<IAsset, T>, "T must be derived from IAsset.");
    static_assert(AssetTraits::HasLoadParams_v<T>, "T must define a nested struct T::LoadParams.");
    static_assert(AssetTraits::HasCreateFunction_v<T>, "T must define a static function `std::shared_ptr<T> Create(const T::LoadParams&)`.");
    static_assert(AssetTraits::HasParseFunction_v<T>, "T must define a static function `T::LoadParams ParseCyasset(const CyassetDocument&)`.");
    static_assert(AssetTraits::HasTypeNameFunction_v<T>, "T must define a static function `std::string GetTypeName()`.");
    
private:
    friend class AssetManager;
    
    std::unordered_map<std::string, std::shared_ptr<T>> assetMap;
    std::shared_ptr<T> defaultAsset{ nullptr };
    
public:
    bool tryUnloadAsset(const std::string& name) override
    {
        auto iter = assetMap.find(name);
        
        if (iter == assetMap.end()) return false;
        if (iter->second.use_count() > 1) return false;
        
        assetMap.erase(iter);
        return true;
    }
    
    void clearAllAssets() override
    {        
        assetMap.clear();
        defaultAsset = nullptr;
    }
};