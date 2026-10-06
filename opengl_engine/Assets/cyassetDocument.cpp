#include "cyassetDocument.h"
#include <ServiceLocator/locator.h>

#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


struct CyassetDocument::Impl
{
    json data;
    
    explicit Impl(json _data) : data(std::move(_data)) {}
};


bool CyassetDocument::hasKey(const std::string& key) const
{
    return impl->data.contains(key);
}

std::vector<std::string> CyassetDocument::getKeys() const
{
    std::vector<std::string> keys;
    keys.reserve(impl->data.size());
    
    for (const auto& [key, value] : impl->data.items())
    {
        keys.push_back(key);
    }
    
    return keys;
}

std::string CyassetDocument::getString(const std::string& key) const
{
    return impl->data.at(key).get<std::string>();
}

std::filesystem::path CyassetDocument::getPath(const std::string& key) const
{
    return std::filesystem::path(getString(key));
}

int CyassetDocument::getInt(const std::string& key) const
{
    return impl->data.at(key).get<int>();
}

float CyassetDocument::getFloat(const std::string& key) const
{
    return impl->data.at(key).get<float>();
}

bool CyassetDocument::getBool(const std::string& key) const
{
    return impl->data.at(key).get<bool>();
}

std::optional<CyassetDocument> CyassetDocument::getSubDocument(const std::string& key) const
{
    if (!impl->data.contains(key))
    {
        // Not an error, this key simply don't exist
        return std::nullopt;
    }
    
    if (!impl->data.at(key).is_object())
    {
        Locator::getLog().LogMessage_Category("Cyasset Parsing: The cyasset document contains a type that isn't a sub-document under the key \"" + key + "\".", LogCategory::Warning);
        return std::nullopt;
    }
    
    CyassetDocument sub_document;
    sub_document.impl = std::make_unique<Impl>(impl->data.at(key).get<json>());
    return sub_document;
}


CyassetDocument::CyassetDocument() = default;
CyassetDocument::~CyassetDocument() = default;

CyassetDocument::CyassetDocument(const CyassetDocument& other) :
    impl(std::make_unique<Impl>(*other.impl)) {}

CyassetDocument& CyassetDocument::operator=(const CyassetDocument& other)
{
    if (this == &other) return *this;
    impl = std::make_unique<Impl>(*other.impl);
    return *this;
}

CyassetDocument::CyassetDocument(CyassetDocument&& other) noexcept :
    impl(std::move(other.impl)) {}

CyassetDocument& CyassetDocument::operator=(CyassetDocument&& other) noexcept
{
    if (this == &other) return *this;
    impl = std::move(other.impl);
    return *this;
}


std::optional<CyassetDocument> CyassetDocument::LoadCyassetDocument(
    const std::filesystem::path& cyassetPath, const std::string& expectedTypeName)
{
    const std::string cyasset_path_str = std::filesystem::canonical(cyassetPath).string(); // For the error logs

    // 1. Open the cyasset file
    std::ifstream cyasset_file(cyassetPath);
    if (!cyasset_file.is_open())
    {
        Locator::getLog().LogMessage_Category("Cyasset Loading: Could not open the file \"" + cyasset_path_str + "\"!", LogCategory::Error);
        return std::nullopt;
    }
    
    // 2. Parse the json content of the cyasset file
    json cyasset_data;
    try
    {
        cyasset_data = json::parse(cyasset_file);
    }
    catch (const json::exception& e)
    {
        Locator::getLog().LogMessage_Category("Cyasset Loading: Could not parse the data of the file \"" + cyasset_path_str + "\"! | " + e.what(), LogCategory::Error);
        return std::nullopt;
    }
    
    // 3. Check if the parsed json content is a json object
    if (!cyasset_data.is_object())
    {
        Locator::getLog().LogMessage_Category("Cyasset Loading: Could not parse the data of the file \"" + cyasset_path_str + "\"! | Parsed content is not a json object.", LogCategory::Error);
        return std::nullopt;
    }
    
    // 4. Check if the cyasset data has the required key "asset_type"
    if (!cyasset_data.contains("asset_type"))
    {
        Locator::getLog().LogMessage_Category("Cyasset Loading: The cyasset document doesn't contains the required key \"asset_type\"!", LogCategory::Error);
        return std::nullopt;
    }
    if (!cyasset_data.at("asset_type").is_string())
    {
        Locator::getLog().LogMessage_Category("Cyasset Loading: The cyasset document contains the required key \"asset_type\" but its value isn't a string!", LogCategory::Error);
        return std::nullopt;
    }
    
    // 5. Check if the cyasset is of the correct type
    const std::string type_name = cyasset_data.at("asset_type").get<std::string>();
    if (type_name != expectedTypeName)
    {
        Locator::getLog().LogMessage_Category("Cyasset Loading: The cyasset document is of type \"" + type_name + "\", when the type \"" + expectedTypeName + "\" was required!", LogCategory::Error);
        return std::nullopt;
    }
    
    // 6. Construct the CyassetDocument object and return it
    CyassetDocument document;
    document.impl = std::make_unique<Impl>(cyasset_data);
    return document;
}