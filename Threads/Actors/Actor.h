#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

class Actor
{
public:
	typedef std::function<void()> WorkFunction;

	Actor()
	: _stop(false),
	  _worker([=]() { doWork(); })
	{}

	~Actor()
	{
		std::cout << "Destroying actor" << std::endl;
		send(nullptr);
		_worker.join();
		std::cout << "Destruction complete" << std::endl;
	}

	void ping()
	{
		send([=]() {
			std::cout << "Doing some work in thread " << std::this_thread::get_id() << std::endl;
			std::this_thread::sleep_for(2s);
		});
	}

private:
	void send(WorkFunction w)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_workQueue.push_back(w);
	}

	// This is the function that runs in the worker thread
	void doWork()
	{
		std::cout << "Start thread " << std::this_thread::get_id() << std::endl;
		while(! _stop)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			while(_workQueue.empty())
				_signal.wait(lock);

			auto workItem = _workQueue.front();
			if(workItem == nullptr)
			{
				_stop = true;
			}
			else
			{
				_workQueue.pop_front();
				workItem();
			}
		}
		std::cout << "Exit thread " << std::this_thread::get_id() << std::endl;
	}

	std::deque<WorkFunction> _workQueue;

	std::atomic_bool _stop;
	std::mutex 		_mutex;
	std::condition_variable _signal;
	std::thread 	_worker;
};
