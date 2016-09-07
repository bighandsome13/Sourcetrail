#ifndef TASK_GROUP_PARALLEL_H
#define TASK_GROUP_PARALLEL_H

#include <map>
#include <mutex>
#include <thread>

#include "utility/scheduling/TaskGroup.h"
#include "utility/scheduling/TaskRunner.h"

class TaskGroupParallel
	: public TaskGroup
{
public:
	TaskGroupParallel();
	virtual ~TaskGroupParallel();

	virtual void addTask(std::shared_ptr<Task> task);

private:
	struct TaskInfo
	{
		TaskInfo(std::shared_ptr<TaskRunner> taskRunner)
			: taskRunner(taskRunner)
			, active(false)
		{}
		std::shared_ptr<TaskRunner> taskRunner;
		std::shared_ptr<std::thread> thread;
		volatile bool active;
	};

	virtual void doEnter();
	virtual TaskState doUpdate();
	virtual void doExit();
	virtual void doReset();

	void processTaskThreaded(std::shared_ptr<TaskInfo> taskInfo);
	int getActveTaskCount() const;

	std::vector<std::shared_ptr<TaskInfo>> m_tasks;
	bool m_needsToStartThreads;

	volatile bool m_taskFailed;
	volatile int m_activeTaskCount;
	mutable std::mutex m_activeTaskCountMutex;
};

#endif // TASK_GROUP_PARALLEL_H
