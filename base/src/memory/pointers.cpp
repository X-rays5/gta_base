//
// Created by X-ray on 3/7/2022.
//

#include "pointers.hpp"
#include <xorstr.hpp>

#define VAR_NAME(var) xorstr_(#var)

namespace gta_base {
  namespace memory {
    Pointers::Pointers() {
      kPOINTERS = this;

      game_state_ = Signature(xorstr_("83 3D ? ? ? ? ? 75 17 8B 43 20")).Scan(VAR_NAME(game_state_)).Add(2).Rip().As<rage::GameState*>();

      ped_factory_ = Signature(xorstr_("48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52 8B 81")).Scan(VAR_NAME(ped_factory_)).Add(3).Rip().As<CPedFactory**>();

      swap_chain_ = Signature(xorstr_("48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8")).Scan(VAR_NAME(swap_chain_)).Add(3).Rip().As<IDXGISwapChain**>();

      ReadBitBufDword = Signature(xorstr_("48 89 74 24 ? 57 48 83 EC 20 48 8B D9 33 C9 41 8B F0 8A")).Scan(VAR_NAME(ReadBitBufDword)).Sub(5).As<decltype(ReadBitBufDword)>();
      ReadBitBufArray = Signature(xorstr_("48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 4C")).Scan(VAR_NAME(ReadBitBufArray)).As<decltype(ReadBitBufArray)>();
      ReadBitBufString = Signature(xorstr_("E8 ? ? ? ? 48 8D 4F 3C")).Scan(VAR_NAME(ReadBitBufString)).Add(1).Rip().As<decltype(ReadBitBufString)>();
      ReadBitBufBool = Signature(xorstr_("E8 ? ? ? ? 84 C0 74 2D 48 8D 57 20")).Scan(VAR_NAME(ReadBitBufBool)).Add(1).Rip().As<decltype(ReadBitBufBool)>();
      WriteBitBufDword = Signature(xorstr_("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 20 8B EA BF 01")).Scan(VAR_NAME(WriteBitBufDword)).As<decltype(WriteBitBufDword)>();
      WriteBitBufQword = Signature(xorstr_("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 41 8B F0 48 8B EA 48 8B D9 41 83 F8 20")).Scan(VAR_NAME(WriteBitBufQword)).As<decltype(WriteBitBufQword)>();
      WriteBitBufInt64 = Signature(xorstr_("E8 ? ? ? ? 8A 53 39 48 8B CF")).Scan(VAR_NAME(WriteBitBufInt64)).Add(1).Rip().As<decltype(WriteBitBufInt64)>();
      WriteBitBufInt32 = Signature(xorstr_("E8 ? ? ? ? 8A 53 74")).Scan(VAR_NAME(WriteBitBufInt32)).Add(1).Rip().As<decltype(WriteBitBufInt32)>();
      WriteBitBufBool = Signature(xorstr_("E8 ? ? ? ? 8A 57 39")).Scan(VAR_NAME(WriteBitBufBool)).Add(1).Rip().As<decltype(WriteBitBufBool)>();
      WriteBitBufArray = Signature(xorstr_("E8 ? ? ? ? 01 7E 08")).Scan(VAR_NAME(WriteBitBufArray)).Add(1).Rip().As<decltype(WriteBitBufArray)>();

      auto script_threads_tmp = Signature(xorstr_("45 33 F6 8B E9 85 C9 B8")).Scan("script_threads");
      script_threads_ = script_threads_tmp.Sub(4).Rip().Sub(8).As<decltype(script_threads_)>();
      RunScriptThreads = script_threads_tmp.Sub(0x1F).As<decltype(RunScriptThreads)>();
    }

    Pointers::~Pointers() {
      kPOINTERS = nullptr;
    }
  }
}