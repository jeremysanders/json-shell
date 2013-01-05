#ifndef YAJL_HPP
#define YAJL_HPP

#include <string>

#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>

namespace Yajl
{

  class Error
  {
  public:
    virtual ~Error();
  };

  class ParseError : public Error
  {
  public:
    ParseError(yajl_status status, const std::string& errtxt)
      : _status(status),
	_errtxt(errtxt)
    {
    };

    yajl_status _status;
    std::string _errtxt;
  };

  class GeneratorError : public Error
  {
  public:
    GeneratorError(yajl_gen_status s)
    : _status(s)
    {
    };

    yajl_gen_status _status;
  };

  class Parser
  {
  public:
    Parser();
    virtual ~Parser();

    // parse the next chunk of text given
    void parse(const std::string& str);
    // complete parsing anything from parse when input stream has finished
    void complete_parse();

    // override these to parse the json
    virtual void null();
    virtual void boolean(bool b);
    virtual void number(const std::string& num);
    virtual void string(const std::string& str);
    virtual void map_open();
    virtual void map_key(const std::string& key);
    virtual void map_close();
    virtual void array_open();
    virtual void array_close();

  private:
    // throw a ParseError if the status is bad
    void _checkerror(yajl_status s);

  private:
    yajl_handle _parsehand;
    yajl_status _stat;
    std::string _last_input;
  };

  // Generate JSON output
  class Generator
  {
  public:
    Generator();
    ~Generator();

    // get current (incremental) output
    std::string get_incr_output();

    // generate these parts of the output stream
    void null();
    void boolean(bool b);
    void number(const std::string& num);
    void string(const std::string& str);
    void map_open();
    void map_close();
    void array_open();
    void array_close();

    // various options for the outputted json
    void set_beautify(bool on);
    void set_validate_utf8(bool on);
    void set_indent_string(const std::string& s);
    void set_escape_solidus(bool on);

  private:
    yajl_gen _gen;
  };

}

#endif
