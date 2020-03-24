#ifndef GlobalTracerH
#define GlobalTracerH
//-----------------------------------------------------------------------------
#include "TracerTypes.h"
//-----------------------------------------------------------------------------
class GlobalTracer
{
public:
  static GlobalTracer & instance();

  void write(const TraceRecord & record);
  void setLogLevel(TraceLevel level);
  void enableLogLevel(TraceLevel level);
  void disableLogLevel(TraceLevel level);
  bool isLogLevelEnabled(TraceLevel level);

private:
  GlobalTracer();

  int _level;
};
//-----------------------------------------------------------------------------
#endif // GlobalTracerH
//-----------------------------------------------------------------------------
