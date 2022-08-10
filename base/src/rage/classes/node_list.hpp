//
// Created by X-ray on 08/08/22.
//

#pragma once
#ifndef GTA_BASE_NODE_LIST_HPP
#define GTA_BASE_NODE_LIST_HPP
#include "base.hpp"

namespace rage
{
  template <typename T, typename Base = datBase>
  class atDNode : public Base
  {
  public:
    T m_data;
    void *m_unk;
    atDNode<T, Base> *m_next;
  };

  template <typename Node>
  class atDList
  {
  public:
    Node *m_head;
    Node *m_tail;
  };
}

#endif //GTA_BASE_NODE_LIST_HPP
