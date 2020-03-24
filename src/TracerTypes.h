#ifndef TracerTypesH
#define TracerTypesH
//-----------------------------------------------------------------------------
#include <string>
#include <ctime>
#include <unordered_set>
#include <memory>
//-----------------------------------------------------------------------------
class ITracerWriter;
typedef std::shared_ptr<ITracerWriter> TracerWriterPtr;
typedef std::unordered_set<TracerWriterPtr> SetTraceWriters;
//-----------------------------------------------------------------------------
enum TraceLevel : int
{
  NON = 0,
  VRB = 1,
  DBG = 2,
  WRN = 4,
  INF = 8,
  ERR = 16,
  ALL = VRB | DBG | WRN | INF | ERR
};
//-----------------------------------------------------------------------------
struct TraceRecord
{
  TraceLevel _lvl;
  time_t _time;
  std::string _objName;
  std::string _objId;
  std::string _str;
  SetTraceWriters _disabledWriters;
  bool _force;
};
//-----------------------------------------------------------------------------
#endif // TracerTypesH
//-----------------------------------------------------------------------------
