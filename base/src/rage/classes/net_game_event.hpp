//
// Created by X-ray on 08/08/22.
//

#pragma once
#ifndef GTA_BASE_NET_GAME_EVENT_HPP
#define GTA_BASE_NET_GAME_EVENT_HPP
#pragma once
#include "../../memory/pointers.hpp"
#include "../types.hpp"

#pragma pack(push, 1)
namespace rage {
class netPlayer;

enum class eNetMessage : uint32_t {
  CMsgInvalid = 0xFFFFF,
  CMsgSessionAcceptChat = 0x62,
  CMsgStartMatchCmd = 0x2D,
  CMsgSetInvitableCmd = 0x1F,
  CMsgSessionMemberIds = 0x23,
  CMsgRequestGamerInfo = 0x54,
  CMsgRemoveGamersFromSessionCmd = 0x53,
  CMsgNotMigrating = 0x35,
  CMsgMigrateHostResponse = 0x12,
  CMsgMigrateHostRequest = 0x66,
  CMsgJoinResponse = 0x2A,
  CMsgJoinRequest = 0x41,
  CMsgHostLeftWhilstJoiningCmd = 0x58,
  CMsgConfigResponse = 0x5F,
  CMsgConfigRequest = 0x48,
  CMsgChangeSessionAttributesCmd = 0x5A,
  CMsgAddGamerToSessionCmd = 0x64, // this is where send net info to lobby is called, among other things
  CMsgReassignResponse = 0x10,
  CMsgReassignNegotiate = 0x01,
  CMsgReassignConfirm = 0x26,
  CMsgPlayerData = 0x18,
  CMsgPackedReliables = 0x30,
  CMsgPackedCloneSyncACKs = 0x3B,
  CMsgNonPhysicalData = 0x16,
  CMsgNetArrayMgrUpdateAck = 0x5D,
  CMsgNetArrayMgrUpdate = 0x60,
  CMsgNetArrayMgrSplitUpdateAck = 0x25,
  CMsgScriptVerifyHostAck = 0x0B,
  CMsgScriptVerifyHost = 0x3E,
  CMsgScriptNewHost = 0x0E,
  CMsgScriptMigrateHostFailAck = 0x1A,
  CMsgScriptMigrateHost = 0x33,
  CMsgScriptLeaveAck = 0x40,
  CMsgScriptLeave = 0x17,
  CMsgScriptJoinHostAck = 0x4D,
  CMsgScriptJoinAck = 0x43,
  CMsgScriptJoin = 0x5C,
  CMsgScriptHostRequest = 0x67,
  CMsgScriptHandshakeAck = 0x5B,
  CMsgScriptHandshake = 0x57,
  CMsgScriptBotLeave = 0x2B, // unused?
  CMsgScriptBotJoinAck = 0x63, // unused?
  CMsgScriptBotJoin = 0x1C, // unused?
  CMsgScriptBotHandshakeAck = 0x31, // unused?
  CMsgScriptBotHandshake = 0x4B, // unused?
  CMsgPartyLeaveGame = 0x3D,
  CMsgPartyEnterGame = 0x1E,
  CMsgCloneSync = 0x4E, // aka clone_create, clone_sync etc.
  CMsgActivateNetworkBot = 0x65, // unused?
  CMsgRequestObjectIds = 0x29,
  CMsgInformObjectIds = 0x09,
  CMsgTextMessage = 0x24, // this one is for chat
  CMsgPlayerIsTyping = 0x61,
  CMsgPackedEvents = 0x4F, // aka received_event
  CMsgPackedEventReliablesCMsgs = 0x20,
  CMsgRequestKickFromHost = 0x0D,
  CMsgTransitionToGameStart = 0x50,
  CMsgTransitionToGameNotify = 0x02,
  CMsgTransitionToActivityStart = 0x06,
  CMsgTransitionToActivityFinish = 0x36,
  CMsgTransitionParameters = 0x3C,
  CMsgTransitionParameterString = 0x37,
  CMsgTransitionLaunchNotify = 0x1B,
  CMsgTransitionLaunch = 0x19,
  CMsgTransitionGamerInstruction = 0x14,
  CMsgTextMessage2 = 0x0A, // this one is for phone message
  CMsgSessionEstablishedRequest = 0x52,
  CMsgSessionEstablished = 0x07,
  CMsgRequestTransitionParameters = 0x42,
  CMsgRadioStationSyncRequest = 0x47,
  CMsgRadioStationSync = 0x46,
  CMsgPlayerCardSync = 0x3A,
  CMsgPlayerCardRequest = 0x6A,
  CMsgLostConnectionToHost = 0x81,
  CMsgKickPlayer = 0x34, // host kick
  CMsgDebugStall = 0x7E, // unused?
  CMsgCheckQueuedJoinRequestReply = 0x59,
  CMsgCheckQueuedJoinRequest = 0x51,
  CMsgBlacklist = 0x0C,
  CMsgRoamingRequestBubbleRequiredResponse = 0x83,
  CMsgRoamingRequestBubbleRequiredCheck = 0x82,
  CMsgRoamingRequestBubble = 0x2E,
  CMsgRoamingJoinBubble = 0x4C,
  CMsgRoamingJoinBubbleAck = 0x3F,
  CMsgRoamingInitialBubble = 0x32,
  CMsgVoiceStatus = 0x03,
  CMsgTextChatStatus = 0x00,
  CMsgJoinResponse2 = 0x08,
  CMsgJoinRequest2 = 0x68,
  CMsgNetTimeSync = 0x38, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 37
  CMsgNetComplaint = 0x55, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 54
  CMsgNetLagPing = 0x27, // unused? ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 26
  CMsgSearchResponse = 0x6B, // unused? ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 6A
  CMsgSearchRequest = 0x05, // unused?
  CMsgQosProbeResponse = 0x2C, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 2B
  CMsgQosProbeRequest = 0x1D, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 1C
  CMsgCxnRelayAddressChanged = 0x49, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 48
  CMsgCxnRequestRemoteTimeout = 0x2F, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 2E
  CMsgSessionDetailRequest = 0x22, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 21
  CMsgSessionDetailResponse = 0x13, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 12
  CMsgKeyExchangeOffer = 0x0F, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 0E (last result)
  CMsgKeyExchangeAnswer = 0x44, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 43
  CMsg_0x87 = 0x87,
  CMsg_0x88 = 0x88,
  CMsg_0x80 = 0x80,
  CMsg_0x28 = 0x28,
  CMsg_0x11 = 0x11,
  CMsg_0x45 = 0x45,
  CMsg_0x89 = 0x89,
  CMsg_0x86 = 0x86,
};
namespace netConnection {
class InFrame {
 public:
  virtual ~InFrame() = default;

  virtual void destroy() = 0;
  virtual uint32_t get_type() = 0;
  virtual uint32_t _0x18() = 0;

  char pad_0008[56]; //0x0008
  uint32_t m_msg_id; //0x0040
  char pad_0044[4]; //0x0044
  InFrame *m_this; //0x0048
  char pad_0050[40]; //0x0050
  uint32_t m_length; //0x0078
  char pad_007C[4]; //0x007C
  void *m_data; //0x0080
};
}

class CEventNetwork {
 public:
  virtual ~CEventNetwork() = default;
  virtual void unk_0x8() = 0;
  virtual void unk_0x10() = 0;
  virtual __int64 get_type() = 0;
  virtual void unk_0x20() = 0;
  virtual void unk_0x28() = 0;
  virtual bool get_extra_information(__int64 *info_array, int check) = 0;
  virtual void unk_0x38() = 0;
};

class netGameEvent {
 public:
  virtual ~netGameEvent() = default;

  virtual const char *get_name() { return 0; };
  virtual bool is_in_scope(netPlayer *player) { return 0; };
  virtual bool time_to_resend(std::uint32_t time) { return 0; };
  virtual bool can_change_scope() { return 0; };

  virtual void prepare_data(datBitBuffer *buffer, netPlayer *source_player, netPlayer *target_player) {};
  virtual void handle_data(datBitBuffer *buffer, netPlayer *source_player, netPlayer *target_player) {};

  virtual bool decide(netPlayer *source_player, netPlayer *target_player) { return 0; };

  virtual void prepare_reply(datBitBuffer *buffer, netPlayer *reply_player) {};
  virtual void handle_reply(datBitBuffer *buffer, netPlayer *souce_player) {};

  virtual void prepare_extra_data(datBitBuffer *buffer, bool is_reply, netPlayer *player, netPlayer *player2) {};
  virtual void handle_extra_data(datBitBuffer *buffer, bool is_reply, netPlayer *player, netPlayer *player2) {};

 private:
  virtual void unk_0x60() {};
  virtual void unk_0x68() {};
  virtual void unk_0x70() {};
  virtual void unk_0x78() {};
 public:
  virtual bool operator==(netGameEvent const &other) { return 0; };
  virtual bool operator!=(netGameEvent const &other) { return 0; };

  virtual bool must_persist() { return 0; };
  virtual bool must_persist_when_out_of_scope() { return 0; };
  virtual bool has_timed_out() { return 0; };
 public:
  std::uint16_t m_id;          // 0x08
  bool m_requires_reply;       // 0x0A
 private:
  char m_padding1[0x05];       // 0x0B
 public:
  netPlayer *m_source_player;  // 0x10
  netPlayer *m_target_player;  // 0x18
  std::uint32_t m_resend_time; // 0x20
 private:
  std::uint16_t m_0x24;        // 0x24
  std::uint8_t m_0x26;         // 0x26
  std::uint8_t m_0x27;         // 0x27
  std::uint32_t m_0x28;        // 0x28
  char m_padding2[0x04];
};
}

class CScriptedGameEvent : public rage::netGameEvent {
 public:
  char m_padding[0x40];      // 0x30
  std::int64_t m_args[54];   // 0x70
  std::uint32_t m_bitset;    // 0x220
  std::uint32_t m_args_size; // 0x224
};

class CNetworkIncrementStatEvent : public rage::netGameEvent {
 public:
  rage::Hash m_stat;    // 0x30
  std::uint32_t m_amount; // 0x34
};
#pragma pack(pop)
#endif //GTA_BASE_NET_GAME_EVENT_HPP
