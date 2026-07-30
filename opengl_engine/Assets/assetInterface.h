#pragma once
#include <optional>
#include <vector>
#include <filesystem>


/**
 * Interface for all asset types.
 * 
 * Mainly used for transversal statistics gathering across all assets.
 */
class IAsset
{    
public:
    IAsset() = default;
    virtual ~IAsset() = default;
    
    IAsset(const IAsset&) = delete;
    IAsset(IAsset&&) = delete;
    IAsset& operator=(const IAsset&) = delete;
    IAsset& operator=(IAsset&&) = delete;

    [[nodiscard]] virtual uint64_t getAssetMemorySize() const = 0;

    [[nodiscard]] const std::vector<std::filesystem::path>& getRawAssetPaths() const { return rawAssetPaths; }
    [[nodiscard]] const std::optional<std::filesystem::path>& getCyassetSourcePath() const { return cyassetSourcePath; }
    
    
protected:
    void setRawAssetPaths(const std::vector<std::filesystem::path>& _rawAssetPaths)
    {
        rawAssetPaths = _rawAssetPaths;
    }

private:
    // Note: "cyassetSourcePath" is set by the Asset Manager when creating the object if the asset was loaded from a .cyasset file
    friend class AssetManager;
    std::optional<std::filesystem::path> cyassetSourcePath;
    
    std::vector<std::filesystem::path> rawAssetPaths;
};