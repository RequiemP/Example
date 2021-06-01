#include "moduleinfo.h"

ModuleInfo::ModuleInfo(QString const& name, char const& data, int const& dataLength) : m_data(data),
                                                                                          m_name(name),
                                                                                          m_dataLength(dataLength) { }

QString const& ModuleInfo::getName() const {
    return m_name;
}

char const& ModuleInfo::getData() const {
    return m_data;
}

int const& ModuleInfo::getDataLength() const {
    return m_dataLength;
}

ModuleInfo const& ModuleInfo::operator=(ModuleInfo const& other) {
    if (this == &other) { return *this; }
    this->m_name = other.m_name;
    this->m_data = other.m_data;
    this->m_dataLength = other.m_dataLength;
    return *this;
}
