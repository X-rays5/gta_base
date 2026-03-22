//
// Created by X-ray on 20/12/2025.
//

#pragma once

namespace rage::script {
  class Thread {
  public:
    enum class State
    {
      IDLE,
      RUNNING,
      KILLED,
      PAUSED,
      UNK4
    };

    class Context
    {
    public:
      std::uint32_t thread_id;       // 0x00
      std::uint64_t script_hash;     // 0x08 TODO: is this still the script hash in enhanced? why is it an 8-byte value now?
      State state;                  // 0x10
      std::uint32_t program_counter; // 0x14
      std::uint32_t frame_pointer;   // 0x18
      std::uint32_t stack_pointer;   // 0x1C
      float timer_a;                 // 0x20
      float timer_b;                 // 0x24
      float wait_timer;              // 0x28
      char pad1[0x2C];          // 0x2C
      std::uint32_t stack_size;      // 0x58
      char pad2[0x54];               // 0x2C don't really need this fields after this
    };
    static_assert(sizeof(Context) == 0xB0);


    virtual ~Thread() = default;
    virtual void Reset(std::uint64_t script_hash, void* args, std::uint32_t arg_count) = 0;
    virtual State RunImpl() = 0;
    virtual State Run() = 0;
    virtual void Kill() = 0;
    virtual void GetInfo(void* info) = 0; // new in Enhanced, more research needed
    // I'm not sure what's going on with this func, best to not touch it

    Context ctx;             // 0x08
    void* stack;                 // 0xB8
    char pad1[0x4];               // 0xC0
    std::uint32_t parameter_size; // 0xC4
    std::uint32_t parameter_loc;  // 0xC8
    char pad2[0x4];              // 0xCC
    char error_message[128];      // 0xD0 finally works now (size 124, 4 padding)
    std::uint32_t script_hash;    // 0x150
    char script_name[64];         // 0x154

    static Thread* GetRunningThread();
  };
  static_assert(sizeof(Thread) == 0x198);
}