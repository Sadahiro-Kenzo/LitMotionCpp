#include "pch.h"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "JobSystem.h"

std::unique_ptr<JobSystem::Impl> JobSystem::impl;

class JobSystem::Impl
{
private:
	std::vector<std::thread> m_workers;

	std::queue<std::packaged_task<void()>> m_jobQueue;
	std::mutex m_queueMutex;
	std::condition_variable m_condition;
	std::atomic<bool> m_stopping{ false };
private:
	void workerThread()
	{
		while (true)
		{
			std::packaged_task<void()> job;
			{
				std::unique_lock<std::mutex> lock(m_queueMutex);
				m_condition.wait(lock, [this]() { return !m_jobQueue.empty() || m_stopping.load(); });

				if (m_stopping.load() && m_jobQueue.empty())
				{
					return;
				}

				job = std::move(m_jobQueue.front());
				m_jobQueue.pop();
			}
			job();
		}
	}
public:
	Impl(size_t workerCount)
	{
		// Initialize worker threads
		for(size_t i = 0; i < workerCount; ++i)
		{
			m_workers.emplace_back([this]() {this->workerThread(); });
		}
	}
	~Impl()
	{
		// Cleanup worker threads
		m_stopping.store(true);
		m_condition.notify_all();

		for(auto& worker : m_workers)
		{
			if(worker.joinable())
			{
				worker.join();
			}
		}
	}

	JobHandle Schedule(const std::function<void()>& job)
	{
		std::packaged_task<void()> task(job);
		auto handle = task.get_future().share();
		{
			std::lock_guard<std::mutex> lock(m_queueMutex);
			m_jobQueue.push(std::move(task));
		}
		m_condition.notify_one();
		return handle;
	}
};

void JobSystem::initialize(size_t workerCount)
{
	impl = std::make_unique<Impl>(workerCount);
}

void JobSystem::shutdown()
{
	impl.reset();
}

JobHandle JobSystem::schedule(const std::function<void()>& job)
{
	return impl->Schedule(job);
}

JobHandle JobSystem::schedule(const std::function<void()>& job, const std::vector<JobHandle>& dependencies)
{
	auto wrapper = [job, dependencies]() {
		waitForAll(dependencies);
		job();
		};

	return impl->Schedule(wrapper);
}

JobHandle JobSystem::schedule(const std::function<void(size_t)>& job, size_t size, size_t innerLoopBatchCount)
{
	size_t totalBatches = (size + innerLoopBatchCount - 1) / innerLoopBatchCount;
	std::vector<JobHandle> batchHandles;
	for (size_t batchIndex = 0; batchIndex < totalBatches; ++batchIndex)
	{
		size_t start = batchIndex * innerLoopBatchCount;
		size_t end = std::min(start + innerLoopBatchCount, size);
		auto batchJob = [job, start, end]() {
			for (size_t i = start; i < end; ++i)
			{
				job(i);
			}
		};
		batchHandles.push_back(impl->Schedule(batchJob));
	}
	auto wrapper = [handles=std::move(batchHandles)]() {
		JobSystem::waitForAll(handles);
	};
	return impl->Schedule(wrapper);
}

void JobSystem::waitForAll(const std::vector<JobHandle>& jobHandles)
{
	for (auto& handle : jobHandles)
	{
		if (handle.valid())
		{
			handle.wait();
		}
	}
}

std::string JobSystem::getAsString(const std::thread::id& threadId)
{
	std::ostringstream stream;
	stream << threadId;
	return stream.str();
}
