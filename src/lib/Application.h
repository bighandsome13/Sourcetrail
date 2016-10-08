#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "component/ComponentManager.h"
#include "utility/messaging/MessageListener.h"
#include "utility/messaging/type/MessageActivateWindow.h"
#include "utility/messaging/type/MessageEnteredLicense.h"
#include "utility/messaging/type/MessageFinishedParsing.h"
#include "utility/messaging/type/MessageLoadProject.h"
#include "utility/messaging/type/MessageRefresh.h"
#include "utility/messaging/type/MessageSwitchColorScheme.h"
#include "Project.h"
#include "ProjectFactory.h"

class DialogView;
class IDECommunicationController;
class MainView;
class NetworkFactory;
class StorageCache;
class Version;
class ViewFactory;
class ProjectFactoryModule;

class Application
	: public MessageListener<MessageActivateWindow>
	, public MessageListener<MessageEnteredLicense>
	, public MessageListener<MessageFinishedParsing>
	, public MessageListener<MessageLoadProject>
	, public MessageListener<MessageRefresh>
	, public MessageListener<MessageSwitchColorScheme>
{
public:
	static void createInstance(const Version& version, ViewFactory* viewFactory, NetworkFactory* networkFactory);
	static std::shared_ptr<Application> getInstance();
	static void destroyInstance();

	static void loadSettings();
	static void loadStyle(const FilePath& colorSchemePath);

	void addProjectFactoryModule(std::shared_ptr<ProjectFactoryModule> module);

	~Application();

	const std::shared_ptr<Project> getCurrentProject();

	void createAndLoadProject(const FilePath& projectSettingsFilePath);
	void refreshProject(bool force);
	bool hasGUI();

	int handleDialog(const std::string& message);
	int handleDialog(const std::string& message, const std::vector<std::string>& options);

	void setTitle(const std::string& title);

	bool isInTrial() const;

private:
	static std::shared_ptr<Application> s_instance;

	Application(bool withGUI=true);

	virtual void handleMessage(MessageActivateWindow* message);
	virtual void handleMessage(MessageEnteredLicense* message);
	virtual void handleMessage(MessageFinishedParsing* message);
	virtual void handleMessage(MessageLoadProject* message);
	virtual void handleMessage(MessageRefresh* message);
	virtual void handleMessage(MessageSwitchColorScheme* message);

	void startMessagingAndScheduling();

	void updateRecentProjects(const FilePath& projectSettingsFilePath);

	DialogView* getDialogView() const;

	const bool m_hasGUI;
	ProjectFactory m_projectFactory;
	std::shared_ptr<Project> m_project;
	std::shared_ptr<StorageCache> m_storageCache;

	std::shared_ptr<MainView> m_mainView;
	std::shared_ptr<ComponentManager> m_componentManager;

	std::shared_ptr<IDECommunicationController> m_ideCommunicationController;

	bool m_isInTrial;
};

#endif // APPLICATION_H
