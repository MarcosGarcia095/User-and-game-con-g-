#include <iostream>
#include <memory>
#include "SqliteDatabaseHandler.h"
#include "User.h"
#include "Game.h"

void createTables(std::unique_ptr<SqliteDatabaseHandler>& dbh) {
    dbh->prepareQuery("CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, email TEXT UNIQUE)");
    dbh->execute();
    dbh->prepareQuery("CREATE TABLE IF NOT EXISTS Games (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)");
    dbh->execute();
    dbh->prepareQuery("CREATE TABLE IF NOT EXISTS UserGames (userId INTEGER, gameId INTEGER, FOREIGN KEY(userId) REFERENCES Users(id), FOREIGN KEY(gameId) REFERENCES Games(id), PRIMARY KEY(userId, gameId))");
    dbh->execute();
}

void addUser(std::unique_ptr<SqliteDatabaseHandler>& dbh, const std::string& name, const std::string& email) {
    dbh->prepareQuery("INSERT INTO Users(name, email) VALUES(?, ?)");
    dbh->addParameter(1, name);
    dbh->addParameter(2, email);
    dbh->execute();
}

void addGame(std::unique_ptr<SqliteDatabaseHandler>& dbh, const std::string& name) {
    dbh->prepareQuery("INSERT INTO Games(name) VALUES(?)");
    dbh->addParameter(1, name);
    dbh->execute();
}

void associateUserGame(std::unique_ptr<SqliteDatabaseHandler>& dbh, int userId, int gameId) {
    dbh->prepareQuery("INSERT INTO UserGames(userId, gameId) VALUES(?, ?)");
    dbh->addParameter(1, std::to_string(userId));
    dbh->addParameter(2, std::to_string(gameId));
    dbh->execute();
}

void listUsers(std::unique_ptr<SqliteDatabaseHandler>& dbh) {
    dbh->prepareQuery("SELECT * FROM Users");
    Table users = dbh->fetchAll();
    for (const auto& user : users) {
        std::cout << "User ID: " << user.at("id") << ", Name: " << user.at("name") << ", Email: " << user.at("email") << std::endl;
    }
}

void listGames(std::unique_ptr<SqliteDatabaseHandler>& dbh) {
    dbh->prepareQuery("SELECT * FROM Games");
    Table games = dbh->fetchAll();
    for (const auto& game : games) {
        std::cout << "Game ID: " << game.at("id") << ", Name: " << game.at("name") << std::endl;
    }
}

void searchByUser(std::unique_ptr<SqliteDatabaseHandler>& dbh, const std::string& userName) {
    dbh->prepareQuery("SELECT g.name FROM Games g JOIN UserGames ug ON g.id = ug.gameId JOIN Users u ON u.id = ug.userId WHERE u.name = ?");
    dbh->addParameter(1, userName);
    Table games = dbh->fetchAll();
    std::cout << "Games for user " << userName << ":" << std::endl;
    for (const auto& game : games) {
        std::cout << game.at("name") << std::endl;
    }
}

void searchByGame(std::unique_ptr<SqliteDatabaseHandler>& dbh, const std::string& gameName) {
    dbh->prepareQuery("SELECT u.name FROM Users u JOIN UserGames ug ON u.id = ug.userId JOIN Games g ON g.id = ug.gameId WHERE g.name = ?");
    dbh->addParameter(1, gameName);
    Table users = dbh->fetchAll();
    std::cout << "Users for game " << gameName << ":" << std::endl;
    for (const auto& user : users) {
        std::cout << user.at("name") << std::endl;
    }
}

int main() {
    auto dbh = std::make_unique<SqliteDatabaseHandler>();
    dbh->setConfFile("configuration.ini");

    createTables(dbh);

    addUser(dbh, "Juan", "juan@example.com");
    addUser(dbh, "Pedro", "pedro@example.com");

    addGame(dbh, "Call of Duty");
    addGame(dbh, "FIFA 24");
    addGame(dbh, "F1 2020");
    addGame(dbh, "Black II");

    associateUserGame(dbh, 1, 1); // Juan -> Call of Duty
    associateUserGame(dbh, 1, 2); // Juan -> FIFA 24
    associateUserGame(dbh, 2, 3); // Pedro -> F1 2020
    associateUserGame(dbh, 2, 4); // Pedro -> Black II

    std::cout << "Users:" << std::endl;
    listUsers(dbh);
    std::cout << std::endl;

    std::cout << "Games:" << std::endl;
    listGames(dbh);
    std::cout << std::endl;

    std::cout << "Search by user 'Juan':" << std::endl;
    searchByUser(dbh, "Juan");
    std::cout << std::endl;

    std::cout << "Search by game 'F1 2020':" << std::endl;
    searchByGame(dbh, "F1 2020");
    std::cout << std::endl;

    return 0;
}
