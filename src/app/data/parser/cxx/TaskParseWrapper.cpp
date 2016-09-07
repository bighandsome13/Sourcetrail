#include "data/parser/cxx/TaskParseWrapper.h"

#include "component/view/DialogView.h"
#include "data/PersistentStorage.h"
#include "utility/file/FileRegister.h"
#include "utility/messaging/type/MessageFinishedParsing.h"
#include "utility/utility.h"

TaskParseWrapper::TaskParseWrapper(
	PersistentStorage* storage,
	std::shared_ptr<FileRegister> fileRegister,
	DialogView* dialogView
)
	: m_storage(storage)
	, m_fileRegister(fileRegister)
	, m_dialogView(dialogView)
{
}

TaskParseWrapper::~TaskParseWrapper()
{
}

void TaskParseWrapper::setTask(std::shared_ptr<Task> task)
{
	if (task)
	{
		m_taskRunner = std::make_shared<TaskRunner>(task);
	}
}

void TaskParseWrapper::doEnter()
{
	m_dialogView->updateIndexingDialog(0, m_fileRegister->getSourceFilesCount(), "");

	m_start = utility::durationStart();
	m_storage->startParsing();
}

Task::TaskState TaskParseWrapper::doUpdate()
{
	return m_taskRunner->update();
}

void TaskParseWrapper::doExit()
{
	m_dialogView->showProgressDialog("Finish Indexing", "Optimizing database");

	m_storage->optimizeMemory();

	m_dialogView->showProgressDialog("Finish Indexing", "Building caches");

	m_storage->finishParsing();

	m_dialogView->hideProgressDialog();

	MessageFinishedParsing().dispatch();

	m_dialogView->finishedIndexingDialog(
		m_fileRegister->getParsedSourceFilesCount(),
		m_fileRegister->getSourceFilesCount(),
		utility::duration(m_start),
		m_storage->getErrorCount()
	);
}

void TaskParseWrapper::doReset()
{
	m_taskRunner->reset();
}
