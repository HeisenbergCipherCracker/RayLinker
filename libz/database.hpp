#include <sqlite3.h>
#include <iostream>


#include <stdio.h>
#include <sqlite3.h>

namespace database{


    void database() {
        sqlite3 *db;
        char *errMessage = 0;
        int rc;

        rc = sqlite3_open("test.db", &db);

        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cerr << "Opened database successfully" << std::endl;
        }

        const char *sql = "CREATE TABLE COMPANY(" \
                        "ID INT PRIMARY KEY     NOT NULL," \
                        "NAME           TEXT    NOT NULL," \
                        "AGE            INT     NOT NULL," \
                        "ADDRESS        CHAR(50)," \
                        "SALARY         REAL );";

        rc = sqlite3_exec(db, sql, 0, 0, &errMessage);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMessage << std::endl;
            sqlite3_free(errMessage);
        } else {
            std::cerr << "Table created successfully" << std::endl;
        }

        sqlite3_close(db);

    }
};