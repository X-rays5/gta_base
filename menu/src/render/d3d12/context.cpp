//
// Created by X-ray on 22/08/2025.
//
#include "context.hpp"
#include <imgui/imgui_impl_dx12.h>
#include <imgui/imgui_impl_win32.h>

namespace base::menu::render::d3d12 {
  Context::Context(const Microsoft::WRL::ComPtr<IDXGISwapChain1>& game_swap_chain, const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& command_queue) : swap_chain_1_(game_swap_chain), command_queue_(command_queue) {}

  Context::~Context() {
    Cleanup();
  }

  Context::Context(Context&& other) noexcept
    : initialized_(other.initialized_)
      , resizing_(other.resizing_)
      , swap_chain_1_(std::move(other.swap_chain_1_))
      , swap_chain_2_(std::move(other.swap_chain_2_))
      , swap_chain_3_(std::move(other.swap_chain_3_))
      , device_(std::move(other.device_))
      , command_queue_(std::move(other.command_queue_))
      , command_allocator_(std::move(other.command_allocator_))
      , command_list_(std::move(other.command_list_))
      , descriptor_heap_(std::move(other.descriptor_heap_))
      , back_buffer_descriptor_heap_(std::move(other.back_buffer_descriptor_heap_))
      , swap_chain_desc_(other.swap_chain_desc_)
      , frame_ctx_(std::move(other.frame_ctx_))
      , descriptor_heap_allocator_(std::move(other.descriptor_heap_allocator_))
      , fence_(std::move(other.fence_))
      , fence_event_(other.fence_event_)
      , fence_last_signaled_value_(other.fence_last_signaled_value_)
      , swap_chain_waitable_object_(other.swap_chain_waitable_object_) {
    other.fence_event_ = nullptr;
    other.swap_chain_waitable_object_ = nullptr;
    other.initialized_ = false;
    other.resizing_ = false;
  }

  Context& Context::operator=(Context&& other) noexcept {
    if (this != &other) {
      Cleanup();

      swap_chain_1_ = std::move(other.swap_chain_1_);
      swap_chain_2_ = std::move(other.swap_chain_2_);
      swap_chain_3_ = std::move(other.swap_chain_3_);
      device_ = std::move(other.device_);
      command_queue_ = std::move(other.command_queue_);
      command_allocator_ = std::move(other.command_allocator_);
      command_list_ = std::move(other.command_list_);
      descriptor_heap_ = std::move(other.descriptor_heap_);
      back_buffer_descriptor_heap_ = std::move(other.back_buffer_descriptor_heap_);
      swap_chain_desc_ = other.swap_chain_desc_;
      frame_ctx_ = std::move(other.frame_ctx_);
      descriptor_heap_allocator_ = std::move(other.descriptor_heap_allocator_);
      fence_ = std::move(other.fence_);
      fence_event_ = other.fence_event_;
      fence_last_signaled_value_ = other.fence_last_signaled_value_;
      swap_chain_waitable_object_ = other.swap_chain_waitable_object_;
      initialized_ = other.initialized_;
      resizing_ = other.resizing_;

      other.fence_event_ = nullptr;
      other.swap_chain_waitable_object_ = nullptr;
      other.initialized_ = false;
      other.resizing_ = false;
    }
    return *this;
  }

  bool Context::Initialize() {
    if (initialized_) {
      LOG_WARN("Context is already initialized");
      return true;
    }

    try {
      if (!CreateDeviceAndObjects()) {
        return false;
      }

      initialized_ = true;
      return true;
    } catch (const std::exception& e) {
      LOG_ERROR("Exception during context initialization: {}", e.what());
      return false;
    }
  }

  bool Context::IsInitialized() const {
    return initialized_;
  }

  void Context::Cleanup() noexcept {
    if (!initialized_) {
      return;
    }

    try {
      // 1. Ensure GPU is completely idle
      WaitForAllFrames();

      // 2. Release frame resources
      for (auto& frame : frame_ctx_) {
        frame.resource.Reset();
        frame.command_allocator.Reset();
        frame.fence_value = 0;
      }
      frame_ctx_.clear();

      // 3. Close handles
      if (fence_event_) {
        CloseHandle(fence_event_);
        fence_event_ = nullptr;
      }

      if (swap_chain_waitable_object_) {
        CloseHandle(swap_chain_waitable_object_);
        swap_chain_waitable_object_ = nullptr;
      }

      // 4. Cleanup allocator
      descriptor_heap_allocator_.Destroy();

      // 5. Reset COM objects (they'll release automatically)
      command_list_.Reset();
      command_allocator_.Reset();
      descriptor_heap_.Reset();
      back_buffer_descriptor_heap_.Reset();
      fence_.Reset();
      device_.Reset();
      swap_chain_3_.Reset();
      swap_chain_2_.Reset();
      swap_chain_1_.Reset();
      command_queue_.Reset();

      initialized_ = false;
      LOG_DEBUG("D3D12 context cleanup complete");
    } catch (...) {
      // Never throw from cleanup
      LOG_ERROR("Exception during D3D12 context cleanup");
      initialized_ = false;
    }
  }

  ID3D12Device* Context::GetDevice() const {
    return device_.Get();
  }

  ID3D12CommandQueue* Context::GetCommandQueue() const {
    return command_queue_.Get();
  }

  ID3D12GraphicsCommandList* Context::GetCommandList() const {
    return command_list_.Get();
  }

  ID3D12DescriptorHeap* Context::GetDescriptorHeap() const {
    return descriptor_heap_.Get();
  }

  IDXGISwapChain3* Context::GetSwapChain() const {
    return swap_chain_3_.Get();
  }

  const DXGI_SWAP_CHAIN_DESC& Context::GetSwapChainDesc() const {
    return swap_chain_desc_;
  }

  std::vector<Context::Frame>& Context::GetFrameContexts() {
    return frame_ctx_;
  }

  DescriptorHeapAllocator& Context::GetDescriptorHeapAllocator() {
    return descriptor_heap_allocator_;
  }

  void Context::PreResize() {
    resizing_ = true;
    WaitForLastFrame();

    ImGui_ImplDX12_InvalidateDeviceObjects();

    for (size_t i{}; i != swap_chain_desc_.BufferCount; ++i) {
      frame_ctx_[i].resource.Reset();
    }
  }

  void Context::PostResize() {
    ImGui_ImplDX12_CreateDeviceObjects();
    const auto RTV_descriptor_size{device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)};
    D3D12_CPU_DESCRIPTOR_HANDLE RTV_handle{back_buffer_descriptor_heap_->GetCPUDescriptorHandleForHeapStart()};
    for (std::uint32_t i{}; i != swap_chain_desc_.BufferCount; ++i) {
      Microsoft::WRL::ComPtr<ID3D12Resource> back_buffer{};
      frame_ctx_[i].descriptor = RTV_handle;
      HRESULT hr = swap_chain_3_->GetBuffer(i, IID_PPV_ARGS(&back_buffer));
      if (FAILED(hr)) {
        LOG_ERROR("Failed to get back buffer {}: HRESULT = 0x{:08X}", i, hr);
        continue;
      }
      device_->CreateRenderTargetView(back_buffer.Get(), nullptr, RTV_handle);
      frame_ctx_[i].resource = back_buffer;
      RTV_handle.ptr += RTV_descriptor_size;
    }

    resizing_ = false;
  }

  void Context::NewFrame() const {
    if (!initialized_) {
      LOG_ERROR("Context not initialized - cannot create new frame");
      return;
    }

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
  }

  void Context::WaitForNextFrame() {
    const std::uint32_t next_frame_idx = frame_index_ + 1;
    frame_index_ = next_frame_idx;

    HANDLE waitable_objects[] = {swap_chain_waitable_object_, nullptr};
    DWORD num_waitable_objects = 1;

    const Frame& cur_frame_ctx = frame_ctx_[next_frame_idx % swap_chain_desc_.BufferCount];
    const std::uint64_t fence_value = cur_frame_ctx.fence_value;

    if (fence_value != 0) {
      HRESULT hr = fence_->SetEventOnCompletion(fence_value, fence_event_);
      if (FAILED(hr)) {
        LOG_ERROR("Failed to set fence event on completion: HRESULT = 0x{:08X}", hr);
        return;
      }
      waitable_objects[1] = fence_event_;
      num_waitable_objects = 2;
    }

    // Only wait if we have waitable objects
    if (swap_chain_waitable_object_ || fence_value != 0) {
      WaitForMultipleObjects(num_waitable_objects, waitable_objects, TRUE, INFINITE);
    }
  }

  void Context::WaitForLastFrame() {
    Frame& cur_frame_ctx = frame_ctx_[frame_index_ % swap_chain_desc_.BufferCount];
    const std::uint64_t fence_value = cur_frame_ctx.fence_value;
    if (fence_value == 0) {
      return;
    }

    cur_frame_ctx.fence_value = 0;
    if (fence_->GetCompletedValue() >= fence_value) {
      return;
    }

    HRESULT hr = fence_->SetEventOnCompletion(fence_value, fence_event_);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to set fence event on completion: HRESULT = 0x{:08X}", hr);
      return;
    }
    WaitForSingleObject(fence_event_, INFINITE);
  }

  void Context::WaitForAllFrames() const {
    if (!fence_ || !fence_event_) return;

    // Find the highest fence value from all frames
    std::uint64_t max_fence_value = 0;
    for (const auto& frame : frame_ctx_) {
      max_fence_value = std::max(max_fence_value, frame.fence_value);
    }

    if (max_fence_value > 0 && fence_->GetCompletedValue() < max_fence_value) {
      const HRESULT hr = fence_->SetEventOnCompletion(max_fence_value, fence_event_);
      if (SUCCEEDED(hr)) {
        WaitForSingleObject(fence_event_, 5000); // 5 second timeout
      }
    }
  }

  void Context::EndFrame() {
    WaitForNextFrame();
    HRESULT hr{};

    if (!IsDeviceValid()) {
      LOG_ERROR("Device is not valid in EndFrame");
      ImGui_ImplDX12_InvalidateDeviceObjects();
      return;
    }

    Frame& cur_frame_ctx = GetCurrentFrame();
    hr = cur_frame_ctx.command_allocator->Reset();
    if (FAILED(hr)) {
      LOG_ERROR("Failed to reset command allocator: {}", hr);
      return;
    }

    D3D12_RESOURCE_BARRIER barrier{
      D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
      D3D12_RESOURCE_BARRIER_FLAG_NONE,
      {
        {
          cur_frame_ctx.resource.Get(),
          D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
          D3D12_RESOURCE_STATE_PRESENT,
          D3D12_RESOURCE_STATE_RENDER_TARGET
        }
      }
    };
    hr = command_list_->Reset(cur_frame_ctx.command_allocator.Get(), nullptr);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to reset command list: {}", hr);
      return;
    }
    command_list_->ResourceBarrier(1, &barrier);
    command_list_->OMSetRenderTargets(1, &cur_frame_ctx.descriptor, false, nullptr);
    command_list_->SetDescriptorHeaps(1, descriptor_heap_.GetAddressOf());

    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list_.Get());

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    command_list_->ResourceBarrier(1, &barrier);
    hr = command_list_->Close();
    if (FAILED(hr)) {
      LOG_ERROR("Failed to close command list: {}", hr);
      return;
    }

    ID3D12CommandList* CommandLists[]{command_list_.Get()};
    command_queue_->ExecuteCommandLists(_countof(CommandLists), CommandLists);

    const std::uint64_t fence_value = fence_last_signaled_value_ + 1;
    hr = command_queue_->Signal(fence_.Get(), fence_value);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to signal fence: {}", hr);
      return;
    }
    fence_last_signaled_value_ = fence_value;
    cur_frame_ctx.fence_value = fence_value;
  }

  Context::Frame& Context::GetFrame(const size_t frame_index) {
    if (frame_index >= frame_ctx_.size()) {
      throw std::out_of_range("Frame index out of range");
    }
    return frame_ctx_[frame_index];
  }

  std::uint32_t Context::GetCurrentFrameIndex() const {
    return swap_chain_3_->GetCurrentBackBufferIndex();
  }

  Context::Frame& Context::GetCurrentFrame() {
    return GetFrame(GetCurrentFrameIndex());
  }

  bool Context::IsDeviceValid() const {
    if (!device_) {
      return false;
    }

    HRESULT hr = device_->GetDeviceRemovedReason();
    if (FAILED(hr)) {
      LOG_ERROR("Device removed/lost: HRESULT = 0x{:08X}", hr);
      return false;
    }

    return true;
  }

  bool Context::CreateDeviceAndObjects() {
    HRESULT hr{};

    // Validate input parameters
    if (!swap_chain_1_) {
      LOG_ERROR("Failed to create D3D12 context: swap_chain_1 is null");
      return false;
    }

    if (!command_queue_) {
      LOG_ERROR("Failed to create D3D12 context: command_queue is null");
      return false;
    }

    // Get SwapChain2 interface
    hr = swap_chain_1_.As(&swap_chain_2_);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to query IDXGISwapChain2 interface: HRESULT = 0x{:08X}", hr);
      return false;
    }

    swap_chain_waitable_object_ = swap_chain_2_->GetFrameLatencyWaitableObject();

    // Get SwapChain3 interface
    hr = swap_chain_1_.As(&swap_chain_3_);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to query IDXGISwapChain3 interface: HRESULT = 0x{:08X}", hr);
      return false;
    }

    // Get device from swap chain
    hr = swap_chain_3_->GetDevice(IID_PPV_ARGS(&device_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to get D3D12 device from swap chain: HRESULT = 0x{:08X}", hr);
      return false;
    }

    // Get swap chain description
    hr = swap_chain_3_->GetDesc(&swap_chain_desc_);
    if (FAILED(hr)) {
      LOG_ERROR("Failed to get swap chain description: HRESULT = 0x{:08X}", hr);
      return false;
    }

    // Create fence
    hr = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create D3D12 fence: HRESULT = 0x{:08X}", hr);
      return false;
    }

    fence_event_ = CreateEventA(nullptr, FALSE, FALSE, nullptr);
    if (!fence_event_) {
      LOG_ERROR("Failed to create fence event: GetLastError = {}", GetLastError());
      return false;
    }

    // Resize frame contexts
    frame_ctx_.resize(swap_chain_desc_.BufferCount);

    // Create descriptor heap for shader resources
    const UINT descriptor_count = std::max(256U, swap_chain_desc_.BufferCount * 32);
    const D3D12_DESCRIPTOR_HEAP_DESC descriptor_desc{
      D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
      descriptor_count,
      D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
      0
    };
    hr = device_->CreateDescriptorHeap(&descriptor_desc, IID_PPV_ARGS(&descriptor_heap_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create descriptor heap: HRESULT = 0x{:08X}", hr);
      return false;
    }

    // Create command allocators
    hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create main command allocator: HRESULT = 0x{:08X}", hr);
      return false;
    }

    frame_ctx_[0].command_allocator = command_allocator_;

    for (size_t i = 1; i < swap_chain_desc_.BufferCount; ++i) {
      hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frame_ctx_[i].command_allocator));
      if (FAILED(hr)) {
        LOG_ERROR("Failed to create command allocator for frame {}: HRESULT = 0x{:08X}", i, hr);
        return false;
      }
    }

    // Create command list
    hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), nullptr, IID_PPV_ARGS(&command_list_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create command list: HRESULT = 0x{:08X}", hr);
      return false;
    }

    hr = command_list_->Close();
    if (FAILED(hr)) {
      LOG_ERROR("Failed to close command list: HRESULT = 0x{:08X}", hr);
      return false;
    }

    // Create RTV descriptor heap
    const D3D12_DESCRIPTOR_HEAP_DESC rtv_desc{
      D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
      swap_chain_desc_.BufferCount,
      D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
      0
    };
    hr = device_->CreateDescriptorHeap(&rtv_desc, IID_PPV_ARGS(&back_buffer_descriptor_heap_));
    if (FAILED(hr)) {
      LOG_ERROR("Failed to create RTV descriptor heap: HRESULT = 0x{:08X}", hr);
      return false;
    }

    // Create RTVs for back buffers
    const UINT rtv_descriptor_size = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = back_buffer_descriptor_heap_->GetCPUDescriptorHandleForHeapStart();

    for (size_t i = 0; i < swap_chain_desc_.BufferCount; ++i) {
      frame_ctx_[i].descriptor = rtv_handle;

      hr = swap_chain_3_->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&frame_ctx_[i].resource));
      if (FAILED(hr)) {
        LOG_ERROR("Failed to get back buffer {}: HRESULT = 0x{:08X}", i, hr);
        return false;
      }

      device_->CreateRenderTargetView(frame_ctx_[i].resource.Get(), nullptr, rtv_handle);
      rtv_handle.ptr += rtv_descriptor_size;
    }

    // Initialize descriptor heap allocator
    descriptor_heap_allocator_.Create(device_.Get(), descriptor_heap_.Get());

    return true;
  }
}
