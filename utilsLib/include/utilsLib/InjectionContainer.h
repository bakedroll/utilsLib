#pragma once

#include <typeindex>
#include <map>
#include <cassert>
#include <functional>

#include <utilsLib/PointerTypeDefinition.h>
#include <utilsLib/Utils.h>

namespace utilsLib
{

template <typename TPtr>
class InjectionContainer final
{
public:
  using ReferencedPtr   = TPtr;
  using PtrTypeDef      = PointerTypeDefinition<TPtr>;
  using NewInstanceFunc = std::function<TPtr(Injector<TPtr>&)>;

  struct SingletonTypeInfo
  {
    ReferencedPtr   instance;
    NewInstanceFunc newInstanceFunc;
  };

  using TypeNewInstanceFuncMap = std::map<std::type_index, NewInstanceFunc>;
  using SingletonTypeInfosMap  = std::map<std::type_index, SingletonTypeInfo>;

  using InterfaceTypesSetMap           = std::map<std::type_index, TypeNewInstanceFuncMap>;
  using InterfaceSingletonTypeInfosMap = std::map<std::type_index, SingletonTypeInfosMap>;

  InjectionContainer() = default;
  ~InjectionContainer() = default;

  template<typename Type>
  void registerType()
  {
    const std::type_index tid = typeid(Type);
    assert(m_registeredTypes.count(tid) == 0);

    m_registeredTypes[tid] = PtrTypeDef::template getNewInstanceFunc<Type>();
  }

  template<typename Type>
  void registerSingletonType()
  {
    const std::type_index tid = typeid(Type);
    assert(m_registeredSingletonTypes.count(tid) == 0);

    m_registeredSingletonTypes[tid] = { ReferencedPtr(), PtrTypeDef::template getNewInstanceFunc<Type>() };
  }

  template<typename Interface, typename Type,
           typename = typename std::enable_if<std::is_base_of<Interface, Type>::value>::type>
  void registerInterfaceType()
  {
    const std::type_index iid = typeid(Interface);
    const std::type_index tid = typeid(Type);

    m_registeredInterfaceTypes[iid][tid] = PtrTypeDef::template getNewInstanceFunc<Type>();
  }

  template<typename Interface, typename Type,
           typename = typename std::enable_if<std::is_base_of<Interface, Type>::value>::type>
  void registerSingletonInterfaceType()
  {
    const std::type_index iid = typeid(Interface);
    const std::type_index tid = typeid(Type);

    m_registeredInterfaceSingletonTypes[iid][tid] = { ReferencedPtr(), PtrTypeDef::template getNewInstanceFunc<Type>() };
  }

  const TypeNewInstanceFuncMap& getRegisteredTypes() const
  {
    return m_registeredTypes;
  }

  SingletonTypeInfosMap& getRegisteredSingletonTypes()
  {
    return m_registeredSingletonTypes;
  }

  const InterfaceTypesSetMap& getRegisteredInterfaceTypes() const
  {
    return m_registeredInterfaceTypes;
  }

  InterfaceSingletonTypeInfosMap& getRegisteredInterfaceSingletonTypes()
  {
    return m_registeredInterfaceSingletonTypes;
  }

  void clear()
  {
#ifdef _DEBUG
    SingletonObservers singletonObservers;
    singletonObservers.reserve(m_registeredSingletonTypes.size());

    for (const auto& singleton : m_registeredSingletonTypes)
    {
      singletonObservers.push_back({ singleton.second.instance, singleton.first.name() });
    }

    for (const auto& singletons : m_registeredInterfaceSingletonTypes)
    {
      for (const auto& singleton : singletons.second)
      {
        singletonObservers.push_back({ singleton.second.instance, singleton.first.name() });
      }
    }
#endif

    m_registeredTypes.clear();
    m_registeredSingletonTypes.clear();
    m_registeredInterfaceSingletonTypes.clear();
    m_registeredInterfaceTypes.clear();

#ifdef _DEBUG
    for (const auto& it : singletonObservers)
    {
      if (!PointerTypeDefinition<TPtr>::isExpired(it.ptr))
      {
        const auto refCount = PointerTypeDefinition<TPtr>::getUseCount(it.ptr);
        UTILS_LOG_WARN(it.name + " has " + std::to_string(refCount) + " references left");
      }
    }
#endif
  }

private:
  struct SingletonObserver
  {
    typename PointerTypeDefinition<TPtr>::GenericWeakPtr ptr;
    std::string                                          name;
  };

  using SingletonObservers = std::vector<SingletonObserver>;

  TypeNewInstanceFuncMap m_registeredTypes;
  SingletonTypeInfosMap  m_registeredSingletonTypes;

  InterfaceTypesSetMap           m_registeredInterfaceTypes;
  InterfaceSingletonTypeInfosMap m_registeredInterfaceSingletonTypes;

};

using StdInjectionContainer = InjectionContainer<std::shared_ptr<void>>;

}