#include "ConsoleWriter.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include "TracerCommon.h"
//-----------------------------------------------------------------------------
ConsoleWriter::ConsoleWriter(bool enable) :
  _enable(enable)
{}
//-----------------------------------------------------------------------------
void ConsoleWriter::write(const TraceRecord & record)
{
  if (_enable == false && record._force == false)
    return;

  writeImpl(record);
}
//-----------------------------------------------------------------------------
std::string ConsoleWriter::getName() const noexcept
{
  return "console";
}
//-----------------------------------------------------------------------------
void ConsoleWriter::setEnabled(bool enable) noexcept
{
  _enable = enable;
}
//-----------------------------------------------------------------------------
bool ConsoleWriter::getEnabled() const noexcept
{
  return _enable;
}
//-----------------------------------------------------------------------------
void ConsoleWriter::writeImpl(const TraceRecord & record)
{
  //200322-171545 [DBG] | class A      |               | text\n
  writeTime(record._time);
  writeObjParams(record);
  writeMsg(record);
}
//-----------------------------------------------------------------------------
void ConsoleWriter::writeTime(time_t t)
{
  char buf[32] = {};
  tm * ptime = nullptr;
  ptime = localtime(&t);
  strftime(buf, sizeof(buf), "%y%m%d-%H%M%S", ptime);
  std::cout << buf;
}
//-----------------------------------------------------------------------------
void ConsoleWriter::writeObjParams(const TraceRecord & record)
{
  std::cout << " [" << convertLvlToString(record._lvl) << "] "
            << std::left
            << "| " << std::setw(12) << record._objName << " | "
            << std::setw(12) << record._objId << " | "
            << std::right;
}
//-----------------------------------------------------------------------------
void ConsoleWriter::writeMsg(const TraceRecord & record)
{
  if (record._str.empty() == false)
    std::cout << record._str;
  std::cout << "\n";
}
//-----------------------------------------------------------------------------
