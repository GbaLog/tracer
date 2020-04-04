#include "ConsoleWriter.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include "TracerCommon.h"

#ifdef _WIN32
#include <windows.h>
#endif
//-----------------------------------------------------------------------------
ConsoleWriter::ConsoleWriter(bool enable) :
  _enable(enable)
{}
//-----------------------------------------------------------------------------
ConsoleWriter::~ConsoleWriter()
{
  changeColor(TraceLevel::DBG);
}
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
  changeColor(record._lvl);
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
void ConsoleWriter::changeColor(TraceLevel lvl)
{
#ifdef _WIN32
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO cInfo;
  if (GetConsoleScreenBufferInfo(hStdOut, &cInfo) == FALSE)
    return;
  WORD attrs = cInfo.wAttributes;
  attrs &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
  switch (lvl)
  {
  case VRB:
  case DBG:
    attrs |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case WRN:
    attrs |= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
    break;
  case INF:
    attrs |= FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case ERR:
    attrs |= FOREGROUND_INTENSITY | FOREGROUND_RED;
    break;
  default:
    break;
  }

  SetConsoleTextAttribute(hStdOut, attrs);
#else
  switch (lvl)
  {
  case VRB:
  case DBG:
    printf("\033[0m");
    break;
  case WRN:
    printf("\033[33;1m");
    break;
  case INF:
    printf("\033[36;1m");
    break;
  case ERR:
    printf("\033[31;1m");
    break;
  default:
    break;
  }
#endif
}
//-----------------------------------------------------------------------------
