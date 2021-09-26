#pragma once

#include <functional>
#include <memory>

namespace utilsLib
{

template <typename TPtr>
class Injector;

template <typename TPtr>
class PointerTypeDefinition final
{
public:
  ~PointerTypeDefinition() = delete;

  PointerTypeDefinition() = delete;
  PointerTypeDefinition(const PointerTypeDefinition&) = delete;
  PointerTypeDefinition(PointerTypeDefinition&&) = delete;
  PointerTypeDefinition& operator=(const PointerTypeDefinition&) = delete;
  PointerTypeDefinition& operator=(PointerTypeDefinition&&) = delete;

};

template <>
class PointerTypeDefinition<std::shared_ptr<void>> final
{
public:
  template<class Type>
  using TypePtr        = std::shared_ptr<Type>;
  using GenericTypePtr = std::shared_ptr<void>;
  using GenericWeakPtr = std::weak_ptr<void>;

  template<typename Type>
  static std::function<GenericTypePtr(Injector<GenericTypePtr>&)> getNewInstanceFunc()
  {
    return [](Injector<GenericTypePtr>& injector)
    {
      return std::make_shared<Type>(injector);
    };
  }

  template<typename Type>
  static TypePtr<Type> pointer_cast(const GenericTypePtr& ref)
  {
    return std::reinterpret_pointer_cast<Type>(ref);
  }

  template<typename Type>
  static bool isNotNull(const GenericTypePtr& ref)
  {
    return ref != nullptr;
  }

  static bool isExpired(const GenericWeakPtr& ref)
  {
    return ref.expired();
  }

  static long getUseCount(const GenericWeakPtr& ref)
  {
    return ref.use_count();
  }
};

}
