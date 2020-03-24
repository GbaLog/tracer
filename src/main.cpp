#include "Tracer.h"
#include "ConsoleWriter.h"
#include "TracerConfig.h"
#include "FileWriter.h"

class A : public Traceable
{
public:
  A() : Traceable("ololo", 1)
  {
    TRACE(VRB) << "ololo '" << 42 << "' dawd";
    TRACE(DBG) << "ololo '" << 42 << "' dawd";
    TRACE(WRN) << "ololo '" << 42 << "' dawd";
    TRACE(INF) << "ololo '" << 42 << "' dawd";
    TRACE(ERR) << "ololo '" << 42 << "' dawd";
    TRACE_FORCE(DBG, "force") << "forced";
    TRACE_SINGLE(DBG, "ololo") << "-----------------------";
  }
};

int main()
{
  FileWriterParams fileWrParams;
  fileWrParams._filePattern = "ololo.log";
  fileWrParams._maxBytes = 1024 * 1024;
  fileWrParams._maxFiles = 2;

  auto consoleWriter = std::make_shared<ConsoleWriter>();
  auto fileWriter = std::make_shared<FileWriter>(fileWrParams);
  TracerConfig::instance().addWriter(consoleWriter);
  TracerConfig::instance().addWriter(fileWriter);
  {
    A a;
  }
  TracerConfig::instance().disableLogLevel(TraceLevel::VRB);
  auto wr = TracerConfig::instance().getWriterByName<ConsoleWriter>("console");
  if (wr)
  {
    wr->setEnabled(false);
  }
  auto fwr = TracerConfig::instance().getWriterByName<FileWriter>("file");
  if (fwr)
  {
    TRACE_FORCE(DBG, "single") << "File casted to console";
    fwr->setEnabled(false);
  }
  {
    A a;
    A b;
  }
}
