//
// Created by X-ray on 07/30/22.
//

#pragma once
#ifndef GTA_BASE_TEXTUREMANAGER_HPP
#define GTA_BASE_TEXTUREMANAGER_HPP
#include <filesystem>
#include <memory>
#include <robin_hood.h>
#include <d3d11.h>
#include "../misc/common.hpp"
#undef LoadImage

namespace gta_base::d3d {
    class TextureManager {
    public:
      struct Texture {
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        ID3D11ShaderResourceView* texture = nullptr;
      };
    public:
      TextureManager();
      ~TextureManager();

      Texture Get(const std::filesystem::path& path) {
        auto texture_path = common::GetTextureDir() / path;

        auto key = textures_.find(path);

        if (key != textures_.end())
          return key->second;

        Texture tex_tmp;
        if (LoadImage(path.string(), &tex_tmp.texture, &tex_tmp.width, &tex_tmp.height)) {
          textures_[path] = tex_tmp;

          return tex_tmp;
        } else {
          return tex_tmp;
        }
      }

    private:
      robin_hood::unordered_map<std::filesystem::path, Texture> textures_;

    private:
      static bool LoadImage(const std::string& filename, ID3D11ShaderResourceView** out_srv, std::uint32_t* out_width, std::uint32_t* out_height);
    };
    inline TextureManager* kTEXTURE_MANAGER{};
  }
#endif //GTA_BASE_TEXTUREMANAGER_HPP