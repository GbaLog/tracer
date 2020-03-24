#include "GlobalTracer.h"
#include "WriterStorage.h"
//-----------------------------------------------------------------------------
GlobalTracer & GlobalTracer::instance()
{
  static GlobalTracer instance;
  return instance;
}
//-----------------------------------------------------------------------------
void GlobalTracer::write(const TraceRecord & record)
{
  if (isLogLevelEnabled(record._lvl) == false && !record._force)
    return;

  WriterStorage::instance().write(record);
}
//-----------------------------------------------------------------------------
void GlobalTracer::setLogLevel(TraceLevel level)
{
  _level = static_cast<int>(level);
}
//-----------------------------------------------------------------------------
void GlobalTracer::enableLogLevel(TraceLevel level)
{
  _level |= static_cast<int>(level);
}
//-----------------------------------------------------------------------------
void GlobalTracer::disableLogLevel(TraceLevel level)
{
  _level &= ~(static_cast<int>(level));
}
//-----------------------------------------------------------------------------
bool GlobalTracer::isLogLevelEnabled(TraceLevel level)
{
  return _level & static_cast<int>(level);
}
//-----------------------------------------------------------------------------
GlobalTracer::GlobalTracer() :
  _level(static_cast<int>(TraceLevel::ALL))
{}
//-----------------------------------------------------------------------------
