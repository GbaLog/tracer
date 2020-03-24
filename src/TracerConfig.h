#ifndef TracerConfigH
#define TracerConfigH
//-----------------------------------------------------------------------------
#include <string>
#include "TracerTypes.h"
//-----------------------------------------------------------------------------
class TracerConfig
{
public:
  static TracerConfig & instance();

  bool addWriter(TracerWriterPtr writer);
  void delWriter(TracerWriterPtr writer);
  void delWriterByName(const std::string & name);
  TracerWriterPtr getWriterByName(const std::string & name);
  void setLogLevel(TraceLevel level);
  void enableLogLevel(TraceLevel level);
  void disableLogLevel(TraceLevel level);
  bool isLogLevelEnabled(TraceLevel level);

  template<class T>
  std::enable_if_t<std::is_pointer_v<T>, std::shared_ptr<std::remove_pointer_t<T>>>
  getWriterByName(const std::string & name)
  {
    auto ptr = getWriterByName(name);
    return std::dynamic_pointer_cast<std::remove_pointer_t<T>>(ptr);
  }

  template<class T>
  std::enable_if_t<!std::is_pointer_v<T>, std::shared_ptr<T>>
  getWriterByName(const std::string & name)
  {
    auto ptr = getWriterByName(name);
    return std::dynamic_pointer_cast<T>(ptr);
  }
};
//-----------------------------------------------------------------------------
#endif // TracerConfigH
//-----------------------------------------------------------------------------
