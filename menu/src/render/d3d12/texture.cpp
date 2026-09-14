//
// Created by X-ray on 14/09/2026.
//

#include "texture.hpp"

#include <base-common/logging/logging_macro.hpp>
#include "context.hpp"
#include "../renderer.hpp"

namespace base::menu::render::d3d12 {
  namespace {
    constexpr DXGI_FORMAT kTextureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    constexpr UINT64 kUploadFenceValue = 1;

    /// The upload goes through the queue the frames use and gets a fence of its own: the render
    /// loop needs the texture readable before it submits the frame that draws it, and the frame
    /// fences only cover presentation.
    bool SubmitAndWait(ID3D12Device* device, ID3D12CommandQueue* queue, ID3D12GraphicsCommandList* list) {
      if (FAILED(list->Close())) {
        LOG_ERROR("Failed to close the texture upload command list");
        return false;
      }

      ID3D12CommandList* lists[] = {list};
      queue->ExecuteCommandLists(1, lists);

      Microsoft::WRL::ComPtr<ID3D12Fence> fence;
      if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
        LOG_ERROR("Failed to create the texture upload fence");
        return false;
      }

      if (FAILED(queue->Signal(fence.Get(), kUploadFenceValue))) {
        LOG_ERROR("Failed to signal the texture upload fence");
        return false;
      }

      if (fence->GetCompletedValue() < kUploadFenceValue) {
        const HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (event == nullptr) {
          LOG_ERROR("Failed to create the texture upload event");
          return false;
        }

        fence->SetEventOnCompletion(kUploadFenceValue, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
      }

      return true;
    }
  }

  Texture::~Texture() {
    Destroy();
  }

  Texture::Texture(Texture&& other) noexcept :
    resource_(std::move(other.resource_)),
    cpu_handle_(other.cpu_handle_),
    gpu_handle_(other.gpu_handle_),
    has_descriptor_(other.has_descriptor_),
    width_(other.width_),
    height_(other.height_) {
    other.has_descriptor_ = false;
    other.width_ = 0;
    other.height_ = 0;
  }

  Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
      Destroy();

      resource_ = std::move(other.resource_);
      cpu_handle_ = other.cpu_handle_;
      gpu_handle_ = other.gpu_handle_;
      has_descriptor_ = other.has_descriptor_;
      width_ = other.width_;
      height_ = other.height_;

      other.has_descriptor_ = false;
      other.width_ = 0;
      other.height_ = 0;
    }
    return *this;
  }

  bool Texture::Create(const std::span<const std::uint8_t> rgba_pixels, const std::uint32_t width, const std::uint32_t height) {
    if (!kRENDERER) {
      LOG_ERROR("Cannot create a texture before the renderer exists");
      return false;
    }

    if (width == 0 || height == 0) {
      LOG_ERROR("Refusing to create a {}x{} texture", width, height);
      return false;
    }

    if (rgba_pixels.size() < static_cast<std::size_t>(width) * height * 4) {
      LOG_ERROR("Pixel buffer of {} bytes is too small for a {}x{} texture", rgba_pixels.size(), width, height);
      return false;
    }

    auto& context = kRENDERER->GetD3D12Context();
    ID3D12Device* device = context.GetDevice();
    if (device == nullptr) {
      LOG_ERROR("Cannot create a texture without a device");
      return false;
    }

    context.GetDescriptorHeapAllocator().Alloc(&cpu_handle_, &gpu_handle_);
    has_descriptor_ = true;
    width_ = width;
    height_ = height;

    D3D12_RESOURCE_DESC texture_desc{};
    texture_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texture_desc.Width = width;
    texture_desc.Height = height;
    texture_desc.DepthOrArraySize = 1;
    texture_desc.MipLevels = 1;
    texture_desc.Format = kTextureFormat;
    texture_desc.SampleDesc = {1, 0};
    texture_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    texture_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES default_heap{};
    default_heap.Type = D3D12_HEAP_TYPE_DEFAULT;

    HRESULT hr = device->CreateCommittedResource(&default_heap, D3D12_HEAP_FLAG_NONE, &texture_desc,
      D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create the texture resource: HRESULT = 0x{:08X}", hr);
      Destroy();
      return false;
    }

    // D3D12 requires every upload row to start on a 256 byte boundary, so the source rows are
    // copied into a wider buffer rather than handed over as they are.
    const UINT row_pitch = (width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
    const UINT64 upload_size = static_cast<UINT64>(row_pitch) * height;

    D3D12_RESOURCE_DESC upload_desc{};
    upload_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    upload_desc.Width = upload_size;
    upload_desc.Height = 1;
    upload_desc.DepthOrArraySize = 1;
    upload_desc.MipLevels = 1;
    upload_desc.Format = DXGI_FORMAT_UNKNOWN;
    upload_desc.SampleDesc = {1, 0};
    upload_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    upload_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES upload_heap{};
    upload_heap.Type = D3D12_HEAP_TYPE_UPLOAD;

    Microsoft::WRL::ComPtr<ID3D12Resource> upload_buffer;
    hr = device->CreateCommittedResource(&upload_heap, D3D12_HEAP_FLAG_NONE, &upload_desc,
      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&upload_buffer));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create the texture upload buffer: HRESULT = 0x{:08X}", hr);
      Destroy();
      return false;
    }

    void* mapped = nullptr;
    hr = upload_buffer->Map(0, nullptr, &mapped);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to map the texture upload buffer: HRESULT = 0x{:08X}", hr);
      Destroy();
      return false;
    }

    auto* dst = static_cast<std::uint8_t*>(mapped);
    const std::size_t src_row_size = static_cast<std::size_t>(width) * 4;
    for (std::uint32_t y = 0; y < height; ++y) {
      memcpy(dst + static_cast<std::size_t>(y) * row_pitch, rgba_pixels.data() + static_cast<std::size_t>(y) * src_row_size, src_row_size);
    }
    upload_buffer->Unmap(0, nullptr);

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
    hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create the texture upload allocator: HRESULT = 0x{:08X}", hr);
      Destroy();
      return false;
    }

    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list;
    hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&command_list));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create the texture upload command list: HRESULT = 0x{:08X}", hr);
      Destroy();
      return false;
    }

    D3D12_TEXTURE_COPY_LOCATION dst_location{};
    dst_location.pResource = resource_.Get();
    dst_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst_location.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION src_location{};
    src_location.pResource = upload_buffer.Get();
    src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src_location.PlacedFootprint.Offset = 0;
    src_location.PlacedFootprint.Footprint.Format = kTextureFormat;
    src_location.PlacedFootprint.Footprint.Width = width;
    src_location.PlacedFootprint.Footprint.Height = height;
    src_location.PlacedFootprint.Footprint.Depth = 1;
    src_location.PlacedFootprint.Footprint.RowPitch = row_pitch;

    command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);

    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = resource_.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    command_list->ResourceBarrier(1, &barrier);

    if (!SubmitAndWait(device, context.GetCommandQueue(), command_list.Get())) {
      Destroy();
      return false;
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
    srv_desc.Format = kTextureFormat;
    srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(resource_.Get(), &srv_desc, cpu_handle_);

    return true;
  }

  void Texture::Destroy() {
    // A submitted frame may still be reading these, so drain the GPU first. At teardown
    // kRENDERER is already gone along with the heap the descriptor came from, and there is
    // nothing left to hand it back to.
    if (resource_ != nullptr && kRENDERER) {
      kRENDERER->GetD3D12Context().WaitForAllFrames();
    }

    if (has_descriptor_ && kRENDERER) {
      kRENDERER->GetD3D12Context().GetDescriptorHeapAllocator().Free(cpu_handle_, gpu_handle_);
    }

    has_descriptor_ = false;
    resource_.Reset();
    cpu_handle_ = {};
    gpu_handle_ = {};
    width_ = 0;
    height_ = 0;
  }
}
