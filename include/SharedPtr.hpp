//
// Created by hacker on 28.11.2020.
//

#ifndef SHARED_PTR_SHAREDPTR_HPP
#define SHARED_PTR_SHAREDPTR_HPP

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <algorithm>    // std::swap

template <typename T>
class SharedPtr {
 public:
  SharedPtr():m_ptr(nullptr), m_count(nullptr){
    std::cout << "Конструктор" <<std::endl;
  };
  SharedPtr(T* ptr):m_ptr(ptr), m_count(new std::atomic_uint(1)){std::cout << "Конструктор ptr" <<std::endl;};
  //консруктор копирования
  SharedPtr(const SharedPtr& r):m_ptr(r.m_ptr){
    ++*r.m_count;
    m_count=r.m_count;
    std::cout << "Конструктор копирования" <<std::endl;
                                                  };
  //конструктор перемещения значения r-value
  SharedPtr(SharedPtr&& r):m_ptr(r.m_ptr), m_count(r.m_count){
    r.m_ptr = nullptr;
    r.m_count = nullptr;
    std::cout << "Конструктор перемещения" <<std::endl;
  };
  ~SharedPtr() {
    std::cout<<"Деструктор"<<std::endl;
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

      std::cout << "Перегруженный оператор копирования" <<std::endl;
    }
    else {
      std::cout << "Самокопирование" << std::endl;
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
    }
    else {
      std::cout << "Самоприсваивание" << std::endl;
      throw std::runtime_error("Self-assignment");
    }
    return *this;
  }

  // проверяет, указывает ли указатель на объект
  operator bool() const{
    return (m_ptr!= nullptr);
  }
  auto operator*() const -> T&{
    if(m_ptr == nullptr)
      throw std::runtime_error("Ptr is nullptr");
    else
      return *m_ptr;
  }
  auto operator->() const -> T*{
    return m_ptr;
  }

  auto get() -> T*{
    return m_ptr;
  }
  void reset(){
    if(m_count != nullptr){
      --*m_count;
      if (*m_count == 0){
        m_ptr->~T();
        delete m_ptr;
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
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
auto use_count() const -> size_t{
  if(m_count != nullptr)
    return *m_count;
  else
    return 0;
}

protected:
  T* m_ptr;
  std::atomic_uint* m_count;
};
#endif  // SHARED_PTR_SHAREDPTR_HPP
