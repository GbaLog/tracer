#ifndef FileWriterH
#define FileWriterH
//-----------------------------------------------------------------------------
#include "TracerInterfaces.h"
#include <fstream>
//-----------------------------------------------------------------------------
struct FileWriterParams
{
  std::string _filePattern;
  uint64_t _maxBytes;
  uint32_t _maxFiles;
};
//-----------------------------------------------------------------------------
class FileWriter : public ITracerWriter
{
public:
  FileWriter(const FileWriterParams & params) noexcept(false);
  ~FileWriter();

  void setFilePattern(const std::string & filePattern) noexcept(false);
  void setMaxBytes(uint64_t maxBytes) noexcept;
  void setMaxFiles(uint32_t maxFiles) noexcept;
  void setEnabled(bool enable) noexcept;

private:
  FileWriterParams _params;
  uint32_t _currentFileNum;
  std::string _currentFile;
  std::ofstream _file;
  uint64_t _currentByteCount;
  bool _enabled;

  virtual void write(const TraceRecord & record) override;
  virtual std::string getName() const noexcept override;

  void openCurrentFile() noexcept(false);
  void closeCurrentFile(bool end) noexcept;
  void writeImpl(const TraceRecord & record);
  void writeTime(time_t t);
  void writeObjParams(const TraceRecord & record);
  void writeMsg(const TraceRecord & record);
  void checkSize();
  void rotateFiles();
  void moveCurrentFile();
};
//-----------------------------------------------------------------------------
#endif // FileWriterH
//-----------------------------------------------------------------------------
