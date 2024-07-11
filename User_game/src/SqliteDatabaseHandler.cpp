#include "SqliteDatabaseHandler.h"
#include "DatabaseHandler.h"
#include <iostream>

SqliteDatabaseHandler::SqliteDatabaseHandler() : db(nullptr), stmt(nullptr) {}

SqliteDatabaseHandler::~SqliteDatabaseHandler() {
    closeDatabase();
}

void SqliteDatabaseHandler::setConfFile(const std::string& confFile) {
    this->confFile = confFile;
    openDatabase();
}

void SqliteDatabaseHandler::openDatabase() {
    if (sqlite3_open(confFile.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }
}

void SqliteDatabaseHandler::closeDatabase() {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

void SqliteDatabaseHandler::prepareQuery(const std::string& query) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }
}

void SqliteDatabaseHandler::addParameter(int index, const std::string& value) {
    if (sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind parameter: " << sqlite3_errmsg(db) << std::endl;
    }
}

void SqliteDatabaseHandler::execute() {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_reset(stmt);
}

Row SqliteDatabaseHandler::fetch() {
    Row row;
    int cols = sqlite3_column_count(stmt);
    int res = sqlite3_step(stmt);

    if (res == SQLITE_ROW) {
        for (int col = 0; col < cols; col++) {
            row[sqlite3_column_name(stmt, col)] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
        }
    }
    return row;
}

Table SqliteDatabaseHandler::fetchAll() {
    Table table;
    int cols = sqlite3_column_count(stmt);
    int res;

    while ((res = sqlite3_step(stmt)) == SQLITE_ROW) {
        Row row;
        for (int col = 0; col < cols; col++) {
            row[sqlite3_column_name(stmt, col)] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, col));
        }
        table.push_back(row);
    }
    return table;
}
