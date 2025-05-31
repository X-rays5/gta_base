//
// Created by X-ray on 31/05/2025.
//

#ifndef BASE_SCRIPT_HPP_05162156
#define BASE_SCRIPT_HPP_05162156

namespace base::menu::scripts {
  class BaseScript {
  public:
    enum class Type {
      MainScriptThread,
      GameRenderThread,
    };

  public:
    virtual ~BaseScript() = default;

    virtual void Init() = 0;
    virtual void Tick() = 0;

    virtual Type GetScriptType() const = 0;
  };
}

#endif //BASE_SCRIPT_HPP_05162156
