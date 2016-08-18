//
// Created by Ivan Shynkarenka on 01.08.2016.
//

#include "benchmark/cppbenchmark.h"

#include "logging/config.h"
#include "logging/logger.h"

using namespace CppLogging;

const uint64_t iterations = 1000000;
const auto settings = CppBenchmark::Settings().Iterations(iterations).ThreadsRange(1, 8, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

class NullConfigFixture : public virtual CppBenchmark::FixtureThreads
{
protected:
    NullConfigFixture()
    {
        auto null_sink = std::make_shared<CppLogging::AsyncProcessor>();
        null_sink->appenders().push_back(std::make_shared<CppLogging::NullAppender>());
        CppLogging::Config::ConfigLogger("null", null_sink);
    }

    void Cleanup(CppBenchmark::ContextThreads& context) override
    {
        // Update benchmark metrics
        context.metrics().AddIterations(context.threads() * iterations - 1);
    }
};

class BinaryConfigFixture : public virtual CppBenchmark::FixtureThreads
{
protected:
    BinaryConfigFixture()
    {
        auto null_sink = std::make_shared<CppLogging::AsyncProcessor>();
        null_sink->appenders().push_back(std::make_shared<CppLogging::NullAppender>());
        auto binary_sink = std::make_shared<CppLogging::Processor>();
        binary_sink->layouts().push_back(std::make_shared<CppLogging::BinaryLayout>());
        binary_sink->processors().push_back(null_sink);
        CppLogging::Config::ConfigLogger("binary", binary_sink);
    }

    void Cleanup(CppBenchmark::ContextThreads& context) override
    {
        // Update benchmark metrics
        context.metrics().AddIterations(context.threads() * iterations - 1);
    }
};

class TextConfigFixture : public virtual CppBenchmark::FixtureThreads
{
protected:
    TextConfigFixture()
    {
        auto null_sink = std::make_shared<CppLogging::AsyncProcessor>();
        null_sink->appenders().push_back(std::make_shared<CppLogging::NullAppender>());
        auto text_sink = std::make_shared<CppLogging::Processor>();
        text_sink->layouts().push_back(std::make_shared<CppLogging::TextLayout>());
        text_sink->processors().push_back(null_sink);
        CppLogging::Config::ConfigLogger("text", text_sink);
    }

    void Cleanup(CppBenchmark::ContextThreads& context) override
    {
        // Update benchmark metrics
        context.metrics().AddIterations(context.threads() * iterations - 1);
    }
};

BENCHMARK_THREADS_FIXTURE(NullConfigFixture, "AsyncProcessor-null", settings)
{
    thread_local Logger logger = CppLogging::Config::CreateLogger("null");
    logger.Info("Test message");
}

BENCHMARK_THREADS_FIXTURE(BinaryConfigFixture, "AsyncProcessor-binary", settings)
{
    thread_local Logger logger = CppLogging::Config::CreateLogger("binary");
    logger.Info("Test message");
}

BENCHMARK_THREADS_FIXTURE(TextConfigFixture, "AsyncProcessor-text", settings)
{
    thread_local Logger logger = CppLogging::Config::CreateLogger("text");
    logger.Info("Test message");
}

BENCHMARK_MAIN()
