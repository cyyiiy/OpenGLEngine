#include "assetManager.h"
#include <Utils/stringUtils.h>
#include <iostream>

std::unordered_map<std::string, IAssetManagerType*> AssetManager::typedAssetManagers;
std::unordered_map<AssetUtils::AssetKey, std::weak_ptr<IAsset>> AssetManager::transversalAssetMap;

std::optional<std::string> AssetManager::currentLoadingGroup;
std::unordered_map<std::string, std::vector<AssetUtils::AssetKey>> AssetManager::assetGroups;


void AssetManager::PrintAssetsNumber()
{
    // Get the assets grouped by type and compute the total number of assets
    std::map<std::string, std::vector<AssetUtils::AssetKey>> grouped_assets = GetAssetsGroupedByType();
    const uint32_t total_assets = uint32_t(transversalAssetMap.size());
    
    // Print the number of assets for each type
    std::cout << "==================== ASSET MANAGER - NUMBER OF ASSETS ====================\n";
    for (const auto& [type_name, asset_keys] : grouped_assets)
    {
        const uint32_t type_number = uint32_t(asset_keys.size());
        const float type_percent = static_cast<float>(type_number) / static_cast<float>(total_assets) * 100.0f;
        
        const std::string type_name_str = StringUtils::SanitizeString("\"" + type_name + "\":", 23);
        const std::string type_number_str = StringUtils::SanitizeString(std::to_string(type_number), 3, true);
        std::cout << " > Assets " << type_name_str << " " << type_number_str << " assets (" << StringUtils::FormatFloat(type_percent, 1) << "% of total)\n";
    }
    std::cout << "Total number of assets: " << total_assets << "\n";
    std::cout << "==========================================================================\n";
}

void AssetManager::PrintAllAssets()
{
    // Get the assets grouped by type
    std::map<std::string, std::vector<AssetUtils::AssetKey>> grouped_assets = GetAssetsGroupedByType();
    
    // Iterate on all asset types
    bool firstType = true;
    std::cout << "==================== ASSET MANAGER - PRINT ALL ASSETS ====================\n";
    for (const auto& [type_name, asset_keys] : grouped_assets)
    {
        // Print the asset type "title"
        if (firstType)
            firstType = false;
        else
            std::cout << "----------\n";
        
        const uint32_t type_number = uint32_t(asset_keys.size());
        std::cout << " > Assets \"" << type_name << "\" (" << type_number << " assets):\n";
        
        // Iterate on all assets of the current type
        for (const AssetUtils::AssetKey& asset_key : asset_keys)
        {
            // Sanitize asset name
            std::string asset_name = asset_key.assetName;
            if (std::filesystem::exists(asset_name))
            {
                asset_name = std::filesystem::path(asset_name).stem().string();
            }
            
            // Get the asset use count
            const uint32_t asset_use_count = transversalAssetMap.at(asset_key).use_count() - 1;
            
            // Get the asset memory size and if it has been loaded from a .cyasset
            const IAsset& asset = *transversalAssetMap.at(asset_key).lock();
            const uint64_t asset_memory = asset.getAssetMemorySize();
            const uint64_t asset_gpu = asset.getAssetGpuSize();
            const bool load_cyasset = asset.getCyassetSourcePath().has_value();
            
            // Convert the asset information into sanitized strings
            const std::string asset_name_str = StringUtils::SanitizeString("\"" + asset_name + "\":", 33);
            const std::string asset_use_count_str = StringUtils::SanitizeString(std::to_string(asset_use_count), 2, true);
            const std::string asset_memory_str = StringUtils::SanitizeString(StringUtils::FormatOctet(asset_memory), 9, true);
            const std::string asset_gpu_str = StringUtils::SanitizeString(StringUtils::FormatOctet(asset_gpu), 9, true);
            
            // Print the asset information
            std::cout << "    > " << asset_name_str << " "
                      << "Use count: " << asset_use_count_str << " | "
                      << "Memory usage: " << asset_memory_str << " | "
                      << "GPU usage: " << asset_gpu_str << " | "
                      << "Loaded from .cyasset: " << StringUtils::BoolToString(load_cyasset) << "\n";
        }
    }
    std::cout << "==========================================================================\n";
}

void AssetManager::PrintAssetGroups()
{
    // Prepare a map for the information of the asset groups
    struct AssetGroupInfo
    {
        uint32_t assets_number;
        uint64_t assets_memory;
        uint64_t assets_gpu;
    };
    std::map<std::string, AssetGroupInfo> asset_groups_info;
    
    // Prepare a vector with all asset keys to keep only the assets that are not part of an asset group
    std::vector<AssetUtils::AssetKey> global_assets;
    global_assets.reserve(transversalAssetMap.size());
    for (auto& [asset_key, asset_interface] : transversalAssetMap)
    {
        global_assets.push_back(asset_key);
    }
    
    // Iterate over the asset groups and compute the information
    for  (auto& [group_name, asset_keys] : assetGroups)
    {
        const uint32_t assets_number = uint32_t(asset_keys.size());
        uint64_t assets_memory = 0;
        uint64_t assets_gpu = 0;
        
        for (auto& asset_key : asset_keys)
        {
            assets_memory += transversalAssetMap.at(asset_key).lock()->getAssetMemorySize();
            assets_gpu += transversalAssetMap.at(asset_key).lock()->getAssetGpuSize();
            
            // Remove the asset from the global assets list
            auto iter = std::find(global_assets.begin(), global_assets.end(), asset_key);
            if (iter != global_assets.end()) global_assets.erase(iter);
        }
        
        asset_groups_info.emplace(group_name, AssetGroupInfo{ assets_number, assets_memory, assets_gpu });
    }
    
    // Iterate over the remaining assets in "global_assets" (the assets that are not part of an asset group)
    uint64_t global_assets_memory = 0;
    uint64_t global_assets_gpu = 0;
    for (auto& asset_key : global_assets)
    {
        global_assets_memory += transversalAssetMap.at(asset_key).lock()->getAssetMemorySize();
        global_assets_gpu += transversalAssetMap.at(asset_key).lock()->getAssetGpuSize();
    }
    
    
    // Print the results
    std::cout << "=================== ASSET MANAGER - PRINT ASSETS GROUPS ===================\n";
    for (auto& [group_name, group_info] : asset_groups_info)
    {
        std::cout << " > Assets " << StringUtils::SanitizeString("\"" + group_name + "\":", 40) << " "
                  << StringUtils::SanitizeString(std::to_string(group_info.assets_number), 3, true) << " assets | "
                  << "Memory: " << StringUtils::SanitizeString(StringUtils::FormatOctet(group_info.assets_memory), 9, true) << " | " 
                  << "GPU: " << StringUtils::SanitizeString(StringUtils::FormatOctet(group_info.assets_gpu), 9, true) << "\n";
    }
    std::cout << " > Assets \"global\" (not part of a asset group):    " 
              << StringUtils::SanitizeString(std::to_string(global_assets.size()), 3, true) << " assets | "
              << "Memory: " << StringUtils::SanitizeString(StringUtils::FormatOctet(global_assets_memory), 9, true) << " | " 
              << "GPU: " << StringUtils::SanitizeString(StringUtils::FormatOctet(global_assets_gpu), 9, true) << "\n";
    std::cout << "==========================================================================\n";
    
    
    // Note: This function could also be used to find the assets that share an asset group
}

void AssetManager::PrintMemoriesUsage()
{
    struct MemoriesUsage
    {
        uint64_t memory_usage;
        uint64_t gpu_usage;
    };
    
    // Prepare the memory usage variables
    MemoriesUsage total_memories_usage{ 0, 0 };
    std::map<std::string, MemoriesUsage> type_memories_usage;
    
    // Get the assets grouped by type and iterate on each type
    std::map<std::string, std::vector<AssetUtils::AssetKey>> grouped_assets = GetAssetsGroupedByType();
    for (const auto& [type_name, asset_keys] : grouped_assets)
    {
        // Initialize the memory usage of the iterating type
        type_memories_usage.emplace(type_name, MemoriesUsage{ 0, 0 });
        
        // Cumulate the memory size of each asset of the iterating type
        for (const AssetUtils::AssetKey& asset_key : asset_keys)
        {
            const uint64_t asset_memory_usage = transversalAssetMap.at(asset_key).lock()->getAssetMemorySize();
            const uint64_t asset_gpu_usage = transversalAssetMap.at(asset_key).lock()->getAssetGpuSize();
            
            type_memories_usage.at(type_name).memory_usage += asset_memory_usage;
            type_memories_usage.at(type_name).gpu_usage += asset_gpu_usage;
            total_memories_usage.memory_usage += asset_memory_usage;
            total_memories_usage.gpu_usage += asset_gpu_usage;
        }
    }
    
    
    // Print the results
    std::cout << "===================== ASSET MANAGER - MEMORIES USAGE =====================\n";
    for (const auto& [type_name, memories_usage] : type_memories_usage)
    {
        const float memory_percent = static_cast<float>(memories_usage.memory_usage) / static_cast<float>(total_memories_usage.memory_usage) * 100.0f;
        const float gpu_percent = static_cast<float>(memories_usage.gpu_usage) / static_cast<float>(total_memories_usage.gpu_usage) * 100.0f;
        
        const std::string type_name_str = StringUtils::SanitizeString("\"" + type_name + "\":", 23);
        const std::string memory_usage_str = StringUtils::SanitizeString(StringUtils::FormatOctet(memories_usage.memory_usage), 9, true);
        const std::string memory_percent_str = StringUtils::SanitizeString(StringUtils::FormatFloat(memory_percent, 2), 5, true);
        const std::string gpu_usage_str = StringUtils::SanitizeString(StringUtils::FormatOctet(memories_usage.gpu_usage), 9, true);
        
        std::cout << " > Assets " << type_name_str << ": "
                  << "Memory: " << memory_usage_str << " (" << memory_percent_str << "% of total) | "
                  << "GPU: " << gpu_usage_str << " (" << StringUtils::FormatFloat(gpu_percent, 2) << "% of total)\n";
    }
    std::cout << "Total assets memory usage: " << StringUtils::FormatOctet(total_memories_usage.memory_usage) << "\n";
    std::cout << "Total assets GPU usage: " << StringUtils::FormatOctet(total_memories_usage.gpu_usage) << "\n";
    std::cout << "==========================================================================\n";
    
    
    // Note: This function could also be used to compute the top n biggest asset of each type
}


std::map<std::string, std::vector<AssetUtils::AssetKey>> AssetManager::GetAssetsGroupedByType()
{
    // Prepare the return map
    std::map<std::string, std::vector<AssetUtils::AssetKey>> grouped;

    // Insert all assets in the map, sorted in vectors per type
    for (const auto& [key, asset_ptr] : transversalAssetMap)
    {
        grouped[key.typeName].push_back(key);
    }

    // Sort each vector of the map so the assets are sorted alphabetically
    for (auto& [type_name, keys] : grouped)
    {
        std::sort(keys.begin(), keys.end(), [](const AssetUtils::AssetKey& a, const AssetUtils::AssetKey& b)
        {
            return a.assetName < b.assetName;
        });
    }

    return grouped;
}
