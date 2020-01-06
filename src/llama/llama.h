#pragma once

#include "boost_asio.h"

#include "lightgrep/api.h"

struct Options;
struct ProgramHandle;
struct SFHASH_FileMatcher;

class Cli;
class InputReaderBase;

class Llama {
public:
  Llama();

  int run(int argc, const char *const argv[]);

  void search();

  bool init();

private:
  bool readMatchingSet(const std::string& matchSet);
  bool readpatterns(const std::vector<std::string> &keyFiles);
  bool openInput(const std::string &input);

  std::shared_ptr<Cli> CliParser;

  boost::asio::thread_pool Pool;

  std::shared_ptr<Options> Opts;

  std::shared_ptr<SFHASH_FileMatcher> Matcher;

  std::shared_ptr<ProgramHandle> LgProg;

  std::shared_ptr<InputReaderBase> Input;
};