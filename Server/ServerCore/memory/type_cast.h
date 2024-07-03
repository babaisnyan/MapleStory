// ReSharper disable CppInconsistentNaming

#pragma once
#include "types.h"

#pragma region TypeList

template <typename... T>
struct TypeList;

template <typename T, typename U>
struct TypeList<T, U> {
  using Head = T;
  using Tail = U;
};

template <typename T, typename... U>
struct TypeList<T, U...> {
  using Head = T;
  using Tail = TypeList<U...>;
};

#pragma endregion

#pragma region Length

template <typename T>
struct Length;

template <>
struct Length<TypeList<>> {
  static constexpr size_t value = 0;
};

template <typename T, typename... U>
struct Length<TypeList<T, U...>> {
  static constexpr size_t value = Length<TypeList<U...>>::value + 1;
};

#pragma endregion

#pragma region TypeAt

template <typename TL, int32_t Index>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
  using Type = Head;
};

template <typename Head, typename... Tail, int32_t Index>
struct TypeAt<TypeList<Head, Tail...>, Index> {
  using Type = typename TypeAt<TypeList<Tail...>, Index - 1>::Type;
};

#pragma endregion

#pragma region IndexOf

template <typename TL, typename T>
struct IndexOf;

template <typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T> {
  static constexpr int32_t value = 0;
};

template <typename T>
struct IndexOf<TypeList<>, T> {
  static constexpr int32_t value = -1;
};

template <typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T> {
private:
  static constexpr int32_t temp = IndexOf<TypeList<Tail...>, T>::value;

public:
  static constexpr int32_t value = temp == -1 ? -1 : temp + 1;
};

#pragma endregion

#pragma region Conversion

template <typename From, typename To>
class Conversion {
private:
  using Small = __int8;
  using Big = __int32;

  static Small Test(const To&) {
    return 0;
  }

  static Big Test(...) {
    return 0;
  }

  static From MakeFrom() {
    return 0;
  }

public:
  static constexpr bool exists = sizeof(Test(MakeFrom())) == sizeof(Small);
};


#pragma endregion

#pragma region TypeCast

template <int32_t V>
struct IntToType {
  static constexpr int32_t value = V;
};

template <typename TL>
class TypeConversion {
public:
  static constexpr int32_t length = Length<TL>::value;

  TypeConversion() {
    MakeTable(IntToType<0>(), IntToType<0>());
  }

  template <int32_t I, int32_t J>
  static void MakeTable(IntToType<I>, IntToType<J>) {
    using From = typename TypeAt<TL, I>::Type;
    using To = typename TypeAt<TL, J>::Type;

    if (Conversion<const From*, const To*>::exists) {
      convert[I][J] = true;
    } else {
      convert[I][J] = false;
    }

    MakeTable(IntToType<I>(), IntToType<J + 1>());
  }

  template <int32_t I>
  static void MakeTable(IntToType<I>, IntToType<length>) {
    MakeTable(IntToType<I + 1>(), IntToType<0>());
  }

  template <int32_t J>
  static void MakeTable(IntToType<length>, IntToType<J>) {}

  static inline bool CanConvert(int32_t from, int32_t to) {
    static TypeConversion conversion;
    return convert[from][to];
  }

public:
  static bool convert[length][length];
};

template <typename TL>
bool TypeConversion<TL>::convert[length][length];

template <typename To, typename From>
To TypeCast(From* from) {
  if (from == nullptr) {
    return nullptr;
  }

  using TL = typename From::TL;

  if (TypeConversion<TL>::CanConvert(from->_type_id, IndexOf<TL, std::remove_pointer_t<To>>::value)) {
    return static_cast<To>(from);
  }

  return nullptr;
}

template <typename To, typename From>
std::shared_ptr<To> TypeCast(std::shared_ptr<From> from) {
  if (from == nullptr) {
    return nullptr;
  }

  using TL = typename From::TL;

  if (TypeConversion<TL>::CanConvert(from->_type_id, IndexOf<TL, std::remove_pointer_t<To>>::value)) {
    return std::static_pointer_cast<To>(from);
  }

  return nullptr;
}

template <typename To, typename From>
bool CanCast(From* from) {
  if (from == nullptr) {
    return false;
  }

  using TL = typename From::TL;

  return TypeConversion<TL>::CanConvert(from->_type_id, IndexOf<TL, std::remove_pointer_t<To>>::value);
}

template <typename To, typename From>
bool CanCast(std::shared_ptr<From> from) {
  if (from == nullptr) {
    return false;
  }

  using TL = typename From::TL;

  return TypeConversion<TL>::CanConvert(from->_type_id, IndexOf<TL, std::remove_pointer_t<To>>::value);
}

#pragma endregion

#define DECLARE_TL using TL = TL; int32 _type_id;
#define INIT_TL(Type) _type_id = IndexOf<TL, Type>::value