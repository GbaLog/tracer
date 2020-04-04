# tracer
Simple tracer  

### You must remember that it isn't complete solution for logging.  
That fact that I provide Console and File writers doesn't mean that you can just take and use them.  
This project rather... give you a constructor and you should write that you need by yourself.  
Console and File writers are needed only for simple cases(non-high load/signlethread/etc).  
If you want more, you should write it yourself.  
Either with `Traceable` class. You can replace it with whatever you need.

# Instruction in case you want to try it
1. Add submodule to your project.
2. Add `add_subdirectory` to your CMake.
3. Link `libtracer` in your CMake.
4. Use.  
Usage is simple, you just need to `#include "Tracer.h"` in your header and inherit from `Traceable` class.
After that in constructor of your class you have to call constructor of `Traceable` with parameters:  
- `Traceable(const std::string & block, int id)`  
Here `block` is name of your class in short form and `id` is identifier of current instance of your class.
- `Traceable(const std::string & block, const std::string & id)`  
The same as previous, but `id` is `std::string`.  
After that you're done.  
To use tracer you can just call one of three macroses:  
1. `TRACE(TraceLevel)` - This form must be used inside of class inherited from `Traceable`.  
After this construction you can use it like `std::cout` and so on.  
Example: `TRACE(DBG) << "Hello, " << 42 << " world!";`  
2. `TRACE_SINGLE(TraceLevel, BlockName)` - This form should be used when you don't have class or,  
if you want to just trace single line without creating any objects or from `static` function.  
Usage is the same as for `TRACE`.
3. `TRACE_FORCE(TraceLevel, BlockName)` - This form should be used when you want this line to be output even if logging is off.  
Usage is the same as for `TRACE`.  
##### TraceLevels  
Nothing interesting here, just 5 levels:
- `VRB` - Verbose  
- `DBG` - Debug  
- `WRN` - Warning  
- `INF` - Info  
- `ERR` - Error  
##### Also you have to create and register `ConsoleWriter` or/and `FileWriter` if you need them.  
Out-of-box tracer provides only Console and File writer.
- ConsoleWriter provides output on console, you can only enable/disable it.  
- FileWriter has a feature to control file size and number of maximum files.  
  
Also you can create your own handlers and you here is no limits for your fantasy.  
You just need implement and register them through `TracerConfig`.  

#### To register out-of-box handlers  
```cpp
#include "TracerConfig.h"
#include "ConsoleWriter.h"
#include "FileWriter.h"
#include "Tracer.h"

bool initTracer()
{
  FileWriterParams fileWrParams;
  fileWrParams._filePattern = "/var/log/app.log";
  fileWrParams._maxBytes = 1024 * 1024 * 10; //10 MByte
  fileWrParams._maxFiles = 10;

  try
  {
    auto con = std::make_shared<ConsoleWriter>();
    TracerConfig::instance().addWriter(con);

    //File writer can throw exception if can't create file
    auto file = std::make_shared<FileWriter>(fileWrParams);
    TracerConfig::instance().addWriter(file);
  }
  catch (const std::runtime_error & ex)
  {
    TRACE_FORCE(ERR, "Init") << ex.what();
    return false;
  }
  return true;
}
```  
**Note: You have to remember about `try-catch` while use `FileWriter`, because it will throw exception if it can't open log file.**

##### Enable/disable particular logging  
If you want to disable some log level, you can do it globally through `TracerConfig`.  
Also I want to provide a possibility to disable/enable some log levels for `ConsoleWriter` and `FileWriters` itself.  
If you write your own writers, you can do it.  
  
From `TracerConfig` you can get particular writer through `getWriterByName` function.  
Every Writer must have unique name!  
`ConsoleWriter` has `"console"` name.  
`FileWriter` has `"file"` name.  
If you added, for example, `ConsoleWriter` to tracer, you can get it like this:  
```cpp
auto con = TracerConfig::instance().getWriterByName<ConsoleWriter>("console");
```  
This needed if you, for example, want to disable it at all without deleting it.  
(Both Console and File writers have setEnabled methods).  

##### Multithreading  
As I mentioned above, this is not complete solution for logging.  
`TraceConfig` is singleton and it's not support multithreading, as well as `ConsoleWriter` with `FileWriter`.  
If you need to add some kind of it, you just have to write your own multithreaded writers.  
  
#### Example of usage  
```cpp
#include "Tracer.h"
//...

class Foo : private Traceable
{
  Foo(int id) : Traceable("Foo", id)
  {
    TRACE(DBG) << "Constructed";
  }
  
  ~Foo()
  {
    TRACE(DBG) << "Destructed";
  }
};
//...
int main(int argc, char argv *[])
{
  //Adding needed writers to the TracerConfig
  //...
  {
    Foo f{42};
  }
}
```
In case if you added `ConsoleWriter` to the `TracerConfig` it will output:
```
200325-091225 [DBG] | Foo          | 42           | Constructed
200325-091225 [DBG] | Foo          | 42           | Destructed
```
