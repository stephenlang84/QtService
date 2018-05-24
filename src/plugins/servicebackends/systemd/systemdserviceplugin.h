#ifndef SYSTEMDSERVICEPLUGIN_H
#define SYSTEMDSERVICEPLUGIN_H

#include <QtService/ServicePlugin>

class SystemdServicePlugin : public QObject, public QtService::ServicePlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID QtService_ServicePlugin_Iid FILE "systemd.json")
	Q_INTERFACES(QtService::ServicePlugin)

public:
	SystemdServicePlugin(QObject *parent = nullptr);

	QtService::ServiceBackend *createInstance(const QString &provider, QObject *parent) override;
};

#endif // SYSTEMDSERVICEPLUGIN_H
