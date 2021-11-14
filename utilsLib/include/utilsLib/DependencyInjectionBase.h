#pragma once

#include <utilsLib/Injector.h>
#include <utilsLib/InjectionContainer.h>

#include <memory>

namespace utilsLib
{

template <typename TPtr>
class DependencyInjectionBase
{
public:
  DependencyInjectionBase() = default;
  virtual ~DependencyInjectionBase()
  {
    m_container.clear();
  }

  void setupIOC()
  {
    m_injector = std::make_unique<utilsLib::Injector<TPtr>>(m_container);

    registerComponents(m_container);
    initialize(*m_injector);
  }

protected:
  virtual void initialize(Injector<TPtr>& injector) {}
  virtual void registerComponents(InjectionContainer<TPtr>& container) {}

  Injector<TPtr>& injector()
  {
    return *m_injector;
  }

private:
  InjectionContainer<TPtr>        m_container;
  std::unique_ptr<Injector<TPtr>> m_injector;

};

}