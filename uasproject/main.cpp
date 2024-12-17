#include <iostream>
#include <stdio.h>
#include <iomanip>
#include "sqlite3.h"

using namespace std;

void connection();
void insertR();
void retrieve();
void deleteR();
void update();

sqlite3 *db;
sqlite3_stmt *stmt;
int result, tahun;
string query, judul, penulis;

int main()
{
    //call connection function
    connection();

    int menu;
    cout << "\n-----------------------------------------------\n";
    cout << "Tekan [1] Untuk memasukkan Data baru \n";
    cout << "Tekan [2] Untuk melihat semua Data \n";
    cout << "Tekan [3] Untuk memperbaharui Data \n";
    cout << "Tekan [4] Untuk menghapus sebuah Data \n";
    cout << "Tekan [5] Keluar \n";
    cout << "\n-----------------------------------------------\n";

    cin >> menu;
    getchar();

    if(menu == 1)
    {
        insertR();
        main();
    }
    else if(menu == 2)
    {
        retrieve();
        main();
    }
    else if(menu == 3)
    {
        update();
        main();
    }
    else if(menu == 4)
    {
        deleteR();
        main();
    }
    else if(menu == 5)
    {
        return 0;
    }
    else
    {
        cout << "Perintah Tidak Benar \n" ;
    }

    sqlite3_close(db);
    return 0;
}

void connection()
{
    if(sqlite3_open("buku.db", &db) == SQLITE_OK)
    {
        result = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS buku(judul varchar(255), tahun INT, penulis varchar(255));", -1, &stmt, NULL);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(result != SQLITE_OK)
        {
            cout << "Error: " << sqlite3_errmsg(db) << "\n";
        }

    }
}

void insertR()
{
   cout << "\n -> Masukkan Judul Buku: \n";
   getline(cin, judul);

   cout << "\n -> Masukkan Tahun Terbit: \n";
   cin >> tahun;
   getchar();

   cout << "\n -> Masukkan Nama Penulis: \n";
   getline(cin, penulis);

   query = "INSERT INTO buku(judul, tahun, penulis) VALUES(?,?,?);";

   result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
   sqlite3_bind_text(stmt, 1, judul.c_str(), judul.length(), SQLITE_TRANSIENT);
   sqlite3_bind_int(stmt, 2, tahun);
   sqlite3_bind_text(stmt, 3, penulis.c_str(), penulis.length(), SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(result != SQLITE_OK)
    {
        cout << "\n -> ERROR: " << sqlite3_errmsg(db) << "\n";
    }
    else
    {
        cout << "\n -> Data Berhasil Di Masukkan \n";
    }

}

void retrieve()
{
    query = "SELECT rowid,* FROM buku";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if(result != SQLITE_OK)
    {
        cout << "ERROR: " << sqlite3_errmsg(db) << "\n";
    }
    else
    {
        cout << "\n-------------------------------------------------------------------------------------------------\n";
        cout << left << "\t" << setw(15) << "ID "
             << setw(25) << "Judul "
             << setw(20) << "Tahun "
             << setw(25) << "Penulis";
        cout << "\n-------------------------------------------------------------------------------------------------\n";
        while( (result = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            cout << left << "\t" << setw(15) << sqlite3_column_text(stmt, 0)
                         << setw(25) << sqlite3_column_text(stmt, 1)
                         << setw(20) << sqlite3_column_text(stmt, 2)
                         << setw(25) << sqlite3_column_text(stmt, 3) << "\n";
        }
        cout << "\n------------------------------------------------------------------------------------------------\n";
    }
}

void deleteR()
{
    int id;
    char confirm;
    cout << "\n Data manakah yang ingin Anda hapus? Ketik [id] nomor: \n";
    cin >> id;

    getchar();

    cout << "\n Apakah Anda yakin untuk menghapusnya? Ketik [y] untuk Ya, [n] untuk Tidak \n";
    cin >> confirm;

    if(confirm == 'y')
    {
        query = "DELETE FROM buku WHERE rowid = ?";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(result == SQLITE_OK)
        {
            cout << "\n Data Berhasil Dihapus! \n";
        }
        else
        {
            cout << "\n Failed: " << sqlite3_errmsg(db) << "\n";
        }
    }
    else
    {
        cout << "\n Operasi Hapus Dibatalkan. \n";
    }
}

void update()
{
    int id;
    cout << "\n Data mana yang ingin kamu Perbaharui? Ketik [id] nomor: \n";
    cin >> id;
    getchar();

    cout << "Ketik Judul yang ingin diperbaharui: \n";
    getline(cin, judul);

    cout << "Ketik Tahun yang ingin diperbaharui: \n";
    cin >> tahun;
    getchar();

    cout << "Ketik Penulis yang ingin diperbaharui: \n";
    getline(cin, penulis);

    query = "Update buku SET judul = ?, tahun = ?, penulis = ? WHERE rowid = ?";

    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, judul.c_str(), judul.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, tahun);
    sqlite3_bind_text(stmt, 3, penulis.c_str(), penulis.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(result != SQLITE_OK)
    {
        cout << "\n Failed: " << sqlite3_errmsg(db) << "\n";
    }
    else
    {
        cout << "\n Data Telah Berhasil Diperbaharui! \n";
    }
}
