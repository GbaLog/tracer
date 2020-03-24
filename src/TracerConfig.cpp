#include "TracerConfig.h"
#include "WriterStorage.h"
#include "GlobalTracer.h"
//-----------------------------------------------------------------------------
TracerConfig & TracerConfig::instance()
{
  static TracerConfig instance;
  return instance;
}
//-----------------------------------------------------------------------------
bool TracerConfig::addWriter(TracerWriterPtr writer)
{
  return WriterStorage::instance().addWriter(writer);
}
//-----------------------------------------------------------------------------
void TracerConfig::delWriter(TracerWriterPtr writer)
{
  WriterStorage::instance().delWriter(writer);
}
//-----------------------------------------------------------------------------
TracerWriterPtr TracerConfig::getWriterByName(const std::string & name)
{
  return WriterStorage::instance().getWriterByName(name);
}
//-----------------------------------------------------------------------------
void TracerConfig::setLogLevel(TraceLevel level)
{
  GlobalTracer::instance().setLogLevel(level);
}
//-----------------------------------------------------------------------------
void TracerConfig::enableLogLevel(TraceLevel level)
{
  GlobalTracer::instance().enableLogLevel(level);
}
//-----------------------------------------------------------------------------
void TracerConfig::disableLogLevel(TraceLevel level)
{
  GlobalTracer::instance().disableLogLevel(level);
}
//-----------------------------------------------------------------------------
bool TracerConfig::isLogLevelEnabled(TraceLevel level)
{
  return GlobalTracer::instance().isLogLevelEnabled(level);
}
//-----------------------------------------------------------------------------
