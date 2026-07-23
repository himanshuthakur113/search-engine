#include "sqlite_db.h"
#include <iostream>
#include <stdexcept>

SQLiteDB::SQLiteDB(const std::string& filename)
{
    int res = sqlite3_open(filename.c_str(), &db);
    if(res!= SQLITE_OK)
    {
        throw std::runtime_error("Databases not connected");
    }
    
    const char* createTable = "CREATE TABLE IF NOT EXISTS pages ("
                                "id INTEGER PRIMARY KEY,"
                                "url TEXT UNIQUE NOT NULL,"
                                "html TEXT NOT NULL"
                                ");";
    
    char* errMsg = nullptr;
    res = sqlite3_exec(db,createTable,nullptr,nullptr,&errMsg);
    if(res != SQLITE_OK)
    {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }

    std::cout << "database initialized" <<"\n";
}

SQLiteDB::~SQLiteDB()
{
    if(db != nullptr)
    {
        sqlite3_close(db);
    }
}

void SQLiteDB::insert(const Page &page)
{
    const char* sql = "INSERT INTO pages(url,html) VALUES(?,?);";
    
    sqlite3_stmt* stmt = nullptr;

    int res = sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(res != SQLITE_OK)
    {
        throw std::runtime_error("Can not create prepared statement");
    }

    sqlite3_bind_text(stmt,1,page.url.c_str(),-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,2,page.html.c_str(),-1,SQLITE_TRANSIENT);

    res = sqlite3_step(stmt);
    if(res != SQLITE_DONE)
    {   
        sqlite3_finalize(stmt);
        throw std::runtime_error("Cannot insert value ");
    }
    sqlite3_finalize(stmt);
}

bool SQLiteDB::exists(const std::string &url)
{
    const char* sql = "SELECT 1 FROM pages WHERE url = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;
    int res = sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(res != SQLITE_OK)
    {
        throw std::runtime_error("Cannot create insert command object");
    }

    sqlite3_bind_text(stmt,1,url.c_str(),-1,SQLITE_TRANSIENT);
    res = sqlite3_step(stmt);
    if(res == SQLITE_ROW)
        {
            sqlite3_finalize(stmt);
            return true;
        }
        sqlite3_finalize(stmt);
        return false;
}

Page SQLiteDB::get(int id)
{
    const char* sql = "SELECT id,url,html FROM pages WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    int res = sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(res != SQLITE_OK)
    {
        throw std::runtime_error("Cannot create get command object");
    }

    sqlite3_bind_int(stmt,1,id);
    res = sqlite3_step(stmt);

    if(res != SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Page not found");
    }

    Page page;

    page.id = sqlite3_column_int(stmt, 0);

    page.url = (const char*)sqlite3_column_text(stmt, 1);

    page.html =  (const char*)sqlite3_column_text(stmt, 2);
       
    sqlite3_finalize(stmt);

    return page;

}

std::string SQLiteDB::fetch(const std::string& url)
{
    const char* sql = "SELECT html FROM pages WHERE url  = ?LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    int res = sqlite3_prepare_v2(db,sql,-1,&stmt,nullptr);
    if(res != SQLITE_OK)
    {
        throw std::runtime_error("Cannot create the fetch command");
    }
    sqlite3_bind_text(stmt,1,url.c_str(),-1,SQLITE_TRANSIENT);
    res = sqlite3_step(stmt);

    if(res != SQLITE_ROW)
        {
            throw std::runtime_error("page not found");
        }
    std::string html = (const char*)sqlite3_column_text(stmt, 0);
    sqlite3_finalize(stmt);
    return html;
 
}   
