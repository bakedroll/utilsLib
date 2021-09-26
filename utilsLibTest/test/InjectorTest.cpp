#include <gtest/gtest.h>

#include <utilsLib/InjectionContainer.h>
#include <utilsLib/Injector.h>
#include <utilsLib/PointerTypeDefinition.h>
#include <utilsLib/ILoggingManager.h>
#include <utilsLib/LoggingManager.h>

TEST(InjectorTest, DependencyTree)
{
  class A
  {
  public:
    A(utilsLib::StdInjector& injector)
    {
    }

    virtual ~A() = default;
  };

  class B
  {
  public:
    B(utilsLib::StdInjector& injector)
    {
    }

    virtual ~B() = default;
  };

  class IC
  {
  public:
    virtual ~IC() = default;

    virtual void pureVirtualFunc() = 0;
  };

  class ID
  {
  public:
    virtual ~ID() = default;

    virtual void pureVirtualFunc() = 0;
  };

  class IE
  {
  public:
    virtual ~IE() = default;

    virtual void pureVirtualFunc() = 0;
  };

  class IF
  {
  public:
    virtual ~IF() = default;

    virtual void pureVirtualFunc() = 0;
  };

  class C : public IC
  {
  public:
    C(utilsLib::StdInjector& injector)
    {
    }

    void pureVirtualFunc() override {}
  };

  class D1 : public ID
  {
  public:
    D1(utilsLib::StdInjector& injector)
    {
    }

    void pureVirtualFunc() override {}
  };

  class D2 : public ID
  {
  public:
    D2(utilsLib::StdInjector& injector)
    {
    }

    void pureVirtualFunc() override {}
  };

  class E : public IE
  {
  public:
    E(utilsLib::StdInjector& injector)
    {
    }

    void pureVirtualFunc() override {}
  };

  class F1 : public IF
  {
  public:
    F1(utilsLib::StdInjector& injector)
    {
    }

    void pureVirtualFunc() override {}
  };

  class F2 : public IF
  {
  public:
    F2(utilsLib::StdInjector& injector)
    {
    }

    void pureVirtualFunc() override {}
  };

  utilsLib::ILoggingManager::create<utilsLib::LoggingManager>();

  utilsLib::StdInjectionContainer container;

  container.registerSingletonType<A>();
  container.registerType<B>();

  container.registerSingletonInterfaceType<IC, C>();
  container.registerSingletonInterfaceType<ID, D1>();
  container.registerSingletonInterfaceType<ID, D2>();

  container.registerInterfaceType<IE, E>();
  container.registerInterfaceType<IF, F1>();
  container.registerInterfaceType<IF, F2>();

  utilsLib::StdInjector injector(container);
  
  // Singleton types
  const auto a1 = injector.inject<A>();
  const auto a2 = injector.inject<A>();

  EXPECT_EQ(a1, a2);

  // Instances
  const auto b1 = injector.inject<B>();
  const auto b2 = injector.inject<B>();

  EXPECT_NE(b1, b2);

  // Singleton interface types
  const auto c1 = injector.inject<IC>();
  const auto c2 = injector.inject<IC>();

  EXPECT_EQ(c1, c2);

  const auto dlist1 = injector.injectAll<ID>();
  const auto dlist2 = injector.injectAll<ID>();

  EXPECT_EQ(dlist1.size(), 2);
  EXPECT_EQ(dlist2.size(), 2);

  for (auto i=0; i<dlist1.size(); i++)
  {
    EXPECT_EQ(dlist1[i], dlist2[i]);
  }

  // Interface instances
  const auto e1 = injector.inject<IE>();
  const auto e2 = injector.inject<IE>();

  EXPECT_NE(e1, e2);

  const auto flist1 = injector.injectAll<IF>();
  const auto flist2 = injector.injectAll<IF>();

  EXPECT_EQ(flist1.size(), 2);
  EXPECT_EQ(flist2.size(), 2);

  for (auto i=0; i<flist1.size(); i++)
  {
    EXPECT_NE(flist1[i], flist2[i]);
  }

  container.clear();
}
