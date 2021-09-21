#pragma once

#include <memory>
#include <typeindex>
#include <vector>

#include <utilsLib/InjectionContainer.h>
#include <utilsLib/PointerTypeDefinition.h>
#include <utilsLib/Utils.h>

namespace utilsLib
{

template <typename TPtr>
class Injector final
{
#ifdef _DEBUG
private:
  int                          m_depth;
  std::vector<std::type_index> m_creationOrder;

  void traceInstance(const std::type_index& tid)
  {
    for (const auto& it : m_creationOrder)
    {
      if (it == tid)
      {
        UTILS_LOG_WARN("Circular dependency detected!");
      }
    }

    char buffer[256];
    sprintf_s(buffer, "%*s%s%s", (m_depth)* 2, "", "Injecting ", tid.name());

    UTILS_LOG_INFO(std::string(buffer));
  }

  TPtr createInstance(
    const std::type_index& tid,
    const typename InjectionContainer<TPtr>::NewInstanceFunc& newInstanceFunc)
  {
    traceInstance(tid);

    m_creationOrder.push_back(tid);

    m_depth++;
    auto instance = newInstanceFunc(*this);
    m_depth--;

    m_creationOrder.pop_back();

    return instance;
  }
#endif

public:
  explicit Injector(InjectionContainer<TPtr>& container)
    :
#ifdef _DEBUG
    m_depth(0),
#endif
    m_container(container)
  {
  }

  ~Injector() = default;

  Injector(const Injector<TPtr>&) = delete;
  Injector(Injector<TPtr>&&) = delete;
  Injector<TPtr>& operator=(const Injector<TPtr>&) = delete;

  template<typename Type>
  std::vector<typename PointerTypeDefinition<TPtr>::template TypePtr<Type>> injectAll()
  {
    const std::type_index tid = typeid(Type);
    std::vector<typename PointerTypeDefinition<TPtr>::template TypePtr<Type>> resultInstances;

    auto& interfaceSingletons = m_container.getRegisteredInterfaceSingletonTypes();
    if (interfaceSingletons.count(tid) > 0)
    {
      auto& infos = interfaceSingletons.at(tid);
      for (auto& info : infos)
      {
        resultInstances.push_back(getOrCreateSingletonInstance<Type>(info.first, info.second));
      }

      return resultInstances;
    }

    const auto& interfaceTypes = m_container.getRegisteredInterfaceTypes();
    if (interfaceTypes.count(tid) > 0)
    {
      auto& funcs = interfaceTypes.at(tid);
      for (const auto& func : funcs)
      {
        resultInstances.push_back(createTypeInstance<Type>(func.first, func.second));
      }

      return resultInstances;
    }

    UTILS_LOG_ERROR(std::string("Interface ") + tid.name() + "was not registered");

    assert(false);
    return resultInstances;
  }
  
  template<typename Type>
  typename PointerTypeDefinition<TPtr>::template TypePtr<Type> inject()
  {
    const std::type_index tid = typeid(Type);

    auto& interfaceSingletons = m_container.getRegisteredInterfaceSingletonTypes();
    if (interfaceSingletons.count(tid) > 0)
    {
      auto& instances = interfaceSingletons.at(tid);
      if (instances.size() > 1)
      {
        assertAndLogMoreThanOneImplementationsError(tid);
        return PointerTypeDefinition<TPtr>::template TypePtr<Type>();
      }

      const auto it = instances.begin();
      return getOrCreateSingletonInstance<Type>(it->first, it->second);
    }

    const auto& interfaceTypes = m_container.getRegisteredInterfaceTypes();
    if (interfaceTypes.count(tid) > 0)
    {
      const auto& instances = interfaceTypes.at(tid);
      if (instances.size() > 1)
      {
        assertAndLogMoreThanOneImplementationsError(tid);
        return PointerTypeDefinition<TPtr>::template TypePtr<Type>();
      }

      const auto it = instances.begin();
      return createTypeInstance<Type>(it->first, it->second);
    }

    auto& singletons = m_container.getRegisteredSingletonTypes();
    if (singletons.count(tid) > 0)
    {
      auto& info = singletons.at(tid);
      return getOrCreateSingletonInstance<Type>(tid, info);
    }
    
    const auto& types = m_container.getRegisteredTypes();
    if (types.count(tid) > 0)
    {
      return createTypeInstance<Type>(tid, types.at(tid));
    }

    UTILS_LOG_ERROR(std::string("Type ") + tid.name() + " was not registered");

    assert(false);
    return PointerTypeDefinition<TPtr>::template TypePtr<Type>();
  }

private:
  InjectionContainer<TPtr>& m_container;

  static void assertAndLogMoreThanOneImplementationsError(const std::type_index& tid)
  {
    UTILS_LOG_ERROR(std::string("More than one interface implementations registered for ") + tid.name());
    assert(false);
  }

  template <typename Type>
  typename PointerTypeDefinition<TPtr>::template TypePtr<Type> createTypeInstance(
    const std::type_index& tid, const typename InjectionContainer<TPtr>::NewInstanceFunc& newInstanceFunc)
  {
#ifdef _DEBUG
    return PointerTypeDefinition<TPtr>::template pointer_cast<Type>(createInstance(tid, newInstanceFunc));
#else
    return PointerTypeDefinition<TPtr>::template pointer_cast<Type>(newInstanceFunc(*this));
#endif
  }
  
  template <typename Type>
  typename PointerTypeDefinition<TPtr>::template TypePtr<Type> getOrCreateSingletonInstance(
    const std::type_index& tid, typename InjectionContainer<TPtr>::SingletonTypeInfo& info)
  {
    if (PointerTypeDefinition<TPtr>::template isNotNull<Type>(info.instance))
    {
#ifdef _DEBUG
      traceInstance(tid);
#endif
      return PointerTypeDefinition<TPtr>::template pointer_cast<Type>(info.instance);
    }

#ifdef _DEBUG
    info.instance = createInstance(tid, info.newInstanceFunc);
#else
    info.instance = info.newInstanceFunc(*this);
#endif
    return PointerTypeDefinition<TPtr>::template pointer_cast<Type>(info.instance);
  }
};

using StdInjector = Injector<std::shared_ptr<void>>;

}