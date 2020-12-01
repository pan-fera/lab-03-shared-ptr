//
// Created by hacker on 28.11.2020.
//
// Copyright 2020 hacker

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <algorithm>    // std::swap
#include <utility>
template <typename T>
class SharedPtr {
 public:
  SharedPtr():m_ptr(nullptr), m_count(nullptr){}
   explicit SharedPtr(T* ptr):m_ptr(ptr), m_count(new std::atomic_uint(1)){}
  //консруктор копирования
  SharedPtr(const SharedPtr& r):m_ptr(r.m_ptr){
    ++*r.m_count;
    m_count = r.m_count;
  }
  //конструктор перемещения значения r-value
  SharedPtr(SharedPtr&& r):m_ptr(r.m_ptr), m_count(r.m_count){
    r.m_ptr = nullptr;
    r.m_count = nullptr;
  }
  ~SharedPtr() {
    reset();
  }
  //оператор копирования
  auto operator=(const SharedPtr& r) -> SharedPtr&{
    if (this != &r) {
      reset();
      m_ptr = r.m_ptr;

      if (r.m_count != nullptr)
      ++(*r.m_count);  // nullptr

      m_count = r.m_count;
    }else{
      throw std::runtime_error("Self-copying");
    }

    return *this;
  }
  auto operator=(SharedPtr&& r) -> SharedPtr&{
    if (this != &r) {
    reset();
    m_ptr = r.m_ptr;
    m_count = r.m_count;
    r.m_ptr = nullptr;
    r.m_count = nullptr;
    }else{
      throw std::runtime_error("Self-assignment");
    }
    return *this;
  }

  // проверяет, указывает ли указатель на объект
  operator bool() const{
    return (m_ptr!= nullptr);
  }
  auto operator*() const -> T&{
    if (m_ptr == nullptr)
      throw std::runtime_error("Ptr is nullptr");
    else
      return *m_ptr;
  }
  auto operator->() const -> T*{
    if (m_ptr == nullptr)
      throw std::runtime_error("Ptr is nullptr");
    else
      return m_ptr;
  }

  auto get() -> T*{
    return m_ptr;
  }
  void reset(){
    if (m_count != nullptr){
      --*m_count;
      if (*m_count == 0){
        m_ptr->~T();
        delete m_count;
      }
    }
    m_ptr = nullptr;
    m_count = nullptr;
  }
  void reset(T* ptr){
    reset();
    m_ptr = ptr;
    m_count = new std::atomic_uint(1);
  }
  void swap(SharedPtr& r){
    std::swap(m_count, r.m_count);
    std::swap(m_ptr, r.m_ptr);
  }
  // возвращает количество объектов SharedPtr,
  // которые ссылаются на тот же управляемый объект
auto use_count() const -> size_t{
  if (m_count != nullptr)
    return *m_count;
  else
    return 0;
}

protected:
  T* m_ptr;
  std::atomic_uint* m_count;
};
#endif  // INCLUDE_SHAREDPTR_HPP_
