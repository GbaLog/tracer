#ifndef TracerInterfacesH
#define TracerInterfacesH
//-----------------------------------------------------------------------------
#include <string>
#include "TracerTypes.h"
//-----------------------------------------------------------------------------
class ITracerWriter
{
public:
  virtual ~ITracerWriter() = default;

  virtual void write(const TraceRecord & rec) = 0;
  virtual std::string getName() const noexcept = 0;
};
//-----------------------------------------------------------------------------
#endif // TracerInterfacesH
//-----------------------------------------------------------------------------
