#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <QString>

class ModuleInfo {
public:
    explicit ModuleInfo(QString const& name = "", char const& data = '\0', int const& dataLength = 0);

    QString const& getName() const;
    char const& getData() const;
    int const& getDataLength() const;
    ModuleInfo const& operator=(ModuleInfo const& other);

private:
    char m_data;
    QString m_name;
    int m_dataLength;
};

#endif // MODULEINFO_H
