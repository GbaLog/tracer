#ifndef WriterStorageH
#define WriterStorageH
//-----------------------------------------------------------------------------
#include "TracerTypes.h"
//-----------------------------------------------------------------------------
class WriterStorage
{
public:
  static WriterStorage & instance();

  bool addWriter(TracerWriterPtr writer);
  void delWriter(TracerWriterPtr writer);
  void delWriterByName(const std::string & name);
  TracerWriterPtr getWriterByName(const std::string & name);

  void write(const TraceRecord & record);

private:
  WriterStorage();

  SetTraceWriters _writers;
};
//-----------------------------------------------------------------------------
#endif // WriterStorageH
//-----------------------------------------------------------------------------
