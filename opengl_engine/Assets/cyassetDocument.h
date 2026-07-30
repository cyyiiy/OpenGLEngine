#pragma once
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <memory>


/**
 * Public interface of the cyasset json content, using the pimpl idiom.
 * Note that all "get" functions can throw an exception, if asking for a non-existing key or a wrong value type.
 */
class CyassetDocument
{
public:
    bool hasKey(const std::string& key) const;
    std::vector<std::string> getKeys() const;
    
    std::string getString(const std::string& key) const;
    std::filesystem::path getPath(const std::string& key) const;
    
    int getInt(const std::string& key) const;
    float getFloat(const std::string& key) const;
    bool getBool(const std::string& key) const;
    // TODO: Add `getVector3` when integrating this class in the engine with the real `Vector3` type
    
    std::optional<CyassetDocument> getSubDocument(const std::string& key) const;
    
    
    static std::optional<CyassetDocument> LoadCyassetDocument(
        const std::filesystem::path& cyassetPath, const std::string& expectedTypeName);
    
    ~CyassetDocument();
    CyassetDocument(const CyassetDocument& other);
    CyassetDocument& operator=(const CyassetDocument& other);
    CyassetDocument(CyassetDocument&& other) noexcept;
    CyassetDocument& operator=(CyassetDocument&& other) noexcept;
    
private:
    // Private default constructor so it's impossible to create a `CyassetDocument` object without using
    // the static function `LoadCyassetDocument`
    CyassetDocument();
    
    struct Impl;
    std::unique_ptr<Impl> impl;
};