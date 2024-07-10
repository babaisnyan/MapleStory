#pragma once

enum class Color {
  kBlack,
  kWhite,
  kRed,
  kGreen,
  kBlue,
  kYellow,
};

class ConsoleLog {
  enum { kBufferSize = 4096 };

public:
  static ConsoleLog& GetInstance() {
    static ConsoleLog instance;
    return instance;
  }

public:
  ConsoleLog();
  ~ConsoleLog() = default;

public:
  void WriteStdOut(Color color, const wchar_t* format, ...);
  void WriteStdErr(Color color, const wchar_t* format, ...);

protected:
  void SetColor(bool std_out, Color color);

private:
  HANDLE _std_out;
  HANDLE _std_err;
};
