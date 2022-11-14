//
// Created by X-ray on 11/03/22.
//

#include "fipack_reader.hpp"
#include "../../memory/pointers.hpp"
#include "../../fiber/script.hpp"

namespace rage::util {
  using namespace gta_base;

  namespace {
    std::vector<std::string> GetNonDlcMountedDevicesNames() {
      std::vector<std::string> non_dlc_mounted_devices_names;

      std::uint16_t mounted_devices_len = *memory::kPOINTERS->fidevices_len_;
      if (mounted_devices_len)
      {
        auto devices_arr = *(uint64_t*)memory::kPOINTERS->fidevices_;
        uint8_t** current_device_mount_name_ptr = *(unsigned __int8***)memory::kPOINTERS->fidevices_;
        auto device_i = 0;

        while (true)
        {
          non_dlc_mounted_devices_names.push_back(*(const char**)current_device_mount_name_ptr);

          ++device_i;
          current_device_mount_name_ptr += 4;
          if (device_i >= mounted_devices_len)
            break;
        }
      }

      return non_dlc_mounted_devices_names;
    }
  }

  FipackReader::FipackReader(rage::fiPackfile* rpf, std::string mount_name) : rpf_(rpf), mount_name_(std::move(mount_name)) {}

  void FipackReader::ForEachFipackFile(const std::function<std::size_t(FipackReader&)>& cb) {
    // the idea is to reuse existing mount points as much as possible because
    // even when mounting / unmounting properly you'll get file errors
    // and crashes if the rpf file was already mounted

    // iterate the fidevice array which contains devices that are currently mounted
    // the dlc devices are in another array
    const std::vector<std::string> non_dlc_mounted_devices_names = GetNonDlcMountedDevicesNames();

    // for not hanging the game too much
    constexpr std::size_t yield_increment = 80;

    std::size_t i = 1;
    while(memory::kPOINTERS->fipackfile_instances_[i]) {
      rage::fiPackfile* rpf = memory::kPOINTERS->fipackfile_instances_[i];

      // it's hard coded in the binary?
      if (++i >= 3672) {
        break;
      }

      FipackReader rpf_wrapper = {rpf, default_mount_name};

      bool already_mounted = false;
      for (const auto& non_dlc_mounted_device_name : non_dlc_mounted_devices_names) {
        rage::fiDevice* non_dlc_mounted_device = memory::kPOINTERS->FiDeviceGetDevice(non_dlc_mounted_device_name.c_str(), true);

        if (rpf == non_dlc_mounted_device) {
          rpf_wrapper.mount_name_ = non_dlc_mounted_device_name;
          already_mounted = true;
        }
      }

      if (already_mounted) {
        cb(rpf_wrapper);
      } else {
        size_t acc = 0;

        rpf_wrapper.mount_name_ = "memory:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "memory:";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "dlc";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "dlc:";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "dlc:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "dlcpacks:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "common:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "commoncrc:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "update:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "update2:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "platform:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "platformcrc:/";
        acc += cb(rpf_wrapper);

        rpf_wrapper.mount_name_ = "gamecache:/";
        acc += cb(rpf_wrapper);

        // if we got nothing with those mount points for this rpf, mount it
        if (!acc) {
          rpf_wrapper.mount_name_ = default_mount_name;
          rpf->Mount(default_mount_name);

          cb(rpf_wrapper);

          memory::kPOINTERS->FiPackFileUnmount(default_mount_name);
        }
      }

      if (i % yield_increment == 0)
        fiber::Script::GetCurr()->Yield();
    }
  }

  std::vector<std::filesystem::path> FipackReader::GetFilePaths(std::string parent) {
    std::vector<std::filesystem::path> file_paths;
    std::vector<std::string> dirs;

    if (parent.empty())
      parent = mount_name_;

    rage::fiFindData find_data{0};
    std::uint64_t file_h = rpf_->FindFirst(parent.c_str(), &find_data);
    if (file_h != -1)
    {
      do
      {
        std::string fn = std::string(parent.c_str()) + std::string("/") + std::string(find_data.fileName);

        if (find_data.fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          dirs.push_back(fn);
        }
        else
        {
          file_paths.push_back(fn);
        }
      } while (rpf_->FindNext(file_h, &find_data));

      rpf_->FindClose(file_h);
    }

    for (auto& directory : dirs)
    {
      auto files = GetFilePaths(directory);

      file_paths.insert(file_paths.end(), files.begin(), files.end());
    }

    return file_paths;
  }

  void FipackReader::ReadFile(const std::filesystem::path& path, FipackReader::file_contents_cb_t&& cb) {
    if (const auto handle = rpf_->Open(path.string().c_str(), true); handle != -1)
    {
      const auto data_length = rpf_->GetFileLength(handle);
      const auto file_content = std::make_unique<std::uint8_t[]>(data_length);

      rpf_->ReadFull(handle, file_content.get(), data_length);

      cb(file_content, data_length);

      rpf_->Close(handle);
    }
  }

  void FipackReader::ReadXmlFile(const std::filesystem::path& path, FipackReader::xml_file_contents_cb_t cb) {
    ReadFile(path, [&](const std::unique_ptr<std::uint8_t[]>& file_content, const std::int32_t data_size){
      pugi::xml_document doc;
      if (doc.load_buffer(file_content.get(), data_size).status == pugi::xml_parse_status::status_ok) {
        cb(doc);
      }
    });
  }
}