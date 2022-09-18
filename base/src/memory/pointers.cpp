//
// Created by X-ray on 3/7/2022.
//

#include "pointers.hpp"
#include <xorstr.hpp>
#include "scanner/all.hpp"

#define VAR_NAME(var) xorstr_(#var)

namespace gta_base {
  namespace memory {
    Pointers::Pointers() {
      kPOINTERS = this;
      scanner::Batch main_batch;

      main_batch.add(VAR_NAME(game_state_), xorstr_("83 3D ? ? ? ? ? 75 17 8B 43 20"), [this](scanner::Handle ptr){
        game_state_ = ptr.add(2).rip().as<decltype(game_state_)>();
      });

      main_batch.add(VAR_NAME(ped_factory_), xorstr_("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81"), [this](scanner::Handle ptr){
        ped_factory_ = ptr.add(3).rip().as<CPedFactory**>();
      });

      main_batch.add(VAR_NAME(swap_chain_), xorstr_("48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8"), [this](scanner::Handle ptr){
        swap_chain_ = ptr.add(3).rip().as<IDXGISwapChain**>();
      });

      main_batch.add(VAR_NAME(ReadBitBufDword), xorstr_("48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F0 8A"), [this](scanner::Handle ptr){
        ReadBitBufDword = ptr.sub(5).rip().as<decltype(ReadBitBufDword)>();
      });

      main_batch.add(VAR_NAME(ReadBitBufArray), xorstr_("48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C"), [this](scanner::Handle ptr){
        ReadBitBufArray = ptr.as<decltype(ReadBitBufArray)>();
      });

      main_batch.add(VAR_NAME(ReadBitBufString), xorstr_("E8 ? ? ? ? 48 8D 4F 3C"), [this](scanner::Handle ptr){
        ReadBitBufString = ptr.add(1).rip().as<decltype(ReadBitBufString)>();
      });

      main_batch.add(VAR_NAME(ReadBitBufBool), xorstr_("E8 ? ? ? ? 84 C0 74 2D 48 8D 57 20"), [this](scanner::Handle ptr){
        ReadBitBufBool = ptr.add(1).rip().as<decltype(ReadBitBufBool)>();
      });

      main_batch.add(VAR_NAME(WriteBitBufDword), xorstr_("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 8B EA BF 01"), [this](scanner::Handle ptr){
        WriteBitBufDword = ptr.as<decltype(WriteBitBufDword)>();
      });

      main_batch.add(VAR_NAME(WriteBitBufQword), xorstr_("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 41 8B F0 48 8B EA 48 8B D9 41 83 F8 20"), [this](scanner::Handle ptr){
        WriteBitBufQword = ptr.as<decltype(WriteBitBufQword)>();
      });

      main_batch.add(VAR_NAME(WriteBitBufInt64), xorstr_("E8 ? ? ? ? 8A 53 39 48 8B CF"), [this](scanner::Handle ptr){
        WriteBitBufInt64 = ptr.add(1).rip().as<decltype(WriteBitBufInt64)>();
      });

      main_batch.add(VAR_NAME(WriteBitBufInt32), xorstr_("E8 ? ? ? ? 8A 53 74"), [this](scanner::Handle ptr){
        WriteBitBufInt32 = ptr.add(1).rip().as<decltype(WriteBitBufInt32)>();
      });

      main_batch.add(VAR_NAME(WriteBitBufBool), xorstr_("E8 ? ? ? ? 8A 57 39"), [this](scanner::Handle ptr){
        WriteBitBufBool = ptr.add(1).rip().as<decltype(WriteBitBufBool)>();
      });

      main_batch.add(VAR_NAME(WriteBitBufArray), xorstr_("E8 ? ? ? ? 01 7E 08"), [this](scanner::Handle ptr){
        WriteBitBufArray = ptr.add(1).rip().as<decltype(WriteBitBufArray)>();
      });

      main_batch.add(std::string(VAR_NAME(script_threads_)) + "/" + VAR_NAME(RunScriptThreads), xorstr_("45 33 F6 8B E9 85 C9 B8"), [this](scanner::Handle ptr){
        script_threads_ = ptr.sub(4).rip().sub(8).as<decltype(script_threads_)>();
        RunScriptThreads = ptr.sub(0x1F).as<decltype(RunScriptThreads)>();
      });

      main_batch.add(std::string(VAR_NAME(native_registration_table_)) + "/" + VAR_NAME(GetNativeHandler), xorstr_("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A"), [this](scanner::Handle ptr){
        native_registration_table_ = ptr.add(3).rip().as<decltype(native_registration_table_)>();
        GetNativeHandler = ptr.add(12).rip().as<decltype(GetNativeHandler)>();
      });

      main_batch.add(VAR_NAME(FixVectors), xorstr_("83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA"), [this](scanner::Handle ptr){
        FixVectors = ptr.as<decltype(FixVectors)>();
      });

      main_batch.add(VAR_NAME(native_return_), xorstr_("FF E3"), [this](scanner::Handle ptr){
        native_return_ = ptr.as<decltype(native_return_)>();
      });

      main_batch.add(VAR_NAME(network_player_mgr_), xorstr_("48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07 48 8B CF"), [this](scanner::Handle ptr){
        network_player_mgr_ = ptr.add(3).rip().as<decltype(network_player_mgr_)>();
      });

      main_batch.add(VAR_NAME(network_), xorstr_("48 8B 0D ? ? ? ? 48 8D 55 E8 E8 ? ? ? ? 84 C0 75 C9"), [this](scanner::Handle ptr){
        network_ = ptr.add(3).rip().as<decltype(network_)>();
      });

      main_batch.add(std::string(VAR_NAME(GetConnectionPeer)) + "/" + VAR_NAME(SendRemoveGamerCmd), xorstr_("8D 42 FF 83 F8 FD 77 3D"), [this](scanner::Handle ptr){
        GetConnectionPeer = ptr.add(23).rip().as<decltype(GetConnectionPeer)>();
        SendRemoveGamerCmd = ptr.add(65).rip().as<decltype(SendRemoveGamerCmd)>();
      });

      main_batch.add(VAR_NAME(HandleRemoveGamerCmd), xorstr_("41 FF C6 FF C7"), [this](scanner::Handle ptr){
        HandleRemoveGamerCmd = ptr.sub(0x6E).as<decltype(HandleRemoveGamerCmd)>();
      });

      main_batch.add(VAR_NAME(NetworkPlayerMgrInit), xorstr_("41 56 48 83 EC ? 48 8B F1 B9 ? ? ? ? 49 8B F9 41 8B E8 4C 8B F2 E8"), [this](scanner::Handle ptr){
        NetworkPlayerMgrInit = ptr.sub(0x13).as<decltype(NetworkPlayerMgrInit)>();
      });

      main_batch.add(VAR_NAME(NetworkPlayerMgrShutdown), xorstr_("48 8D 9F ? ? ? ? EB ? 48 8B 13 48 85 D2 74 ? 48 8B CB E8 ? ? ? ? 48 83 7B ? ? 75 ? 48 8D 9F"), [this](scanner::Handle ptr){
        NetworkPlayerMgrShutdown = ptr.sub(0x1A).as<decltype(NetworkPlayerMgrShutdown)>();
      });

      main_batch.add(VAR_NAME(AssignPhysicalIdx), xorstr_("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 20 41 8A E8"), [this](scanner::Handle ptr){
        AssignPhysicalIdx = ptr.as<decltype(AssignPhysicalIdx)>();
      });

      main_batch.add(VAR_NAME(is_session_started_), xorstr_("40 38 35 ? ? ? ? 75 0E 4C 8B C3 49 8B D7 49 8B CE"), [this](scanner::Handle ptr){
        is_session_started_ = ptr.add(3).rip().as<decltype(is_session_started_)>();
      });

      // Request Control of Entity PATCH
      main_batch.add(VAR_NAME(spectator_check_), xorstr_("48 89 5C 24 ? 57 48 83 EC 20 8B D9 E8 ? ? ? ? ? ? ? ? 8B CB"), [this](scanner::Handle ptr){
        spectator_check_ = ptr.add(0x13).as<decltype(spectator_check_)>();
        *spectator_check_ = 0x9090;
      });

      main_batch.add(VAR_NAME(PtrToHandle), xorstr_("48 8B F9 48 83 C1 10 33 DB"), [this](scanner::Handle ptr) {
        PtrToHandle = ptr.sub(0x15).as<decltype(PtrToHandle)>();
      });

      main_batch.add(VAR_NAME(blame_explode_bypass_), xorstr_("0F 85 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 48 08 E8"), [this](scanner::Handle ptr) {
        blame_explode_bypass_ = ptr.as<decltype(blame_explode_bypass_)>();
      });

      main_batch.add(VAR_NAME(script_globals_), xorstr_("48 8D 15 ? ? ? ? 4C 8B C0 E8 ? ? ? ? 48 85 FF 48 89 1D"), [this](scanner::Handle ptr) {
        script_globals_ = ptr.add(3).rip().as<decltype(script_globals_)>();
      });

      auto mem_region = scanner::Module(nullptr);
      main_batch.run(mem_region);

      /**
		 * Freemode thread restorer through VM patch
		*/

      LOG_DEBUG("freemode patch 1");
      if (auto pat1 = mem_region.scan(xorstr_("3b 0a 0f 83 ? ? ? ? 48 ff c7"))) {
        *pat1.add(2).as<uint32_t*>() = 0xc9310272;
        *pat1.add(6).as<uint16_t*>() = 0x9090;
      }

      LOG_DEBUG("freemode patch 2");
      if (auto pat2 = mem_region.scan(xorstr_("3b 0a 0f 83 ? ? ? ? 49 03 fa"))) {
        *pat2.add(2).as<uint32_t*>() = 0xc9310272;
        *pat2.add(6).as<uint16_t*>() = 0x9090;
      }

      LOG_DEBUG("freemode patch 3");
      auto pat3 = mem_region.scan_all(xorstr_("3b 11 0f 83 ? ? ? ? 48 ff c7"));
      for (auto& handle : pat3) {
        *handle.add(2).as<uint32_t*>() = 0xd2310272;
        *handle.add(6).as<uint16_t*>() = 0x9090;
      }

      LOG_DEBUG("freemode patch 4");
      auto pat4 = mem_region.scan_all(xorstr_("3b 11 0f 83 ? ? ? ? 49 03 fa"));
      for (auto& handle : pat4) {
        *handle.add(2).as<uint32_t*>() = 0xd2310272;
        *handle.add(6).as<uint16_t*>() = 0x9090;
      }
    }

    Pointers::~Pointers() {
      *spectator_check_ = 0x6A75;

      kPOINTERS = nullptr;
    }
  }
}