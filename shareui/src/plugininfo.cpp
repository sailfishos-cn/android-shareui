#include "plugininfo.h"

PluginInfo::PluginInfo(): m_ready(false)
{
}

PluginInfo::~PluginInfo()
{

}

QList<TransferMethodInfo> PluginInfo::info() const
{
    return m_infoList;
}

void PluginInfo::query()
{
    TransferMethodInfo info;

    QStringList capabilities;
    capabilities << QLatin1String("image/*")
                 << QLatin1String("audio/*")
                 << QLatin1String("video/*")
                 << QLatin1String("text/*");

    info.displayName     = QLatin1String("Android");
    info.methodId        = QLatin1String("AndroidSharePlugin");
    info.shareUIPath     = QLatin1String("/usr/share/nemo-transferengine/plugins/AndroidShare.qml");
    info.capabilitities  = capabilities;
    m_infoList.clear();
    m_infoList << info;

    m_ready = true;
    Q_EMIT infoReady();
}

bool PluginInfo::ready() const
{
    return m_ready;
}
