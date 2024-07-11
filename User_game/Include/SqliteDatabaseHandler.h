#ifndef SQLITEDATABASEHANDLER_H
#define SQLITEDATABASEHANDLER_H

#include "DatabaseHandler.h"
#include <sqlite3.h>
#include <iostream>

class SqliteDatabaseHandler : public DatabaseHandler {
private:
    sqlite3* db;
    sqlite3_stmt* stmt;
    std::string confFile;

public:
    SqliteDatabaseHandler();
    ~SqliteDatabaseHandler();
    void setConfFile(const std::string& confFile) override;
    void prepareQuery(const std::string& query) override;
    void addParameter(int index, const std::string& value) override;
    void execute() override;
    Row fetch() override;
    Table fetchAll() override;

private:
    void openDatabase();
    void closeDatabase();
};

#endif // SQLITEDATABASEHANDLER_H
