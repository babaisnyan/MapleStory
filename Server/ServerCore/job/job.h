#pragma once

#include <functional>

using CallBackType = std::function<void()>;

class Job {
public:
  explicit Job(CallBackType&& call_back) : _callback(std::move(call_back)) {}

  template <typename T, typename Ret, typename... Args>
  Job(std::shared_ptr<T> obj, Ret (T::*func)(Args...), Args&&... args) {
    _callback = [obj, func, args...] {
      (obj.get()->*func)(args...);
    };
  }

  void Execute() const {
    _callback();
  }

private:
  CallBackType _callback;
};
