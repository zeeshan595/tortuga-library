#include "Shader.h"

namespace Tortuga
{
Shader::Shader(Device *device, std::string vertexPath, std::string fragmentPath)
{
    this->_device = device;

    _vertexShader = CreateShader(_device->GetVirtualDevice(), ReadFile(vertexPath));
    _fragmentShader = CreateShader(_device->GetVirtualDevice(), ReadFile(fragmentPath));

    auto vertexCreateInfo = VkPipelineShaderStageCreateInfo();
    {
        vertexCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexCreateInfo.module = _vertexShader;
        vertexCreateInfo.pName = "main";
    }
    auto fragmentCreateInfo = VkPipelineShaderStageCreateInfo();
    {
        fragmentCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentCreateInfo.module = _fragmentShader;
        fragmentCreateInfo.pName = "main";
    }

    _pipelineShaderInfo = {vertexCreateInfo, fragmentCreateInfo};
}

Shader::~Shader()
{
    vkDestroyShaderModule(_device->GetVirtualDevice(), _vertexShader, nullptr);
    vkDestroyShaderModule(_device->GetVirtualDevice(), _fragmentShader, nullptr);
}

std::vector<char> Shader::ReadFile(const std::string &filename)
{
    std::ifstream file(filename.c_str(), std::ios::ate | std::ios::binary);
    if (!file.is_open())
        Console::Fatal("Failed to open file! {0}", filename.c_str());

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

VkShaderModule Shader::CreateShader(VkDevice device, std::vector<char> code)
{
    auto shaderCreateInfo = VkShaderModuleCreateInfo();
    {
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = code.size();
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
    }
    VkShaderModule shader;
    if (vkCreateShaderModule(device, &shaderCreateInfo, nullptr, &shader) != VK_SUCCESS)
    {
        Console::Fatal("Failed to load shader!");
    }
    return shader;
}
}; // namespace Tortuga