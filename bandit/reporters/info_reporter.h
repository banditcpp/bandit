#ifndef BANDIT_INFO_REPORTER_H
#define BANDIT_INFO_REPORTER_H

namespace bandit {
namespace detail {

struct info_reporter : public progress_reporter
{
	info_reporter(std::ostream &stm, const failure_formatter &failure_formatter,
	  const detail::colorizer &colorizer)
	  : progress_reporter(failure_formatter)
	  , stm_(stm)
	  , colorizer_(colorizer)
	  , indentation_(0)
	{}

	info_reporter(const failure_formatter &failure_formatter, const detail::colorizer &colorizer)
	  : progress_reporter(failure_formatter)
	  , stm_(std::cout)
	  , colorizer_(colorizer)
	  , indentation_(0)
	{}

	info_reporter &operator=(const info_reporter &)
	{
		return *this;
	}

	void summary()
	{
		stm_
		  << colorizer_.white()
		  << "Tests run: " << specs_run_
		  << std::endl;
		if (specs_skipped_ > 0) {
			stm_
			  << colorizer_.yellow()
			  << "Skipped: " << specs_skipped_
			  << std::endl;
		}
		if (specs_succeeded_ > 0) {
			stm_
			  << colorizer_.green()
			  << "Passed: " << specs_succeeded_
			  << std::endl;
		}
		if (specs_failed_ > 0) {
			stm_
			  << colorizer_.red()
			  << "Failed: " << specs_failed_
			  << std::endl;
			std::for_each(failures_.begin(), failures_.end(), [&](const std::string &failure) {
				stm_
				 << colorizer_.white()
				 << " (*) "
				 << colorizer_.red()
				 << failure << std::endl;
			});
		}
		if (test_run_errors_.size() > 0) {
			stm_
			  << colorizer_.red()
			  << "Errors: " << test_run_errors_.size()
			  << std::endl;
			std::for_each(test_run_errors_.begin(), test_run_errors_.end(), [&](const std::string &error) {
				stm_
				 << colorizer_.white()
				 << " (*) "
				 << colorizer_.red()
				 << error << std::endl;
			});
		}
		stm_
		  << colorizer_.reset()
		  << std::endl;
	}

	void test_run_complete()
	{
		progress_reporter::test_run_complete();
		stm_ << std::endl;
		summary();
		stm_.flush();
	}

	void test_run_error(const char *desc, const struct test_run_error &err)
	{
		progress_reporter::test_run_error(desc, err);

		std::stringstream ss;
		ss << std::endl;
		ss << "Failed to run \"" << current_context_name() << "\": error \"" << err.what() << "\"" << std::endl;

		test_run_errors_.push_back(ss.str());
	}

	virtual void context_starting(const char *desc)
	{
		progress_reporter::context_starting(desc);

		stm_
		  << indent()
		  << colorizer_.blue()
		  << "begin "
		  << colorizer_.white()
		  << desc
		  << colorizer_.reset()
		  << std::endl;
		++indentation_;
		stm_.flush();

	}

	virtual void context_ended(const char *desc)
	{
		progress_reporter::context_ended(desc);
		--indentation_;
		stm_
		  << indent()
		  << colorizer_.blue()
		  << "end "
		  << colorizer_.reset()
		  << desc << std::endl;
	}

	virtual void it_starting(const char *desc)
	{
		progress_reporter::it_starting(desc);
		stm_
		  << indent()
		  << colorizer_.yellow()
		  << "[ TEST ]"
		  << colorizer_.reset()
		  << " it " << desc;
		++indentation_;
		stm_.flush();
	}

	virtual void it_succeeded(const char *desc)
	{
		progress_reporter::it_succeeded(desc);
		--indentation_;
		stm_
		  << "\r" << indent()
		  << colorizer_.green()
		  << "[ PASS ]"
		  << colorizer_.reset()
		  << " it " << desc
		  << std::endl;
		stm_.flush();
	}

	virtual void it_failed(const char *desc, const assertion_exception &ex)
	{
		progress_reporter::it_failed(desc, ex);
		--indentation_;
		stm_
		  << "\r" << indent()
		  << colorizer_.red()
		  << "[ FAIL ]"
		  << colorizer_.reset()
		  << " it " << desc
		  << std::endl;
		stm_.flush();
	}

	virtual void it_unknown_error(const char *desc)
	{
		progress_reporter::it_unknown_error(desc);
		--indentation_;
		stm_
		  << "\r" << indent()
		  << colorizer_.red()
		  << "-ERROR->"
		  << colorizer_.reset()
		  << " it " << desc
		  << std::endl;
		stm_.flush();
	}

private:
	std::string indent()
	{
		return std::string(2*indentation_, ' ');
	}

	std::ostream &stm_;
	const detail::colorizer &colorizer_;
	int indentation_;
};
}
}

#endif
