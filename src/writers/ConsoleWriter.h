#ifndef ConsoleWriterH
#define ConsoleWriterH
//-----------------------------------------------------------------------------
#include "TracerInterfaces.h"
#include "TracerTypes.h"
//-----------------------------------------------------------------------------
class ConsoleWriter : public ITracerWriter
{
public:
  explicit ConsoleWriter(bool enable = true);
  ~ConsoleWriter();

  void setEnabled(bool enable) noexcept;
  bool getEnabled() const noexcept;

private:
  bool _enable;

  //ITracerWriter
  virtual void write(const TraceRecord & record) override;
  virtual std::string getName() const noexcept override;

  void writeImpl(const TraceRecord & record);
  void writeTime(time_t t);
  void writeObjParams(const TraceRecord & record);
  void writeMsg(const TraceRecord & record);
  void changeColor(TraceLevel lvl);
};
//-----------------------------------------------------------------------------
#endif // ConsoleWriterH
//-----------------------------------------------------------------------------
