//
// Created by X-ray on 3/7/2022.
//

#pragma once

#ifndef GTABASE_MEM_POINTERS_HPP
#define GTABASE_MEM_POINTERS_HPP
#include <memory>
#include <d3d11.h>
#include <ped/CPedFactory.hpp>
#include <network/CNetGamePlayer.hpp>
#include <network/snSession.hpp>
#include <network/Network.hpp>
#include "../rage/enums.hpp"
#include "../rage/classes/script_thread.hpp"
#include "../rage/classes/at_array.hpp"
#include "../rage/classes/natives.hpp"
#include "../rage/classes/net_connection_mgr.hpp"
#include "../rage/classes/net_connection_peer.hpp"
#include "../rage/classes/script_program.hpp"
#include "../rage/classes/fidevice.hpp"

namespace gta_base::memory {
    class Pointers {
    public:
      Pointers();
      ~Pointers();

      eGameState* game_state_{};

      CPedFactory** ped_factory_{};

      IDXGISwapChain** swap_chain_{};

      using read_bitbuf_dword_t = bool(*)(rage::datBitBuffer* buffer, PVOID read, int bits);
      read_bitbuf_dword_t ReadBitBufDword{};
      using read_bitbuf_string_t = bool(*)(rage::datBitBuffer* buffer, char* read, int bits);
      read_bitbuf_string_t ReadBitBufString{};
      using read_bitbuf_bool_t = bool(*)(rage::datBitBuffer* buffer, bool* read, int bits);
      read_bitbuf_bool_t ReadBitBufBool{};
      using read_bitbuf_array_t = bool(*)(rage::datBitBuffer* buffer, PVOID read, int bits, int unk);
      read_bitbuf_array_t ReadBitBufArray{};
      using write_bitbuf_qword_t = bool(*)(rage::datBitBuffer* buffer, uint64_t val, int bits);
      write_bitbuf_qword_t WriteBitBufQword{};
      using write_bitbuf_dword_t = bool(*)(rage::datBitBuffer* buffer, uint32_t val, int bits);
      write_bitbuf_dword_t WriteBitBufDword{};
      using write_bitbuf_int64_t = bool(*)(rage::datBitBuffer* buffer, int64_t val, int bits);
      write_bitbuf_int64_t WriteBitBufInt64{};
      using write_bitbuf_int32_t = bool(*)(rage::datBitBuffer* buffer, int32_t val, int bits);
      write_bitbuf_int32_t WriteBitBufInt32{};
      using write_bitbuf_bool_t = bool(*)(rage::datBitBuffer* buffer, bool val, int bits);
      write_bitbuf_bool_t WriteBitBufBool{};
      using write_bitbuf_array_t = bool(*)(rage::datBitBuffer* buffer, uint8_t* val, int bits, int unk);
      write_bitbuf_array_t WriteBitBufArray{};

      rage::atArray<GtaThread*>* script_threads_;
      PVOID RunScriptThreads{};

      rage::scrNativeRegistrationTable* native_registration_table_{};
      rage::scrProgramTable* script_programs_table{};
      using get_native_handler_t = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable* registration_table, rage::scrNativeHash hash);
      get_native_handler_t GetNativeHandler{};
      using fix_vectors_t = void(*)(rage::scrNativeCallContext* call_ctx);
      fix_vectors_t FixVectors{};
      PVOID native_return_;

      CNetworkPlayerMgr** network_player_mgr_{};

      Network** network_{};

      using get_connection_peer_t = rage::netConnectionPeer* (*)(rage::netConnectionManager* manager, int peer_id);
      get_connection_peer_t GetConnectionPeer{};
      using send_remove_gamer_cmd_t = void(*)(rage::netConnectionManager* net_connection_mgr, rage::netConnectionPeer* player, int connection_id, rage::snMsgRemoveGamersFromSessionCmd* cmd, int flags);
      send_remove_gamer_cmd_t SendRemoveGamerCmd{};
      using handle_remove_gamer_cmd_t = void* (*)(rage::snSession* session, rage::snPlayer* origin, rage::snMsgRemoveGamersFromSessionCmd* cmd);
      handle_remove_gamer_cmd_t HandleRemoveGamerCmd{};

      PVOID NetworkPlayerMgrInit{};
      PVOID NetworkPlayerMgrShutdown{};
      PVOID AssignPhysicalIdx{};

      PUSHORT spectator_check_;

      using ptr_to_handle_t = Entity(*)(void* entity);
      ptr_to_handle_t PtrToHandle{};

      PVOID blame_explode_bypass_{};
      inline void EnableBlameExplode(bool enable) const {
        *(unsigned short*)blame_explode_bypass_ = enable ? 0xE990 : 0x850F;
      }

      std::int64_t** script_globals_{};

      PVOID GtaThreadStart{};
      PVOID GtaThreadKill{};

      const char* gta_build_version_{};
      const char* gta_online_version_{};

      using fidevice_get_device_t = rage::fiDevice*(*)(const char* path, bool allow_root);
      fidevice_get_device_t FiDeviceGetDevice{};
      using fipackfile_ctor_t = rage::fiPackfile*(*)(rage::fiPackfile* that);
      fipackfile_ctor_t FiPackFileCtor{};
      using fipackfile_open_archive_t = bool(*)(rage::fiPackfile* that, const char* archive, bool b_true, int type, intptr_t very_false);
      fipackfile_open_archive_t FiPackFileOpenArchive{};
      using fipackfile_mount_t = bool(*)(rage::fiPackfile* that, const char* mount_point);
      fipackfile_mount_t FiPackFileMount{};
      using fipackfile_unmount = bool(*)(const char* mount_point);
      fipackfile_unmount FiPackFileUnmount{};
      uintptr_t fidevices_{};
      uint16_t* fidevices_len_{};
      rage::fiPackfile** fipackfile_instances_{};
    };
    inline Pointers* kPOINTERS{};
  }
#endif //GTABASE_MEM_POINTERS_HPP
