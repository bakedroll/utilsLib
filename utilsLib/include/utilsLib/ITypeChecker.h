#pragma once

namespace utilsLib
{

template <typename TBase>
class ITypeChecker
{
public:
  virtual ~ITypeChecker() = default;
  virtual bool isType(const TBase* instance) const = 0;

};

}
