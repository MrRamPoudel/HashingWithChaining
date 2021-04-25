#pragma once

#include <chrono>
class Timer {
public:
	void start()
	{
		if (!started) //check if it is first time calling
		{
			started = true;
			paused = false;
			totalTime = std::chrono::duration < long double> (0);
			current = std::chrono::steady_clock::now();

		}
		else if (paused) //check if it stop was called
		{
			current = std::chrono::steady_clock::now();
			paused = false;
		}
	}
	void stop()
	{
		if (started && !paused) //calling stop twice does nothing
		{
			auto exact = std::chrono::steady_clock::now();
			totalTime += std::chrono::duration_cast<std::chrono::duration<long double>> (exact - current);
			paused = true;
		}
	}
	void reset()
	{
		if (started)
		{
			started = paused = false;
			current = std::chrono::steady_clock::now();
			totalTime = std::chrono::duration<long double>(0);
		}
	}

	template<class duration = std::chrono::milliseconds>
	typename duration::rep count() const 
	{
		if (started)
		{
			if (paused)
			{
				return std::chrono::duration_cast<duration>(totalTime).count();
			}
			else
			{
				return std::chrono::duration_cast<duration>(totalTime + (std::chrono::steady_clock::now() - current)).count();
			}
		}
		return 0;
	}
	Timer(bool isStarted = false) :started(0), paused(0), current(std::chrono::steady_clock::now()),
		totalTime(std::chrono::duration<long double>(0)) {
		if (isStarted) {
			start();
		}
	}
	
private:
	bool started;
	bool paused;
	std::chrono::steady_clock::time_point current;
	std::chrono::duration<long double> totalTime;
};