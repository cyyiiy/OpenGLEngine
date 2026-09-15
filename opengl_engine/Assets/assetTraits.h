#pragma once
#include <type_traits>
#include <memory>


class CyassetDocument;

namespace AssetTraits
{
    // ----- Detect if assets classes declare a nested struct T::LoadParams -----

    template <class T, class = void>
    struct HasLoadParams : std::false_type {};

    template <class T>
    struct HasLoadParams<T, std::void_t<typename T::LoadParams>> : std::true_type {};

    template <class T>
    inline constexpr bool HasLoadParams_v = HasLoadParams<T>::value;
    

    // ----- Detect if assets classes declare a static function T::Create with the correct signature -----

    template <class T, class = void>
    struct HasCreateFunction : std::false_type {};

    template <class T>
    struct HasCreateFunction<T, std::void_t<decltype(T::Create(std::declval<const typename T::LoadParams&>()))>>
        : std::is_same<decltype(T::Create(std::declval<const typename T::LoadParams&>())), std::shared_ptr<T>> {};

    template <class T>
    inline constexpr bool HasCreateFunction_v = HasCreateFunction<T>::value;

    
    // ----- Detect if assets classes declare a static function T::GetTypeName with the correct signature -----

    template <class T, class = void>
    struct HasTypeNameFunction : std::false_type {};

    template <class T>
    struct HasTypeNameFunction<T, std::void_t<decltype(T::GetTypeName())>>
        : std::is_same<decltype(T::GetTypeName()), std::string> {};

    template <class T>
    inline constexpr bool HasTypeNameFunction_v = HasTypeNameFunction<T>::value;

    
    // ----- Detect if assets classes declare a static function T::ParseCyasset with the correct signature -----
    
    template <class T, class = void>
    struct HasParseFunction : std::false_type {};
    
    template <class T>
    struct HasParseFunction<T, std::void_t<decltype(T::ParseCyasset(std::declval<const CyassetDocument&>()))>>
        : std::is_same<decltype(T::ParseCyasset(std::declval<const CyassetDocument&>())), typename T::LoadParams> {};
    
    template <class T>
    inline constexpr bool HasParseFunction_v = HasParseFunction<T>::value;
}
