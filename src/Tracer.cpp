#include "Tracer.h"
#include "GlobalTracer.h"
#include <ctime>
#include <iomanip>
#include <charconv>
//-----------------------------------------------------------------------------
TraceObject::TraceObject(const std::string & name, const std::string & id) :
  _name(name),
  _id(id)
{}
//-----------------------------------------------------------------------------
TraceObject::TraceObject(const std::string & name, int id) :
  _name(name),
  _id(std::to_string(id))
{}
//-----------------------------------------------------------------------------
TempTracer TraceObject::makeTrace(TraceLevel level, bool force)
{
  return TempTracer{level, force, _name, _id, {}};
}
//-----------------------------------------------------------------------------
void TraceObject::setId(const std::string & id)
{
  _id = id;
}
//-----------------------------------------------------------------------------
void TraceObject::setId(int id)
{
  char buf[20] = {};
  auto[p, ec] = std::to_chars(std::begin(buf), std::end(buf), id);
  if (ec == std::errc())
  {
    _id = std::string(std::begin(buf), p - std::begin(buf));
  }
}
//-----------------------------------------------------------------------------
Traceable::Traceable(const std::string & name, const std::string & id) :
  _traceObj(name, id)
{}
//-----------------------------------------------------------------------------
Traceable::Traceable(const std::string & name, int id) :
  _traceObj(name, id)
{}
//-----------------------------------------------------------------------------
TempTracer Traceable::makeTrace(TraceLevel level, bool force)
{
  return _traceObj.makeTrace(level, force);
}
//-----------------------------------------------------------------------------
void Traceable::setId(const std::string & id)
{
  _traceObj.setId(id);
}
//-----------------------------------------------------------------------------
void Traceable::setId(int id)
{
  _traceObj.setId(id);
}
//-----------------------------------------------------------------------------
TempTracer::TempTracer(TraceLevel level, bool force,
                         const std::string & name, const std::string & id,
                         const SetTraceWriters & disabledWriters)
{
  _rec._lvl = level;
  _rec._time = time(nullptr);
  _rec._force = force;
  _rec._objName = name;
  _rec._objId = id;
  _rec._disabledWriters = disabledWriters;
}

TempTracer::~TempTracer()
{
  std::string str = _strm.str();
  _rec._str = std::move(str);
  GlobalTracer::instance().write(_rec);
}
