#include "kdevzealdoc.h"

#include <debug.h>

#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(kdevzealdocFactory, "kdevzealdoc.json", registerPlugin<kdevzealdoc>(); )

kdevzealdoc::kdevzealdoc(QObject *parent, const QVariantList& args)
    : KDevelop::IPlugin(QStringLiteral("kdevzealdoc"), parent)
{
    Q_UNUSED(args);

    qCDebug(PLUGIN_KDEVZEALDOC) << "Hello world, my plugin is loaded!";
}

// needed for QObject class created from K_PLUGIN_FACTORY_WITH_JSON
#include "kdevzealdoc.moc"
#include "moc_kdevzealdoc.cpp"
