#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <string>
#include <vector>
#include <unordered_map>

using Row = std::unordered_map<std::string, std::string>;
using Table = std::vector<Row>;

class DatabaseHandler {
public:
    virtual ~DatabaseHandler() {}
    virtual void setConfFile(const std::string& confFile) = 0;
    virtual void prepareQuery(const std::string& query) = 0;
    virtual void addParameter(int index, const std::string& value) = 0;
    virtual void execute() = 0;
    virtual Row fetch() = 0;
    virtual Table fetchAll() = 0;
};

#endif
