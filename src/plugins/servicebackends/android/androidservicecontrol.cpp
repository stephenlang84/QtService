#include "androidservicecontrol.h"
#include "androidserviceplugin.h"

#include <QtCore/private/qandroidextras_p.h>

using namespace QtService;

Q_LOGGING_CATEGORY(logControl, "qt.service.plugin.android.control")

AndroidServiceControl::AndroidServiceControl(QString &&serviceId, QObject *parent) :
	ServiceControl{std::move(serviceId), parent}
{}

QString AndroidServiceControl::backend() const
{
	return QStringLiteral("android");
}

ServiceControl::SupportFlags AndroidServiceControl::supportFlags() const
{
	return SupportFlag::StartStop |
			SupportFlag::CustomCommands |
			SupportFlag::SetEnabled;
}

bool AndroidServiceControl::serviceExists() const
{
	const auto svcInfo = serviceInfo();
    QJniEnvironment env;
    env.checkAndClearExceptions();
	return svcInfo.isValid() &&
			!svcInfo.getObjectField("name", "Ljava/lang/String;").toString().isEmpty();
}

bool AndroidServiceControl::isEnabled() const
{
	auto componentName = serviceComponent();
	if (!componentName.isValid())
		return false;

    QJniEnvironment env;
    env.checkAndClearExceptions();
    static const auto COMPONENT_ENABLED_STATE_DEFAULT = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "COMPONENT_ENABLED_STATE_DEFAULT");
    static const auto COMPONENT_ENABLED_STATE_ENABLED = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "COMPONENT_ENABLED_STATE_ENABLED");

    auto pm = QJniObject(QtAndroidPrivate::context()).callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");
	const auto componentState = pm.callMethod<jint>("getComponentEnabledSetting", "(Landroid/content/ComponentName;)I",
													componentName.object());
	qCDebug(logControl) << "Android component state:" << componentState;

	if (componentState == COMPONENT_ENABLED_STATE_ENABLED)
		return true;
	else if (componentState != COMPONENT_ENABLED_STATE_DEFAULT)
		return false;
	else {
		// special case: was never changed, so value from manifest is needed
		auto svcInfo = serviceInfo();
		if (svcInfo.isValid())
			return svcInfo.callMethod<jboolean>("isEnabled");
		else
			return false;
	}
}

QVariant AndroidServiceControl::callGenericCommand(const QByteArray &kind, const QVariantList &args)
{
	if (kind == "bind") {
		if (args.size() < 1 || args.size() > 2) {
			setError(tr("The bind command must be called with a QAndroidServiceConnection* as first parameter and QtAndroid::BindFlags as optional second parameter"));
			return {};
		}

		auto connection = args.value(0).value<QAndroidServiceConnection*>();
		if (!connection) {
			setError(tr("The bind command must be called with a QAndroidServiceConnection* as first parameter and QtAndroid::BindFlags as optional second parameter"));
			return {};
		}
        auto flags = args.size() == 2 ? args.value(1).value<QtAndroidPrivate::BindFlags>() : QtAndroidPrivate::BindFlag::None;

		return bind(connection, flags);
	} else if (kind == "unbind") {
		if (args.size() != 1) {
			setError(tr("The unbind command must be called with a QAndroidServiceConnection* as only parameter"));
			return {};
		}

		auto connection = args.value(0).value<QAndroidServiceConnection*>();
		if (!connection) {
			setError(tr("The unbind command must be called with a QAndroidServiceConnection* as only parameter"));
			return {};
		}

		unbind(connection);
		return {};
	} else if (kind == "startWithIntent") {
		if (args.size() != 1) {
			setError(tr("The startWithIntent command must be called with a QAndroidIntent as only parameter"));
			return {};
		}

		auto intent = args.value(0).value<QAndroidIntent>();
		if (!intent.handle().isValid()) {
			setError(tr("The startWithIntent command must be called with a QAndroidIntent as only parameter"));
			return {};
		}

		startWithIntent(intent);
		return {};
	} else
		return ServiceControl::callGenericCommand(kind, args);
}

ServiceControl::BlockMode AndroidServiceControl::blocking() const
{
	return BlockMode::NonBlocking;
}

bool AndroidServiceControl::start()
{
	if (!serviceExists())
		return false;
    QJniEnvironment env;
    env.checkAndClearExceptions();
    QJniObject context = QtAndroidPrivate::context();
	QAndroidIntent intent{context, qUtf8Printable(serviceId())};
    context.callObjectMethod("startService", "(Landroid/content/Intent;)Landroid/content/ComponentName;",
							 intent.handle().object());
	return true;
}

bool AndroidServiceControl::stop()
{
	if(!serviceExists())
		return false;

    QJniEnvironment env;
    env.checkAndClearExceptions();
    QJniObject context = QtAndroidPrivate::context();
	QAndroidIntent intent{context, qUtf8Printable(serviceId())};
    return context.callMethod<jboolean>("stopService", "(Landroid/content/Intent;)Z",
										intent.handle().object());
}

bool AndroidServiceControl::setEnabled(bool enabled)
{
	auto componentName = serviceComponent();
	if(!componentName.isValid())
		return false;

    QJniEnvironment env;
    env.checkAndClearExceptions();
    static const auto COMPONENT_ENABLED_STATE_ENABLED = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "COMPONENT_ENABLED_STATE_ENABLED");
    static const auto COMPONENT_ENABLED_STATE_DISABLED = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "COMPONENT_ENABLED_STATE_DISABLED");

    auto pm = QJniObject(QtAndroidPrivate::context()).callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");
	pm.callMethod<void>("setComponentEnabledSetting", "(Landroid/content/ComponentName;II)V",
						componentName.object(),
						enabled ? COMPONENT_ENABLED_STATE_ENABLED : COMPONENT_ENABLED_STATE_DISABLED,
						0);
	return !env->ExceptionCheck();
}

QString AndroidServiceControl::serviceName() const
{
	return serviceId().split(QLatin1Char('.')).last();
}

QByteArray AndroidServiceControl::jniServiceId() const
{
	return serviceId().replace(QLatin1Char('.'), QLatin1Char('/')).toUtf8();
}

QJniObject AndroidServiceControl::serviceComponent() const
{
    QJniEnvironment env;
    env.checkAndClearExceptions();

    QJniObject jClass{static_cast<jobject>(env->FindClass(jniServiceId().constData()))};
	if (!jClass.isValid()) {
		qCCritical(logControl) << "Cannot find jclass for" << jniServiceId();
		return {};
	}

    QJniObject componentName {
		"android/content/ComponentName", "(Landroid/content/Context;Ljava/lang/Class;)V",
        QtAndroidPrivate::context(),
		jClass.object()
	};
	if (componentName.isValid())
		return componentName;
	else {
		qCCritical(logControl) << "Unable to create component name from jclass" << jClass.toString();
		return {};
	}
}

QJniObject AndroidServiceControl::serviceInfo() const
{
	const auto componentName = serviceComponent();
	if (!componentName.isValid())
		return {};

    QJniEnvironment env;
    env.checkAndClearExceptions();
    static const auto MATCH_DISABLED_COMPONENTS = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "MATCH_DISABLED_COMPONENTS");
    static const auto MATCH_DIRECT_BOOT_AWARE = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "MATCH_DIRECT_BOOT_AWARE");
    static const auto MATCH_DIRECT_BOOT_UNAWARE = QJniObject::getStaticField<jint>("android/content/pm/PackageManager", "MATCH_DIRECT_BOOT_UNAWARE");

    const auto pm = QJniObject(QtAndroidPrivate::context()).callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");
	const auto svcInfo = pm.callObjectMethod("getServiceInfo", "(Landroid/content/ComponentName;I)Landroid/content/pm/ServiceInfo;",
											 componentName.object(),
											 MATCH_DISABLED_COMPONENTS | MATCH_DIRECT_BOOT_AWARE | MATCH_DIRECT_BOOT_UNAWARE);
	if (svcInfo.isValid())
		return svcInfo;
	else {
		qCCritical(logControl) << "Unable to get service info for component" << componentName.toString();
		return {};
	}
}

bool AndroidServiceControl::bind(QAndroidServiceConnection *serviceConnection, QtAndroidPrivate::BindFlags flags)
{
	if (!serviceExists())
		return false;

    QJniEnvironment env;
    env.checkAndClearExceptions();
    auto context = QtAndroidPrivate::context();
	QAndroidIntent intent{context, qUtf8Printable(serviceId())};
    return QtAndroidPrivate::bindService(intent, *serviceConnection, flags);
}

void AndroidServiceControl::unbind(QAndroidServiceConnection *serviceConnection)
{
    QJniEnvironment env;
    env.checkAndClearExceptions();
    auto context = QtAndroidPrivate::context();
    QJniObject(context).callMethod<void>("unbindService", "(Landroid/content/ServiceConnection;)V",
							 serviceConnection->handle().object());
}

void AndroidServiceControl::startWithIntent(const QAndroidIntent &intent)
{
    QJniEnvironment env;
    env.checkAndClearExceptions();
    auto context = QtAndroidPrivate::context();
	intent.handle().callObjectMethod("setClass", "(Landroid/content/Context;Ljava/lang/Class;)Landroid/content/Intent;",
                                     context,
									 env->FindClass(jniServiceId().constData()));
    QJniObject(context).callObjectMethod("startService", "(Landroid/content/Intent;)Landroid/content/ComponentName;",
							 intent.handle().object());
}
