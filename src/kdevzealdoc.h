#ifndef KDEVZEALDOC_H
#define KDEVZEALDOC_H

#include <interfaces/iplugin.h>

class kdevzealdoc : public KDevelop::IPlugin
{
    Q_OBJECT

public:
    // KPluginFactory-based plugin wants constructor with this signature
    kdevzealdoc(QObject* parent, const QVariantList& args);
};

#endif // KDEVZEALDOC_H
