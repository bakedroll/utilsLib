#pragma once

#include <utilsLib/ITypeChecker.h>

namespace utilsLib
{

template <typename T, typename TBase>
class TypeChecker : public ITypeChecker<TBase>
{
  bool isType(const TBase* instance) const override
  {
    return (dynamic_cast<const T*>(instance) != nullptr);
  }
};

}
