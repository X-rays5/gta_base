//
// Created by X-ray on 21/08/2025.
//
#pragma once

#include <d3d12.h>
#include <vector>

namespace base::menu::render::d3d12 {
  class DescriptorHeapAllocator {
  public:
    void Create(ID3D12Device* device, ID3D12DescriptorHeap* heap);
    void Destroy();
    void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle);
    void Free(D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle);

  private:
    ID3D12DescriptorHeap* heap_ = nullptr;
    D3D12_DESCRIPTOR_HEAP_TYPE heap_type_ = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
    D3D12_CPU_DESCRIPTOR_HANDLE heap_start_cpu_{};
    D3D12_GPU_DESCRIPTOR_HANDLE heap_start_gpu_{};
    std::uint32_t heap_handle_increment_{};
    std::vector<std::int32_t> free_indices_;
  };
}
