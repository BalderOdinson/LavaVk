//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_VKCOMMON_H
#define LAVAVK_VKCOMMON_H

#include <vulkan/vulkan.hpp>
#include <string_view>
#include <map>

namespace LavaVk
{
    template <class T>
    using BindingMap = std::map<uint32_t, std::map<uint32_t, T>>;

    inline bool isDepthOnlyFormat(vk::Format format)
    {
        return format == vk::Format::eD16Unorm ||
               format == vk::Format::eD32Sfloat;
    }

    inline bool isDepthStencilFormat(vk::Format format)
    {
        return format == vk::Format::eD16UnormS8Uint ||
               format == vk::Format::eD24UnormS8Uint ||
               format == vk::Format::eD32SfloatS8Uint ||
               isDepthOnlyFormat(format);
    }

    inline bool isDynamicBufferDescriptorType(vk::DescriptorType descriptorType)
    {
        return descriptorType == vk::DescriptorType::eStorageBufferDynamic ||
               descriptorType == vk::DescriptorType::eUniformBufferDynamic;
    }

    inline bool isBufferDescriptorType(vk::DescriptorType descriptorType)
    {
        return descriptorType == vk::DescriptorType::eStorageBuffer ||
               descriptorType == vk::DescriptorType::eUniformBuffer ||
               isDynamicBufferDescriptorType(descriptorType);
    }

    inline int32_t getBitsPerPixel(vk::Format format)
    {
        switch (format)
        {
            case vk::Format::eR4G4UnormPack8:
                return 8;
            case vk::Format::eR4G4B4A4UnormPack16:
            case vk::Format::eB4G4R4A4UnormPack16:
            case vk::Format::eR5G6B5UnormPack16:
            case vk::Format::eB5G6R5UnormPack16:
            case vk::Format::eR5G5B5A1UnormPack16:
            case vk::Format::eB5G5R5A1UnormPack16:
            case vk::Format::eA1R5G5B5UnormPack16:
                return 16;
            case vk::Format::eR8Unorm:
            case vk::Format::eR8Snorm:
            case vk::Format::eR8Uscaled:
            case vk::Format::eR8Sscaled:
            case vk::Format::eR8Uint:
            case vk::Format::eR8Sint:
            case vk::Format::eR8Srgb:
                return 8;
            case vk::Format::eR8G8Unorm:
            case vk::Format::eR8G8Snorm:
            case vk::Format::eR8G8Uscaled:
            case vk::Format::eR8G8Sscaled:
            case vk::Format::eR8G8Uint:
            case vk::Format::eR8G8Sint:
            case vk::Format::eR8G8Srgb:
                return 16;
            case vk::Format::eR8G8B8Unorm:
            case vk::Format::eR8G8B8Snorm:
            case vk::Format::eR8G8B8Uscaled:
            case vk::Format::eR8G8B8Sscaled:
            case vk::Format::eR8G8B8Uint:
            case vk::Format::eR8G8B8Sint:
            case vk::Format::eR8G8B8Srgb:
            case vk::Format::eB8G8R8Unorm:
            case vk::Format::eB8G8R8Snorm:
            case vk::Format::eB8G8R8Uscaled:
            case vk::Format::eB8G8R8Sscaled:
            case vk::Format::eB8G8R8Uint:
            case vk::Format::eB8G8R8Sint:
            case vk::Format::eB8G8R8Srgb:
                return 24;
            case vk::Format::eR8G8B8A8Unorm:
            case vk::Format::eR8G8B8A8Snorm:
            case vk::Format::eR8G8B8A8Uscaled:
            case vk::Format::eR8G8B8A8Sscaled:
            case vk::Format::eR8G8B8A8Uint:
            case vk::Format::eR8G8B8A8Sint:
            case vk::Format::eR8G8B8A8Srgb:
            case vk::Format::eB8G8R8A8Unorm:
            case vk::Format::eB8G8R8A8Snorm:
            case vk::Format::eB8G8R8A8Uscaled:
            case vk::Format::eB8G8R8A8Sscaled:
            case vk::Format::eB8G8R8A8Uint:
            case vk::Format::eB8G8R8A8Sint:
            case vk::Format::eB8G8R8A8Srgb:
            case vk::Format::eA8B8G8R8UnormPack32:
            case vk::Format::eA8B8G8R8SnormPack32:
            case vk::Format::eA8B8G8R8UscaledPack32:
            case vk::Format::eA8B8G8R8SscaledPack32:
            case vk::Format::eA8B8G8R8UintPack32:
            case vk::Format::eA8B8G8R8SintPack32:
            case vk::Format::eA8B8G8R8SrgbPack32:
                return 32;
            case vk::Format::eA2R10G10B10UnormPack32:
            case vk::Format::eA2R10G10B10SnormPack32:
            case vk::Format::eA2R10G10B10UscaledPack32:
            case vk::Format::eA2R10G10B10SscaledPack32:
            case vk::Format::eA2R10G10B10UintPack32:
            case vk::Format::eA2R10G10B10SintPack32:
            case vk::Format::eA2B10G10R10UnormPack32:
            case vk::Format::eA2B10G10R10SnormPack32:
            case vk::Format::eA2B10G10R10UscaledPack32:
            case vk::Format::eA2B10G10R10SscaledPack32:
            case vk::Format::eA2B10G10R10UintPack32:
            case vk::Format::eA2B10G10R10SintPack32:
                return 32;
            case vk::Format::eR16Unorm:
            case vk::Format::eR16Snorm:
            case vk::Format::eR16Uscaled:
            case vk::Format::eR16Sscaled:
            case vk::Format::eR16Uint:
            case vk::Format::eR16Sint:
            case vk::Format::eR16Sfloat:
                return 16;
            case vk::Format::eR16G16Unorm:
            case vk::Format::eR16G16Snorm:
            case vk::Format::eR16G16Uscaled:
            case vk::Format::eR16G16Sscaled:
            case vk::Format::eR16G16Uint:
            case vk::Format::eR16G16Sint:
            case vk::Format::eR16G16Sfloat:
                return 32;
            case vk::Format::eR16G16B16Unorm:
            case vk::Format::eR16G16B16Snorm:
            case vk::Format::eR16G16B16Uscaled:
            case vk::Format::eR16G16B16Sscaled:
            case vk::Format::eR16G16B16Uint:
            case vk::Format::eR16G16B16Sint:
            case vk::Format::eR16G16B16Sfloat:
                return 48;
            case vk::Format::eR16G16B16A16Unorm:
            case vk::Format::eR16G16B16A16Snorm:
            case vk::Format::eR16G16B16A16Uscaled:
            case vk::Format::eR16G16B16A16Sscaled:
            case vk::Format::eR16G16B16A16Uint:
            case vk::Format::eR16G16B16A16Sint:
            case vk::Format::eR16G16B16A16Sfloat:
                return 64;
            case vk::Format::eR32Uint:
            case vk::Format::eR32Sint:
            case vk::Format::eR32Sfloat:
                return 32;
            case vk::Format::eR32G32Uint:
            case vk::Format::eR32G32Sint:
            case vk::Format::eR32G32Sfloat:
                return 64;
            case vk::Format::eR32G32B32Uint:
            case vk::Format::eR32G32B32Sint:
            case vk::Format::eR32G32B32Sfloat:
                return 96;
            case vk::Format::eR32G32B32A32Uint:
            case vk::Format::eR32G32B32A32Sint:
            case vk::Format::eR32G32B32A32Sfloat:
                return 128;
            case vk::Format::eR64Uint:
            case vk::Format::eR64Sint:
            case vk::Format::eR64Sfloat:
                return 64;
            case vk::Format::eR64G64Uint:
            case vk::Format::eR64G64Sint:
            case vk::Format::eR64G64Sfloat:
                return 128;
            case vk::Format::eR64G64B64Uint:
            case vk::Format::eR64G64B64Sint:
            case vk::Format::eR64G64B64Sfloat:
                return 192;
            case vk::Format::eR64G64B64A64Uint:
            case vk::Format::eR64G64B64A64Sint:
            case vk::Format::eR64G64B64A64Sfloat:
                return 256;
            case vk::Format::eB10G11R11UfloatPack32:
                return 32;
            case vk::Format::eE5B9G9R9UfloatPack32:
                return 32;
            case vk::Format::eD16Unorm:
                return 16;
            case vk::Format::eX8D24UnormPack32:
                return 32;
            case vk::Format::eD32Sfloat:
                return 32;
            case vk::Format::eS8Uint:
                return 8;
            case vk::Format::eD16UnormS8Uint:
                return 24;
            case vk::Format::eD24UnormS8Uint:
                return 32;
            case vk::Format::eD32SfloatS8Uint:
                return 40;
            case vk::Format::eUndefined:
            default:
                return -1;
        }
    }

    inline std::string_view convertFormatToString(vk::Format format)
    {
        switch (format)
        {
            case vk::Format::eR4G4UnormPack8:
                return "VK_FORMAT_R4G4_UNORM_PACK8";
            case vk::Format::eR4G4B4A4UnormPack16:
                return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
            case vk::Format::eB4G4R4A4UnormPack16:
                return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
            case vk::Format::eR5G6B5UnormPack16:
                return "VK_FORMAT_R5G6B5_UNORM_PACK16";
            case vk::Format::eB5G6R5UnormPack16:
                return "VK_FORMAT_B5G6R5_UNORM_PACK16";
            case vk::Format::eR5G5B5A1UnormPack16:
                return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
            case vk::Format::eB5G5R5A1UnormPack16:
                return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
            case vk::Format::eA1R5G5B5UnormPack16:
                return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
            case vk::Format::eR8Unorm:
                return "VK_FORMAT_R8_UNORM";
            case vk::Format::eR8Snorm:
                return "VK_FORMAT_R8_SNORM";
            case vk::Format::eR8Uscaled:
                return "VK_FORMAT_R8_USCALED";
            case vk::Format::eR8Sscaled:
                return "VK_FORMAT_R8_SSCALED";
            case vk::Format::eR8Uint:
                return "VK_FORMAT_R8_UINT";
            case vk::Format::eR8Sint:
                return "VK_FORMAT_R8_SINT";
            case vk::Format::eR8Srgb:
                return "VK_FORMAT_R8_SRGB";
            case vk::Format::eR8G8Unorm:
                return "VK_FORMAT_R8G8_UNORM";
            case vk::Format::eR8G8Snorm:
                return "VK_FORMAT_R8G8_SNORM";
            case vk::Format::eR8G8Uscaled:
                return "VK_FORMAT_R8G8_USCALED";
            case vk::Format::eR8G8Sscaled:
                return "VK_FORMAT_R8G8_SSCALED";
            case vk::Format::eR8G8Uint:
                return "VK_FORMAT_R8G8_UINT";
            case vk::Format::eR8G8Sint:
                return "VK_FORMAT_R8G8_SINT";
            case vk::Format::eR8G8Srgb:
                return "VK_FORMAT_R8G8_SRGB";
            case vk::Format::eR8G8B8Unorm:
                return "VK_FORMAT_R8G8B8_UNORM";
            case vk::Format::eR8G8B8Snorm:
                return "VK_FORMAT_R8G8B8_SNORM";
            case vk::Format::eR8G8B8Uscaled:
                return "VK_FORMAT_R8G8B8_USCALED";
            case vk::Format::eR8G8B8Sscaled:
                return "VK_FORMAT_R8G8B8_SSCALED";
            case vk::Format::eR8G8B8Uint:
                return "VK_FORMAT_R8G8B8_UINT";
            case vk::Format::eR8G8B8Sint:
                return "VK_FORMAT_R8G8B8_SINT";
            case vk::Format::eR8G8B8Srgb:
                return "VK_FORMAT_R8G8B8_SRGB";
            case vk::Format::eB8G8R8Unorm:
                return "VK_FORMAT_B8G8R8_UNORM";
            case vk::Format::eB8G8R8Snorm:
                return "VK_FORMAT_B8G8R8_SNORM";
            case vk::Format::eB8G8R8Uscaled:
                return "VK_FORMAT_B8G8R8_USCALED";
            case vk::Format::eB8G8R8Sscaled:
                return "VK_FORMAT_B8G8R8_SSCALED";
            case vk::Format::eB8G8R8Uint:
                return "VK_FORMAT_B8G8R8_UINT";
            case vk::Format::eB8G8R8Sint:
                return "VK_FORMAT_B8G8R8_SINT";
            case vk::Format::eB8G8R8Srgb:
                return "VK_FORMAT_B8G8R8_SRGB";
            case vk::Format::eR8G8B8A8Unorm:
                return "VK_FORMAT_R8G8B8A8_UNORM";
            case vk::Format::eR8G8B8A8Snorm:
                return "VK_FORMAT_R8G8B8A8_SNORM";
            case vk::Format::eR8G8B8A8Uscaled:
                return "VK_FORMAT_R8G8B8A8_USCALED";
            case vk::Format::eR8G8B8A8Sscaled:
                return "VK_FORMAT_R8G8B8A8_SSCALED";
            case vk::Format::eR8G8B8A8Uint:
                return "VK_FORMAT_R8G8B8A8_UINT";
            case vk::Format::eR8G8B8A8Sint:
                return "VK_FORMAT_R8G8B8A8_SINT";
            case vk::Format::eR8G8B8A8Srgb:
                return "VK_FORMAT_R8G8B8A8_SRGB";
            case vk::Format::eB8G8R8A8Unorm:
                return "VK_FORMAT_B8G8R8A8_UNORM";
            case vk::Format::eB8G8R8A8Snorm:
                return "VK_FORMAT_B8G8R8A8_SNORM";
            case vk::Format::eB8G8R8A8Uscaled:
                return "VK_FORMAT_B8G8R8A8_USCALED";
            case vk::Format::eB8G8R8A8Sscaled:
                return "VK_FORMAT_B8G8R8A8_SSCALED";
            case vk::Format::eB8G8R8A8Uint:
                return "VK_FORMAT_B8G8R8A8_UINT";
            case vk::Format::eB8G8R8A8Sint:
                return "VK_FORMAT_B8G8R8A8_SINT";
            case vk::Format::eB8G8R8A8Srgb:
                return "VK_FORMAT_B8G8R8A8_SRGB";
            case vk::Format::eA8B8G8R8UnormPack32:
                return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
            case vk::Format::eA8B8G8R8SnormPack32:
                return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
            case vk::Format::eA8B8G8R8UscaledPack32:
                return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
            case vk::Format::eA8B8G8R8SscaledPack32:
                return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
            case vk::Format::eA8B8G8R8UintPack32:
                return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
            case vk::Format::eA8B8G8R8SintPack32:
                return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
            case vk::Format::eA8B8G8R8SrgbPack32:
                return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
            case vk::Format::eA2R10G10B10UnormPack32:
                return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
            case vk::Format::eA2R10G10B10SnormPack32:
                return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
            case vk::Format::eA2R10G10B10UscaledPack32:
                return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
            case vk::Format::eA2R10G10B10SscaledPack32:
                return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
            case vk::Format::eA2R10G10B10UintPack32:
                return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
            case vk::Format::eA2R10G10B10SintPack32:
                return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
            case vk::Format::eA2B10G10R10UnormPack32:
                return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
            case vk::Format::eA2B10G10R10SnormPack32:
                return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
            case vk::Format::eA2B10G10R10UscaledPack32:
                return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
            case vk::Format::eA2B10G10R10SscaledPack32:
                return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
            case vk::Format::eA2B10G10R10UintPack32:
                return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
            case vk::Format::eA2B10G10R10SintPack32:
                return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
            case vk::Format::eR16Unorm:
                return "VK_FORMAT_R16_UNORM";
            case vk::Format::eR16Snorm:
                return "VK_FORMAT_R16_SNORM";
            case vk::Format::eR16Uscaled:
                return "VK_FORMAT_R16_USCALED";
            case vk::Format::eR16Sscaled:
                return "VK_FORMAT_R16_SSCALED";
            case vk::Format::eR16Uint:
                return "VK_FORMAT_R16_UINT";
            case vk::Format::eR16Sint:
                return "VK_FORMAT_R16_SINT";
            case vk::Format::eR16Sfloat:
                return "VK_FORMAT_R16_SFLOAT";
            case vk::Format::eR16G16Unorm:
                return "VK_FORMAT_R16G16_UNORM";
            case vk::Format::eR16G16Snorm:
                return "VK_FORMAT_R16G16_SNORM";
            case vk::Format::eR16G16Uscaled:
                return "VK_FORMAT_R16G16_USCALED";
            case vk::Format::eR16G16Sscaled:
                return "VK_FORMAT_R16G16_SSCALED";
            case vk::Format::eR16G16Uint:
                return "VK_FORMAT_R16G16_UINT";
            case vk::Format::eR16G16Sint:
                return "VK_FORMAT_R16G16_SINT";
            case vk::Format::eR16G16Sfloat:
                return "VK_FORMAT_R16G16_SFLOAT";
            case vk::Format::eR16G16B16Unorm:
                return "VK_FORMAT_R16G16B16_UNORM";
            case vk::Format::eR16G16B16Snorm:
                return "VK_FORMAT_R16G16B16_SNORM";
            case vk::Format::eR16G16B16Uscaled:
                return "VK_FORMAT_R16G16B16_USCALED";
            case vk::Format::eR16G16B16Sscaled:
                return "VK_FORMAT_R16G16B16_SSCALED";
            case vk::Format::eR16G16B16Uint:
                return "VK_FORMAT_R16G16B16_UINT";
            case vk::Format::eR16G16B16Sint:
                return "VK_FORMAT_R16G16B16_SINT";
            case vk::Format::eR16G16B16Sfloat:
                return "VK_FORMAT_R16G16B16_SFLOAT";
            case vk::Format::eR16G16B16A16Unorm:
                return "VK_FORMAT_R16G16B16A16_UNORM";
            case vk::Format::eR16G16B16A16Snorm:
                return "VK_FORMAT_R16G16B16A16_SNORM";
            case vk::Format::eR16G16B16A16Uscaled:
                return "VK_FORMAT_R16G16B16A16_USCALED";
            case vk::Format::eR16G16B16A16Sscaled:
                return "VK_FORMAT_R16G16B16A16_SSCALED";
            case vk::Format::eR16G16B16A16Uint:
                return "VK_FORMAT_R16G16B16A16_UINT";
            case vk::Format::eR16G16B16A16Sint:
                return "VK_FORMAT_R16G16B16A16_SINT";
            case vk::Format::eR16G16B16A16Sfloat:
                return "VK_FORMAT_R16G16B16A16_SFLOAT";
            case vk::Format::eR32Uint:
                return "VK_FORMAT_R32_UINT";
            case vk::Format::eR32Sint:
                return "VK_FORMAT_R32_SINT";
            case vk::Format::eR32Sfloat:
                return "VK_FORMAT_R32_SFLOAT";
            case vk::Format::eR32G32Uint:
                return "VK_FORMAT_R32G32_UINT";
            case vk::Format::eR32G32Sint:
                return "VK_FORMAT_R32G32_SINT";
            case vk::Format::eR32G32Sfloat:
                return "VK_FORMAT_R32G32_SFLOAT";
            case vk::Format::eR32G32B32Uint:
                return "VK_FORMAT_R32G32B32_UINT";
            case vk::Format::eR32G32B32Sint:
                return "VK_FORMAT_R32G32B32_SINT";
            case vk::Format::eR32G32B32Sfloat:
                return "VK_FORMAT_R32G32B32_SFLOAT";
            case vk::Format::eR32G32B32A32Uint:
                return "VK_FORMAT_R32G32B32A32_UINT";
            case vk::Format::eR32G32B32A32Sint:
                return "VK_FORMAT_R32G32B32A32_SINT";
            case vk::Format::eR32G32B32A32Sfloat:
                return "VK_FORMAT_R32G32B32A32_SFLOAT";
            case vk::Format::eR64Uint:
                return "VK_FORMAT_R64_UINT";
            case vk::Format::eR64Sint:
                return "VK_FORMAT_R64_SINT";
            case vk::Format::eR64Sfloat:
                return "VK_FORMAT_R64_SFLOAT";
            case vk::Format::eR64G64Uint:
                return "VK_FORMAT_R64G64_UINT";
            case vk::Format::eR64G64Sint:
                return "VK_FORMAT_R64G64_SINT";
            case vk::Format::eR64G64Sfloat:
                return "VK_FORMAT_R64G64_SFLOAT";
            case vk::Format::eR64G64B64Uint:
                return "VK_FORMAT_R64G64B64_UINT";
            case vk::Format::eR64G64B64Sint:
                return "VK_FORMAT_R64G64B64_SINT";
            case vk::Format::eR64G64B64Sfloat:
                return "VK_FORMAT_R64G64B64_SFLOAT";
            case vk::Format::eR64G64B64A64Uint:
                return "VK_FORMAT_R64G64B64A64_UINT";
            case vk::Format::eR64G64B64A64Sint:
                return "VK_FORMAT_R64G64B64A64_SINT";
            case vk::Format::eR64G64B64A64Sfloat:
                return "VK_FORMAT_R64G64B64A64_SFLOAT";
            case vk::Format::eB10G11R11UfloatPack32:
                return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
            case vk::Format::eE5B9G9R9UfloatPack32:
                return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
            case vk::Format::eD16Unorm:
                return "VK_FORMAT_D16_UNORM";
            case vk::Format::eX8D24UnormPack32:
                return "VK_FORMAT_X8_D24_UNORM_PACK32";
            case vk::Format::eD32Sfloat:
                return "VK_FORMAT_D32_SFLOAT";
            case vk::Format::eS8Uint:
                return "VK_FORMAT_S8_UINT";
            case vk::Format::eD16UnormS8Uint:
                return "VK_FORMAT_D16_UNORM_S8_UINT";
            case vk::Format::eD24UnormS8Uint:
                return "VK_FORMAT_D24_UNORM_S8_UINT";
            case vk::Format::eD32SfloatS8Uint:
                return "VK_FORMAT_D32_SFLOAT_S8_UINT";
            case vk::Format::eUndefined:
                return "VK_FORMAT_UNDEFINED";
            default:
                return "VK_FORMAT_INVALID";
        }
    }
}

#endif //LAVAVK_VKCOMMON_H
