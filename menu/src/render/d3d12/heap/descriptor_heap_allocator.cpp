//
// Created by X-ray on 21/08/2025.
//

#include "descriptor_heap_allocator.hpp"

namespace base::menu::render::d3d12 {
  void DescriptorHeapAllocator::Create(ID3D12Device* device, ID3D12DescriptorHeap* heap) {
    GTA_BASE_ASSERT(heap_ == nullptr && free_indices_.empty(), "Allocator already initialized");
    heap_ = heap;
    const D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();
    heap_type_ = desc.Type;
    heap_start_cpu_ = heap_->GetCPUDescriptorHandleForHeapStart();
    heap_start_gpu_ = heap_->GetGPUDescriptorHandleForHeapStart();
    heap_handle_increment_ = device->GetDescriptorHandleIncrementSize(heap_type_);
    free_indices_.reserve(static_cast<std::int32_t>(desc.NumDescriptors));
    for (std::int32_t n = desc.NumDescriptors; n > 0; n--)
      free_indices_.push_back(n - 1);
  }

  void DescriptorHeapAllocator::Destroy() {
    heap_ = nullptr;
    free_indices_.clear();
  }

  void DescriptorHeapAllocator::Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle) {
    GTA_BASE_ASSERT(free_indices_.size() > 0, "No free descriptors available");
    const std::int32_t idx = free_indices_.back();
    free_indices_.pop_back();
    out_cpu_desc_handle->ptr = heap_start_cpu_.ptr + (idx * heap_handle_increment_);
    out_gpu_desc_handle->ptr = heap_start_gpu_.ptr + (idx * heap_handle_increment_);
  }

  void DescriptorHeapAllocator::Free(const D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, const D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle) {
    const std::int32_t cpu_idx = static_cast<std::int32_t>((out_cpu_desc_handle.ptr - heap_start_cpu_.ptr) / heap_handle_increment_);
    const std::int32_t gpu_idx = static_cast<std::int32_t>((out_gpu_desc_handle.ptr - heap_start_gpu_.ptr) / heap_handle_increment_);
    GTA_BASE_ASSERT(cpu_idx == gpu_idx, "CPU and GPU descriptor indices do not match");
    free_indices_.push_back(cpu_idx);
  }
}