#ifndef RatelTracerH
#define RatelTracerH
//-----------------------------------------------------------------------------
#include <string>
#include <sstream>
#include "TracerTypes.h"
//-----------------------------------------------------------------------------
class TempTracer
{
public:
  TempTracer(TraceLevel level, bool force, const std::string & name, const std::string & id,
              const SetTraceWriters & disabledWriters);
  ~TempTracer();

  template<class T>
  TempTracer & operator <<(const T & val)
  {
    _strm << val;
    return *this;
  }

private:
  TraceRecord _rec;
  std::ostringstream _strm;
};
//-----------------------------------------------------------------------------
class TraceObject
{
public:
  TraceObject(const std::string & name, const std::string & id);
  TraceObject(const std::string & name, int id);

  TempTracer makeTrace(TraceLevel level, bool force);
  void setId(const std::string & id);
  void setId(int id);

private:
  std::string _name;
  std::string _id;
};
//-----------------------------------------------------------------------------
class Traceable
{
public:
  Traceable(const std::string & name, const std::string & id = "");
  Traceable(const std::string & name, int id);

  TempTracer makeTrace(TraceLevel level, bool force);
  void setId(const std::string & id);
  void setId(int id);

protected:
  mutable TraceObject _traceObj;
};
//-----------------------------------------------------------------------------
#define TRACER_OBJ() _traceObj
#define TRACE(lvl) (this->TRACER_OBJ()).makeTrace(lvl, false)
#define TRACE_SINGLE(lvl, name) TraceObject(name, "").makeTrace(lvl, false)
#define TRACE_FORCE(lvl, name) TraceObject(name, "").makeTrace(lvl, true)
//-----------------------------------------------------------------------------
#endif // RatelTracerH
//-----------------------------------------------------------------------------
