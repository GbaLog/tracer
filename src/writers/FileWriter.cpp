#include "FileWriter.h"
#include "GlobalTracer.h"
#include "TracerConfig.h"
#include "TracerCommon.h"
#include <iostream>
#include <cstddef>
#include <cstring>
#include <filesystem>
//-----------------------------------------------------------------------------
namespace fs = std::filesystem;
//-----------------------------------------------------------------------------
FileWriter::FileWriter(const FileWriterParams & params) noexcept(false) :
  _params(params),
  _currentFileNum(0),
  _currentByteCount(0),
  _enabled(true)
{
  if (_params._maxBytes == 0 || _params._maxFiles == 0)
    return;

  _currentFile = _params._filePattern;
  if (fs::exists(_currentFile))
  {
    _currentFileNum = _params._maxFiles - 1;
    rotateFiles();
    moveCurrentFile();
    _currentFileNum = 0;
  }

  openCurrentFile();
}
//-----------------------------------------------------------------------------
FileWriter::~FileWriter()
{
  closeCurrentFile(true);
}
//-----------------------------------------------------------------------------
void FileWriter::setFilePattern(const std::string & filePattern) noexcept(false)
{
  closeCurrentFile(true);
  _params._filePattern = filePattern;
  _currentFile = _params._filePattern;
  openCurrentFile();
}
//-----------------------------------------------------------------------------
void FileWriter::setMaxBytes(uint64_t maxBytes) noexcept
{
  _params._maxBytes = maxBytes;
}
//-----------------------------------------------------------------------------
void FileWriter::setMaxFiles(uint32_t maxFiles) noexcept
{
  _params._maxFiles = maxFiles;
}
//-----------------------------------------------------------------------------
void FileWriter::setEnabled(bool enable) noexcept
{
  _enabled = enable;
}
//-----------------------------------------------------------------------------
void FileWriter::write(const TraceRecord & record)
{
  if (_enabled == false)
  {
    if (!_file)
      return;
    if (record._force == false)
      return;
  }

  writeImpl(record);
}
//-----------------------------------------------------------------------------
std::string FileWriter::getName() const noexcept
{
  return "file";
}
//-----------------------------------------------------------------------------
void FileWriter::openCurrentFile() noexcept(false)
{
  _file.open(_currentFile, std::ios::trunc);
  if (!_file)
  {
    _enabled = false;

    auto con = TracerConfig::instance().getWriterByName("console");
    TraceRecord rec;
    rec._time = time(nullptr);
    rec._force = true;
    rec._lvl = TraceLevel::ERR;
    rec._objName = "FileWriter";
    rec._objId = _params._filePattern;
    rec._str = "Cannot create or open log file: " + _currentFile;
    con->write(rec);

    throw std::runtime_error(rec._str);
  }

  if (_currentFileNum == 0)
  {
    _file << "*** Begin of the log file ***\n"
          << "Build date: " << __DATE__ << ", build time: " << __TIME__ << "\n";
  }
  else
  {
    _file << "*** This is continue of previous file ***\n";
  }
}
//-----------------------------------------------------------------------------
void FileWriter::closeCurrentFile(bool end) noexcept
{
  if (_file)
  {
    if (end)
      _file << "*** End of log file ***" << std::endl;
    else
      _file << "*** Log will continue in next file ***\n";
    _file.close();
  }
}
//-----------------------------------------------------------------------------
void FileWriter::writeImpl(const TraceRecord & record)
{
  //200322-171545 [DBG] | class A      | 1             | text\n
  writeTime(record._time);
  writeObjParams(record);
  writeMsg(record);
  checkSize();
}
//-----------------------------------------------------------------------------
void FileWriter::writeTime(time_t t)
{
  char buf[32] = {};
  tm * ptime = nullptr;
  ptime = localtime(&t);
  strftime(buf, sizeof(buf), "%y%m%d-%H%M%S", ptime);
  _file << buf;
  _currentByteCount += strlen(buf);
}
//-----------------------------------------------------------------------------
void FileWriter::writeObjParams(const TraceRecord & record)
{
  _file << " [" << convertLvlToString(record._lvl) << "] "
        << std::left
        << "| " << std::setw(12) << record._objName << " | "
        << std::setw(12) << record._objId << " | "
        << std::right;
  // [DBG] | class A      | 1             |
  // `---'  `------------' `-------------'
  //  5          14              15
  // + 5 controls
  //= 5 + 14 + 15 + 5 = 39
  _currentByteCount += 39 + record._objId.size() + record._objName.size();
}
//-----------------------------------------------------------------------------
void FileWriter::writeMsg(const TraceRecord & record)
{
  if (record._str.empty() == false)
  {
    _file << record._str;
    _currentByteCount += record._str.size();
  }
  _file << "\n";
  _currentByteCount += 1;
}
//-----------------------------------------------------------------------------
/**
 * @brief This function checks size of current file and rotate files if needed
 * @algorithm
 * If file size exceed, then:
 * 1. Close current file and set current byte counter to zero
 * 2. If %current file number% >= %max file count% then:
 * 2.1. Rotate files
 * 2.2. Remove last file because it goes out of range [1, %max file count%]
 * 2.3. Move %current file% to %current file%.1
 * 2.4. Open new file and return
 * 3. Else:
 * 3.1. Rotate files
 * 3.2. Move %current file% to %current file%.1
 * 3.3. Increment %current file number%
 * 3.4. Open new file
 */
void FileWriter::checkSize()
{
  if (_currentByteCount < _params._maxBytes)
    return;

  closeCurrentFile(false);
  _currentByteCount = 0;

  if (_currentFileNum >= _params._maxFiles)
  {
    rotateFiles();
    //Removes last file after rotating
    fs::remove(fs::path(_currentFile + "." + std::to_string(_currentFileNum + 1)));

    moveCurrentFile();
    openCurrentFile();
    return;
  }

  rotateFiles();
  moveCurrentFile();

  ++_currentFileNum;
  openCurrentFile();
}
//-----------------------------------------------------------------------------
/**
 * @brief This function rotates file by file pattern and suffix
 * For example:
 * @example
 * From: a.log.1, a.log.2
 * To: a.log.2, a.log.3
 * @note This function doesn't consider any bounds like @a maxfiles
 */
void FileWriter::rotateFiles()
{
  for (uint32_t it = _currentFileNum; it >= 1; --it)
  {
    fs::path from = _params._filePattern + "." + std::to_string(it);
    fs::path to = _params._filePattern + "." + std::to_string(it + 1);
    if (fs::exists(from))
    {
      fs::remove(to);
      fs::rename(from, to);
    }
  }
}
//-----------------------------------------------------------------------------
///< @brief This function renames current file to current file + suffix ".1"
void FileWriter::moveCurrentFile()
{
  fs::path from = _currentFile;
  fs::path to = _currentFile + ".1";
  if (fs::exists(from))
  {
    fs::remove(to);
    fs::rename(from, to);
  }
}
//-----------------------------------------------------------------------------
