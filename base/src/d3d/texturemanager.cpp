//
// Created by X-ray on 07/30/22.
//

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>
#include "texturemanager.hpp"
#include "renderer.hpp"

namespace gta_base::d3d {
    TextureManager::TextureManager() {
      kTEXTURE_MANAGER = this;
    }

    TextureManager::~TextureManager() {
      kTEXTURE_MANAGER = nullptr;
    }

    // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-directx11-users
    bool TextureManager::LoadImage(const std::string& filename, ID3D11ShaderResourceView** out_srv, std::uint32_t* out_width, std::uint32_t* out_height) {
      // Load from disk into a raw RGBA buffer
      int image_width = 0;
      int image_height = 0;
      int image_comp = 0;
      unsigned char* image_data = stbi_load(filename.c_str(), &image_width, &image_height, &image_comp, 4);
      if (image_data == nullptr)
        return false;

      // Create texture
      D3D11_TEXTURE2D_DESC desc;
      ZeroMemory(&desc, sizeof(desc));
      desc.Width = image_width;
      desc.Height = image_height;
      desc.MipLevels = 1;
      desc.ArraySize = 1;
      desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      desc.SampleDesc.Count = 1;
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      desc.CPUAccessFlags = 0;

      ID3D11Texture2D* pTexture = nullptr;
      D3D11_SUBRESOURCE_DATA subResource;
      subResource.pSysMem = image_data;
      subResource.SysMemPitch = desc.Width * image_comp;
      kRENDERER->GetDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

      // Create texture view
      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
      ZeroMemory(&srvDesc, sizeof(srvDesc));
      srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = desc.MipLevels;
      srvDesc.Texture2D.MostDetailedMip = 0;
      kRENDERER->GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
      pTexture->Release();

      *out_width = image_width;
      *out_height = image_height;
      stbi_image_free(image_data);

      return true;
    }
  }