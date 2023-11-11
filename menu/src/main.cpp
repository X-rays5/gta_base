//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <memory>
#include <ThreadPool/ThreadPool.h>
#include "render/manager.hpp"
#include "memory/scanner/module.hpp"

std::atomic<bool> base::globals::kRUNNING = true;

struct CGraphicsSettings {
  char gap0[8];
  int32_t SettingsVersion;
  int32_t ConfigSource;
  char gap10[8];
  int32_t Tessellation;
  float LodScale;
  float PedLodBias;
  float VehicleLodBias;
  int32_t ShadowQuality;
  int32_t ReflectionQuality;
  int32_t ReflectionMSAA;
  int32_t SSAO;
  int32_t AnisotropicFiltering;
  int32_t MSAA;
  int32_t MSAAFragments;
  int32_t MSAAQuality;
  char gap48[4];
  int32_t TextureQuality;
  int32_t ParticleQuality;
  int32_t WaterQuality;
  int32_t GrassQuality;
  int32_t ShaderQuality;
  int32_t Shadow_SoftShadows;
  char gap64;
  bool Shadow_ParticleShadows;
  const float Shadow_Distance;
  bool Shadow_LongShadows;
  const float Shadow_SplitZStart;
  const float Shadow_SplitZEnd;
  const float Shadow_aircraftExpWeight;
  char gap7C;
  bool Reflection_MipBlur;
  bool FXAA_Enabled;
  __declspec(align(2)) bool Lighting_FogVolumes;
  bool Shader_SSA;
  const float CityDensity;
  char gap88[8];
  int32_t DX_Version;
  int32_t PostFX;
  char gap98[80];
  int32_t AdapterIndex;
  int32_t OutputIndex;
  int32_t ScreenWidth;
  int32_t ScreenHeight;
  int32_t RefreshRate;
  int32_t Windowed;
  int32_t VSync;
  int32_t Stereo;
  const float Convergence;
  const float Separation;
  int32_t PauseOnFocusLoss;
  int32_t AspectRatio;
};


int base::menu_main() {
  auto res = memory::scanner::ScanForPattern("E8 ? ? ? ? E8 ? ? ? ? 4C 8D 5C 24 ? 49 8B 5B 20 49 8B 73 30", "", false);
  if (res.ok()) {
    auto handle = res.value();

    const auto setting_manager_ptr = handle.add(0x11).rip().as<std::uintptr_t>();
    const auto graphics_settings = reinterpret_cast<CGraphicsSettings*>(setting_manager_ptr + 8); // CGraphicsSettings?

    if (graphics_settings)
      LOG_INFO("DX version: ", graphics_settings->DX_Version);
    else
      LOG_ERROR("ptr invalid");
  }

  hmthrp::ThreadPool thread_pool(std::thread::hardware_concurrency());
  LOG_INFO("[INIT] Created thread pool with {} threads", std::thread::hardware_concurrency());

  /*std::thread render_thread(&base::render::Thread::RenderMain);
  LOG_INFO("[INIT] Created render thread");

  auto manager_inst = std::make_unique<render::Manager>();
  LOG_INFO("[INIT] render manager");

  while (!render::kTHREAD) {
    std::this_thread::yield();
  }

  render::kTHREAD->AddRenderCallback([](render::DrawQueueBuffer* buffer) {
    buffer->AddCommand(render::Text({0.5, 0.5}, ImColor(255, 0, 0), "Hello World!", false, true, 0.02f));
  });*/

  while (globals::kRUNNING) {
    if (GetAsyncKeyState(VK_END))
      break;
  }

  // Most likely already shutdown but just in case
  /*if (render_thread.joinable())
    render_thread.join();
  LOG_INFO("[SHUTDOWN] render thread");

  manager_inst.reset();
  LOG_INFO("[SHUTDOWN] render manager");*/

  thread_pool.shutdown();
  LOG_INFO("[SHUTDOWN] Thread pool has been shutdown");

  return 0;
}