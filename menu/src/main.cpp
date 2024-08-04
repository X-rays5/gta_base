//
// Created by X-ray on 06/05/2023.
//

#include "main.hpp"
#include <memory>
#include <thread>
#include "util/thread_pool.hpp"
#include "render/renderer.hpp"
#include "render/thread.hpp"
#include "memory/pointers.hpp"
#include "hooking/hooking.hpp"
#include "ui/localization/manager.hpp"
#include "memory/signature/pattern.hpp"
#include "discord/rich_presence.hpp"
#include "ui/notification/manager.hpp"
#include <chrono>
#include <fmt/args.h>

std::atomic<bool> base::globals::kRUNNING = true;

namespace fmt {
  template <typename Context = std::format_context>
  class DynamicFormatArgsStore
    : public std::basic_format_args<Context> {
  private:
    using char_type = typename Context::char_type;

    template <typename T>
    struct need_copy {
      static constexpr detail::type mapped_type =
        detail::mapped_type_constant<T, Context>::value;

      enum {
        value = !(detail::is_reference_wrapper<T>::value ||
          std::is_same<T, basic_string_view<char_type>>::value ||
          std::is_same<T, detail::std_string_view<char_type>>::value ||
          (mapped_type != detail::type::cstring_type &&
            mapped_type != detail::type::string_type &&
            mapped_type != detail::type::custom_type))
      };
    };

    template <typename T>
    using stored_type = conditional_t<
      std::is_convertible<T, std::basic_string<char_type>>::value &&
      !detail::is_reference_wrapper<T>::value,
      std::basic_string<char_type>, T>;

    // Storage of basic_format_arg must be contiguous.
    std::vector<basic_format_arg<Context>> data_;
    std::vector<detail::named_arg_info<char_type>> named_info_;

    // Storage of arguments not fitting into basic_format_arg must grow
    // without relocation because items in data_ refer to it.
    detail::dynamic_arg_list dynamic_args_;

    friend class basic_format_args<Context>;

    auto get_types() const -> unsigned long long {
      return detail::is_unpacked_bit | data_.size() |
      (named_info_.empty()
         ? 0ULL
         : static_cast<unsigned long long>(detail::has_named_args_bit));
    }

    auto data() const -> const basic_format_arg<Context>* {
      return named_info_.empty() ? data_.data() : data_.data() + 1;
    }

    template <typename T>
    void emplace_arg(const T& arg) {
      data_.emplace_back(detail::make_arg<Context>(arg));
    }

    template <typename T>
    void emplace_arg(const detail::named_arg<char_type, T>& arg) {
      if (named_info_.empty()) {
        constexpr const detail::named_arg_info<char_type>* zero_ptr{nullptr};
        data_.insert(data_.begin(), {zero_ptr, 0});
      }
      data_.emplace_back(detail::make_arg<Context>(detail::unwrap(arg.value)));
      auto pop_one = [](std::vector<basic_format_arg<Context>>* data) {
        data->pop_back();
      };
      std::unique_ptr<std::vector<basic_format_arg<Context>>, decltype(pop_one)>
        guard{&data_, pop_one};
      named_info_.push_back({arg.name, static_cast<int>(data_.size() - 2u)});
      data_[0].value_.named_args = {named_info_.data(), named_info_.size()};
      guard.release();
    }

  public:
    constexpr DynamicFormatArgsStore() = default;

    template <typename T>
    void push_back(const T& arg) {
      if (detail::const_check(need_copy<T>::value))
        emplace_arg(dynamic_args_.push<stored_type<T>>(arg));
      else
        emplace_arg(detail::unwrap(arg));
    }

    template <typename T>
    void push_back(std::reference_wrapper<T> arg) {
      static_assert(
        need_copy<T>::value,
        "objects of built-in types and string views are always copied");
      emplace_arg(arg.get());
    }

    template <typename T>
    void push_back(const detail::named_arg<char_type, T>& arg) {
      const char_type* arg_name =
        dynamic_args_.push<std::basic_string<char_type>>(arg.name).c_str();
      if (detail::const_check(need_copy<T>::value)) {
        emplace_arg(
          fmt::arg(arg_name, dynamic_args_.push<stored_type<T>>(arg.value)));
      } else {
        emplace_arg(fmt::arg(arg_name, arg.value));
      }
    }

    void clear() {
      data_.clear();
      named_info_.clear();
      dynamic_args_ = detail::dynamic_arg_list();
    }

    /**
      \rst
      Reserves space to store at least *new_cap* arguments including
      *new_cap_named* named arguments.
      \endrst
    */
    void reserve(size_t new_cap, size_t new_cap_named) {
      FMT_ASSERT(new_cap >= new_cap_named,
                 "Set of arguments includes set of named arguments");
      data_.reserve(new_cap);
      named_info_.reserve(new_cap_named);
    }
  };
}

namespace base {
  namespace {
    std::unique_ptr<memory::Pointers> pointers_inst;
    std::unique_ptr<util::ThreadPool> thread_pool_inst;
    std::unique_ptr<hooking::Manager> hooking_inst;
    std::unique_ptr<ui::localization::Manager> localization_manager_inst;
    std::unique_ptr<discord::RichPresence> discord_rich_presence_inst;
    std::unique_ptr<render::Renderer> render_inst;
    std::unique_ptr<render::Thread> render_thread_manager_inst;
    std::unique_ptr<std::thread> render_thread_inst;
    std::unique_ptr<ui::notification::Manager> notification_manager_inst;
  }
}

class LifeTimeHelper {
public:
  enum Action {
    Init,
    Shutdown
  };

  using cb_t = std::function<void(Action)>;

public:
  LifeTimeHelper() = default;

  ~LifeTimeHelper() {
    LOG_INFO("[SHUTDOWN] LifeTimeHelper");
    for (auto it = cb_vec_.rbegin(); it != cb_vec_.rend(); ++it)
      (*it)(Shutdown);
  }

  void RunInit() const {
    LOG_INFO("[INIT] LifeTimeHelper");
    for (auto& cb : cb_vec_)
      cb(Init);
  }

  void AddCallback(const cb_t& cb) {
    cb_vec_.push_back(cb);
  }

private:
  std::vector<cb_t> cb_vec_;
};

#define MANAGER_PTR_LIFETIME(lifetime_helper_inst, init_name, manager_var, ...) lifetime_helper_inst->AddCallback([](LifeTimeHelper::Action action) { \
  if (action == LifeTimeHelper::Init) { \
    manager_var = std::make_unique<std::remove_reference_t<decltype(*manager_var)>>(__VA_ARGS__); \
    LOG_INFO("[INIT] {}", xorstr_(init_name)); \
  } \
  else { \
    manager_var.reset(); \
    LOG_INFO("[SHUTDOWN] {}", xorstr_(init_name)); \
  } \
})

void ThreadPoolLifetime(LifeTimeHelper* life_time_helper) {
  life_time_helper->AddCallback([&](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      base::thread_pool_inst = std::make_unique<std::remove_reference_t<decltype(*base::thread_pool_inst)>>(std::thread::hardware_concurrency() / 2);
      base::util::kTHREAD_POOL = base::thread_pool_inst.get();
      LOG_INFO("[INIT] {}", "ThreadPool");
    } else {
      base::util::kTHREAD_POOL = nullptr;
      base::thread_pool_inst.reset();
      LOG_INFO("[SHUTDOWN] {}", "ThreadPool");
    }
  });
}

void RenderThreadLifeTime(LifeTimeHelper* lifetime_helper) {
  lifetime_helper->AddCallback([](LifeTimeHelper::Action action) {
    if (action == LifeTimeHelper::Init) {
      LOG_INFO("[INIT] Renderer");
      base::render_inst = std::make_unique<base::render::Renderer>();
      base::render_thread_manager_inst = std::make_unique<base::render::Thread>();

      base::render::kTHREAD->AddRenderCallback(0, [](base::render::DrawQueueBuffer* buffer) {
        buffer->AddCommand(base::render::Text({0.5, 0.5}, ImColor(255, 0, 0), base::ui::localization::kMANAGER->Localize("text/hello_world"), 0.02F, false, true, true));
        buffer->AddCommand(base::render::RunRenderCode([] {
          if (ImGui::Begin("Hello World")) {
            ImGui::Text(base::ui::localization::kMANAGER->Localize("text/hello_world").c_str());
            ImGui::End();
          }
        }));
      });

      LOG_INFO("[INIT] Render thread");
      base::render_thread_inst = std::make_unique<std::thread>(base::render::Thread::RenderMain);
    } else {
      // First make sure it's actually waiting then unblock it.
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      base::render::kRENDERER->GetDrawQueueBuffer()->UnblockRenderThread();

      LOG_DEBUG("[SHUTDOWN] Stopping render thread...");
      if (base::render_thread_inst->joinable())
        base::render_thread_inst->join();
      LOG_INFO("[SHUTDOWN] Render thread stopped.");

      base::render_thread_manager_inst.reset();
      base::render_inst.reset();
    }
  });
}

int base::menu_main() {
  auto lifetime_helper = std::make_unique<LifeTimeHelper>();

  ThreadPoolLifetime(lifetime_helper.get());
  MANAGER_PTR_LIFETIME(lifetime_helper, "Pointers", pointers_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "HookingManager", hooking_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "LocalizationManager", localization_manager_inst);
  MANAGER_PTR_LIFETIME(lifetime_helper, "DiscordRichPresence", discord_rich_presence_inst);
  RenderThreadLifeTime(lifetime_helper.get());
  MANAGER_PTR_LIFETIME(lifetime_helper, "NotificationManager", notification_manager_inst);

  lifetime_helper->RunInit();

  hooking_inst->Enable();

  NOTIFY_INFO("Hello", "World!");
  NOTIFY_WARN("Hello", "World!");
  NOTIFY_ERR("Hello", "World!");

  LOG_INFO("Loaded");
  while (globals::kRUNNING) {
    if (GetAsyncKeyState(VK_END)) {
      globals::kRUNNING = false;
      break;
    }

    discord::kRICH_PRESENCE->Tick();
    std::this_thread::yield();
  }
  LOG_INFO("Unloading...");

  hooking_inst->Disable();

  lifetime_helper.reset();

  return 0;
}

#undef MANAGER_PTR_LIFETIME
