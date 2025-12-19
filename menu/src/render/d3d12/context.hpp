//
// Created by X-ray on 21/08/2025.
//

#pragma once
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <wrl.h>

#include "heap/descriptor_heap_allocator.hpp"

namespace base::menu::render::d3d12 {
  /**
  * @class Context
  * @brief D3D12 rendering context managing device, command objects, and frame resources
  *
  * This class encapsulates all the D3D12 objects needed for rendering, including
  * device, command queue, swap chain, and per-frame resources. It provides
  * proper resource management and initialization in a single cohesive unit.
  */
  class Context {
  public:
    /**
    * @struct Frame
    * @brief Per-frame rendering resources
    *
    * Contains all resources that are needed on a per-frame basis,
    * including command allocators, back buffer resources, and synchronization.
    */
    struct Frame {
      Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator; ///< Command allocator for this frame
      Microsoft::WRL::ComPtr<ID3D12Resource> resource; ///< Back buffer resource for this frame
      D3D12_CPU_DESCRIPTOR_HANDLE descriptor; ///< RTV descriptor handle for this frame
      UINT64 fence_value = 0; ///< Last fence value used for this frame
    };

    /**
    * @brief Constructs a D3D12 context with the provided swap chain and command queue
    * @param game_swap_chain The swap chain to use for presentation
    * @param command_queue The command queue for submitting commands
    */
    Context(const Microsoft::WRL::ComPtr<IDXGISwapChain1>& game_swap_chain, const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& command_queue);

    ~Context();
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&& other) noexcept;
    Context& operator=(Context&& other) noexcept;

    /**
    * @brief Initializes the D3D12 context and creates all necessary objects
    * @return true if initialization succeeded, false otherwise
    *
    * Creates the D3D12 device, command objects, descriptor heaps, and frame resources.
    * Must be called before using the context for rendering operations.
    *
    * @throws std::runtime_error if initialization fails after constructor validation
    */
    bool Initialize();

    /**
    * @brief Checks if the context has been successfully initialized
    * @return true if initialized and ready for use, false otherwise
    */
    bool IsInitialized() const;

    /**
    * @brief Cleans up all resources and resets the context
    */
    void Cleanup() noexcept;

    /**
    * @brief Gets the D3D12 device
    * @return Pointer to the ID3D12Device, or nullptr if not initialized
    */
    ID3D12Device* GetDevice() const;

    /**
     * @brief Gets the command queue
     * @return Pointer to the ID3D12CommandQueue, or nullptr if not initialized
     */
    ID3D12CommandQueue* GetCommandQueue() const;

    /**
     * @brief Gets the graphics command list
     * @return Pointer to the ID3D12GraphicsCommandList, or nullptr if not initialized
     */
    ID3D12GraphicsCommandList* GetCommandList() const;

    /**
     * @brief Gets the command allocator for the current frame
     * @return Pointer to the ID3D12CommandAllocator, or nullptr if not initialized
     */
    ID3D12DescriptorHeap* GetDescriptorHeap() const;

    /**
     * @brief Gets the swap chain
     * @return Pointer to the IDXGISwapChain3, or nullptr if not initialized
     */
    IDXGISwapChain3* GetSwapChain() const;

    /**
     * @brief Gets the swap chain description
     * @return Reference to the DXGI_SWAP_CHAIN_DESC
     * @note Only valid after successful initialization
     */
    const DXGI_SWAP_CHAIN_DESC& GetSwapChainDesc() const;

    /**
     * @brief Gets all frame contexts
     * @return Const reference to the vector of Frame objects
     */
    std::vector<Frame>& GetFrameContexts();

    /**
     * @brief Gets the descriptor heap allocator
     * @return Reference to the DescriptorHeapAllocator
     * @note Only valid after successful initialization
     */
    DescriptorHeapAllocator& GetDescriptorHeapAllocator();

    /**
    * @brief Prepares the context for swap chain resize
    *
    * This method should be called before resizing the swap chain. It:
    * - Sets the resizing flag to prevent rendering operations
    * - Waits for all pending GPU operations to complete
    * - Invalidates ImGui device objects
    * - Releases all back buffer resources
    *
    * @note Must be paired with PostResize() after the swap chain is resized
    */
    void PreResize();

    /**
    * @brief Completes the context setup after swap chain resize
    *
    * This method should be called after the swap chain has been resized. It:
    * - Recreates ImGui device objects
    * - Recreates render target views for new back buffers
    * - Updates frame context resources
    * - Clears the resizing flag
    *
    * @note Must be called after PreResize() and swap chain resize
    */
    void PostResize();

    /**
    * @brief Initializes a new ImGui frame
    *
    * Calls ImGui_ImplDX12_NewFrame(), ImGui_ImplWin32_NewFrame(), and ImGui::NewFrame()
    * to prepare ImGui for a new rendering frame. Should be called at the beginning
    * of each frame before any ImGui drawing commands.
    */
    void NewFrame() const;

    /**
    * @brief Waits for the next frame to be ready for rendering
    *
    * Advances the frame index and waits for both the swap chain waitable object
    * and any pending fence operations for the next frame. This ensures proper
    * frame synchronization and prevents rendering to frames that are still in use.
    *
    * @note This method will block until the next frame is available
    */
    void WaitForNextFrame();

    /**
    * @brief Waits for the current frame's GPU operations to complete
    *
    * Blocks until all GPU work for the current frame has finished.
    * This is useful for ensuring GPU idle state before resource changes.
    */
    void WaitForLastFrame();

    /**
    * @brief Waits for all GPU frames to complete execution before proceeding with cleanup
    *
    * This method ensures that all pending GPU work across all frame buffers has finished
    * before allowing resource cleanup to proceed. It signals a fence value higher than
    * any currently pending frame and waits for the GPU to reach that fence value.
    *
    * The method is critical during shutdown to prevent:
    * - Resource cleanup while GPU is still using them
    * - Race conditions between CPU cleanup and GPU execution
    * - Potential crashes or undefined behavior in the host application
    *
    * @note This method includes a 5-second timeout to prevent indefinite blocking
    * @note Safe to call multiple times - will return quickly if GPU is already idle
    * @note Will log warnings if GPU synchronization fails but won't throw exceptions
    *
    * @see WaitForLastFrame() for single-frame synchronization
    * @see Shutdown() for the complete shutdown sequence
    */
    void WaitForAllFrames() const;

    /**
    * @brief Completes the current frame and submits it for presentation
    *
    * @note Should be called after all rendering operations are complete
    */
    void EndFrame();

    /**
     * @brief Gets the frame context for the specified frame index
     * @param frame_index Index of the frame (must be < buffer count)
     * @return Reference to the frame context
     * @throws std::out_of_range if frame_index is invalid
     */
    Frame& GetFrame(size_t frame_index);

    /**
   * @brief Gets the current frame index
   * @return Current frame index for double/triple buffering
   */
    std::uint32_t GetCurrentFrameIndex() const;

    /**
     * @brief Gets the current frame context
     * @return Reference to the current Frame
     */
    Frame& GetCurrentFrame();

    bool IsDeviceValid() const;

  private:
    bool initialized_ = false;
    bool resizing_ = false;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swap_chain_1_;
    Microsoft::WRL::ComPtr<IDXGISwapChain2> swap_chain_2_;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> swap_chain_3_;
    Microsoft::WRL::ComPtr<ID3D12Device> device_;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> back_buffer_descriptor_heap_;
    DXGI_SWAP_CHAIN_DESC swap_chain_desc_{};
    std::vector<Frame> frame_ctx_;
    DescriptorHeapAllocator descriptor_heap_allocator_{};
    std::uint32_t frame_index_ = 0;

    /*
    * Fence
    */
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
    HANDLE fence_event_{};
    std::uint64_t fence_last_signaled_value_{};
    HANDLE swap_chain_waitable_object_{};

  private:
    /**
    * @brief Internal method to create device and all D3D12 objects
    * @return true if successful, false otherwise
    */
    bool CreateDeviceAndObjects();
  };
}
