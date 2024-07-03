#pragma once

template <typename T>
class LockQueue {
public:
  void Push(T item) {
    WRITE_LOCK;
    _queue.push(item);
  }

  std::optional<T> Pop() {
    WRITE_LOCK;
    if (_queue.empty()) {
      return std::nullopt;
    }

    T item = _queue.front();
    _queue.pop();
    return item;
  }

  void PopAll(Vector<T>& out) {
    WRITE_LOCK;

    while (!_queue.empty()) {
      out.push_back(_queue.front());
      _queue.pop();
    }
  }

  void Clear() {
    WRITE_LOCK;
    _queue = Queue<T>();
  }

private
:
  USE_LOCK;
  Queue<T> _queue;
};
