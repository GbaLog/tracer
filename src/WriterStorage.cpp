#include "WriterStorage.h"
#include "TracerInterfaces.h"
//-----------------------------------------------------------------------------
WriterStorage & WriterStorage::instance()
{
  static WriterStorage instance;
  return instance;
}
//-----------------------------------------------------------------------------
bool WriterStorage::addWriter(TracerWriterPtr writer)
{
  if (writer == nullptr)
    return false;

  auto it = _writers.find(writer);
  if (it != _writers.end())
    return false;
  _writers.insert(writer);
  return true;
}
//-----------------------------------------------------------------------------
void WriterStorage::delWriter(TracerWriterPtr writer)
{
  if (writer == nullptr)
    return;
  _writers.erase(writer);
}
//-----------------------------------------------------------------------------
void WriterStorage::delWriterByName(const std::string & name)
{
  auto it = _writers.begin();
  for (; it != _writers.end(); ++it)
  {
    if ((*it)->getName() == name)
      break;
  }

  if (it == _writers.end())
    return;
  _writers.erase(it);
}
//-----------------------------------------------------------------------------
TracerWriterPtr WriterStorage::getWriterByName(const std::string & name)
{
  auto it = _writers.begin();
  for (; it != _writers.end(); ++it)
  {
    if ((*it)->getName() == name)
      break;
  }

  if (it == _writers.end())
    return nullptr;
  return *it;
}
//-----------------------------------------------------------------------------
void WriterStorage::write(const TraceRecord & record)
{
  for (const auto & it : _writers)
  {
    auto wrIt = record._disabledWriters.find(it);
    if (wrIt != record._disabledWriters.end())
      continue;
    it->write(record);
  }
}
//-----------------------------------------------------------------------------
WriterStorage::WriterStorage()
{}
//-----------------------------------------------------------------------------
