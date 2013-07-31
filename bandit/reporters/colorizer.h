#ifndef BANDIT_REPORTERS_COLORIZER_H
#define BANDIT_REPORTERS_COLORIZER_H

namespace bandit { namespace detail {

#ifdef _WIN32
  #include <windows.h>
  struct colorizer
  {
    colorizer(bool colors_enabled = true)
      : colors_enabled_(colors_enabled),
		stdout_handle_(GetStdHandle(STD_OUTPUT_HANDLE))
    {
		original_color_ = get_console_color();
	}

    const char* green() const
    {
      if(colors_enabled_)
	  {
		  set_console_color(FOREGROUND_GREEN);
	  }
	  return "";
    }

    const char* red() const
    {
      if(colors_enabled_)
	  {
		  set_console_color(FOREGROUND_RED);
	  }
	  return "";
    }

    const char* reset() const
    {
      if(colors_enabled_)
	  {
		  set_console_color(original_color_);
	  }
	  return "";
    }

  private:
	  int get_console_color() const
	  {
		  CONSOLE_SCREEN_BUFFER_INFO info = {0};
		  GetConsoleScreenBufferInfo(stdout_handle_, &info);
		  return info.wAttributes;
	  }

	  void set_console_color(int color) const
	  {
		  SetConsoleTextAttribute(stdout_handle_, color);
	  }

    private:
    bool colors_enabled_;
	HANDLE stdout_handle_;
	int original_color_;
  };

#else
  struct colorizer
  {
    colorizer(bool colors_enabled = true)
      : colors_enabled_(colors_enabled)
    {}

    const char* green() const
    {
      return colors_enabled_ ? "\033[1;32m" : "";
    }

    const char* red() const
    {
      return colors_enabled_ ? "\033[1;31m" : "";
    }

    const char* reset() const
    {
      return colors_enabled_ ? "\033[0m" : "";
    }

    private:
    bool colors_enabled_;
  };
#endif
}}

#endif
