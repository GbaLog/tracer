#include "TracerCommon.h"
//-----------------------------------------------------------------------------
std::string convertLvlToString(TraceLevel level)
{
  switch (level)
  {
  case TraceLevel::VRB: return "VRB";
  case TraceLevel::DBG: return "DBG";
  case TraceLevel::INF: return "INF";
  case TraceLevel::WRN: return "WRN";
  case TraceLevel::ERR: return "ERR";
  default: return "UNK";
  }
}
//-----------------------------------------------------------------------------
