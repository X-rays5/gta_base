//
// Created by X-ray on 20/12/2025.
//

#pragma once
#include <cstdint>

namespace rage::script {
  class ProgramBase {
  public:
    virtual void ShutdownClass() = 0;
    virtual void SetHandleIndex(uint32_t index) = 0;
    virtual void Validate(uint32_t arg) = 0;
    virtual void InitClass(int arg) = 0;

    virtual uint32_t GetHandleIndex() const {
      return 0;
    }

    uintptr_t LookupMetaData(const uint32_t index) const {
      const uintptr_t current = reinterpret_cast<uintptr_t>(this);

      const uintptr_t nextPage = *reinterpret_cast<uintptr_t*>(current + 8);
      if (nextPage == 0)
        return 0;

      const uint8_t pageStatus = *reinterpret_cast<uint8_t*>(nextPage + 11);
      if (pageStatus == 0)
        return 0;

      const uintptr_t* metadataStart = reinterpret_cast<uintptr_t*>(nextPage + 16);
      const uintptr_t metadataCount = *reinterpret_cast<uintptr_t*>(nextPage + 24);

      for (uintptr_t i = 0; i < metadataCount; ++i) {
        const uintptr_t currentMeta = *(metadataStart + i);

        const uint32_t metaIndex = *reinterpret_cast<uint32_t*>(currentMeta + 8);
        if (metaIndex == index) {
          return currentMeta;
        }
      }

      return 0;
    }

    static bool HasPageMap() {
      return false;
    }

    bool MapContainsPointer(void* ptr) const {
      const uintptr_t basePage = reinterpret_cast<uintptr_t>(this);
      const uintptr_t nextPage = *reinterpret_cast<uintptr_t*>(basePage + 8);
      if (nextPage == 0)
        return false;

      const uint8_t pageStatus = *reinterpret_cast<uint8_t*>(nextPage + 11);
      if (pageStatus == 0)
        return false;

      const auto data = reinterpret_cast<uintptr_t*>(nextPage + 16);
      const uintptr_t count = *reinterpret_cast<uintptr_t*>(nextPage + 8);

      for (uintptr_t i = 0; i < count; ++i) {
        const uintptr_t elementPage = *(data + i);
        if (elementPage >= reinterpret_cast<uintptr_t>(ptr) && elementPage < reinterpret_cast<uintptr_t>(ptr) + 24) {
          return true;
        }
      }

      return false;
    }

  private:
    uint32_t map_size_;
  };

  class pgBaseRefCounted : public ProgramBase {
  public:
    virtual ~pgBaseRefCounted() = default;
  };

  class pgBaseMetaDataType {
  public:
    static pgBaseMetaDataType* first_;

    pgBaseMetaDataType(const uint32_t id, const uintptr_t value) :
      id_(id),
      value_(value),
      next_(nullptr) {
      if (!first_) {
        first_ = this;
      }
    }

    ~pgBaseMetaDataType() {
      if (first_ == this) {
        first_ = next_;
      } else {
        pgBaseMetaDataType* current = first_;
        while (current && current->next_ != this) {
          current = current->next_;
        }
        if (current) {
          current->next_ = next_;
        }
      }
    }

    static pgBaseMetaDataType* Lookup(const uint32_t id) {
      pgBaseMetaDataType* current = first_;
      while (current != nullptr) {
        if (current->id_ == id) {
          return current;
        }
        current = current->next_;
      }
      return nullptr;
    }

  private:
    uint32_t id_;
    uintptr_t value_;
    pgBaseMetaDataType* next_;
  };
}
