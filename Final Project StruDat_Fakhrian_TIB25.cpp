#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct Tugas {
    int idTugas;
    string namaTugas;
    int deadline; 
    Tugas* next;  
};

struct NodeHistori {
    int idTugas;
    string namaTugas;
    NodeHistori* next;
};

struct NodeAnggota {
    string nim;
    string nama;
    NodeAnggota* next;
};

struct NodePohon {
    int idTugas;
    string namaTugas;
    NodePohon* left;
    NodePohon* right;
};

Tugas* awalTugas = NULL;
Tugas* antreanDepan = NULL;
Tugas* antreanBelakang = NULL;
NodeHistori* atasStack = NULL;
NodePohon* pohonTugas = NULL;

const int HASH_SIZE = 10;
NodeAnggota* hashTable[HASH_SIZE] = {NULL};

const int MAX_GRAPH = 5;
int adjMatrix[MAX_GRAPH][MAX_GRAPH] = {0};
string namaVertex[MAX_GRAPH] = {"Coding", "Database", "UI/UX", "Laporan", "Presentasi"};

void buatGaris() {
    cout << "========================================================\n";
}

void cetakBanner() {
    buatGaris();
    cout << "              Selamat Datang di Aplikasi            \n";
    cout << "   Sistem Manajemen Tugas & Kolaborasi Proyek Kelompok  \n";
    buatGaris();
}

void masukAntrean(int id, string nama, int dl) {
    Tugas* nBaru = new Tugas;
    nBaru->idTugas = id;
    nBaru->namaTugas = nama;
    nBaru->deadline = dl;
    nBaru->next = NULL;

    if (antreanDepan == NULL) {
        antreanDepan = antreanBelakang = nBaru;
    } else {
        antreanBelakang->next = nBaru;
        antreanBelakang = nBaru;
    }
    cout << "\n>> Berhasil: Tugas '" << nama << "' sudah dimasukkan ke dalam antrean pengerjaan.\n";
}

void tampilkanAntrean() {
    if (antreanDepan == NULL) {
        cout << "\n[Info] Antrean saat ini kosong.\n";
        return;
    }
    cout << "\n--- DAFTAR ANTRIAN TUGAS SEKARANG ---\n";
    Tugas* bantu = antreanDepan;
    int no = 1;
    while (bantu != NULL) {
        cout << no << ". ID: " << bantu->idTugas << " | Judul: " << bantu->namaTugas << " | Batas: " << bantu->deadline << " Hari\n";
        bantu = bantu->next;
        no++;
    }
}

void validasiTugas() {
    if (antreanDepan == NULL) {
        cout << "\n[Peringatan] Antrean kosong! Tidak ada tugas yang bisa divalidasi ke daftar aktif.\n";
        return;
    }
    Tugas* temp = antreanDepan;
    antreanDepan = antreanDepan->next;

    temp->next = awalTugas;
    awalTugas = temp;

    cout << "\n>> Sukses: Memindahkan tugas '" << temp->namaTugas << "' menuju daftar tugas aktif semester ini.\n";
}

void urutTugas() {
    if (awalTugas == NULL || awalTugas->next == NULL) {
        return; 
    }
    
    bool adaTukaran;
    Tugas* curr;
    Tugas* batas = NULL;

    do {
        adaTukaran = false;
        curr = awalTugas;

        while (curr->next != batas) {
            if (curr->deadline > curr->next->deadline) {
                int tId = curr->idTugas;
                string tNama = curr->namaTugas;
                int tDl = curr->deadline;

                curr->idTugas = curr->next->idTugas;
                curr->namaTugas = curr->next->namaTugas;
                curr->deadline = curr->next->deadline;

                curr->next->idTugas = tId;
                curr->next->namaTugas = tNama;
                curr->next->deadline = tDl;

                adaTukaran = true;
            }
            curr = curr->next;
        }
        batas = curr;
    } while (adaTukaran);
    cout << "\n>> Sukses: Urutan daftar tugas aktif diperbarui (Urut dari yang paling mendesak).\n";
}

void pushStack(int id, string nama) {
    NodeHistori* nBaru = new NodeHistori;
    nBaru->idTugas = id;
    nBaru->namaTugas = nama;
    nBaru->next = atasStack;
    atasStack = nBaru;
}

NodePohon* cariNilaiMinimum(NodePohon* akar) {
    NodePohon* curr = akar;
    while (curr && curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

NodePohon* hapusPohon(NodePohon* akar, int id) {
    if (akar == NULL) return akar;

    if (id < akar->idTugas) {
        akar->left = hapusPohon(akar->left, id);
    } else if (id > akar->idTugas) {
        akar->right = hapusPohon(akar->right, id);
    } else {
        if (akar->left == NULL) {
            NodePohon* temp = akar->right;
            delete akar;
            return temp;
        } else if (akar->right == NULL) {
            NodePohon* temp = akar->left;
            delete akar;
            return temp;
        }

        NodePohon* temp = cariNilaiMinimum(akar->right);
        akar->idTugas = temp->idTugas;
        akar->namaTugas = temp->namaTugas;
        akar->right = hapusPohon(akar->right, temp->idTugas);
    }
    return akar;
}

void selesaikanTugas(int id) {
    if (awalTugas == NULL) {
        cout << "\n[Peringatan] Tidak ada tugas aktif yang bisa diselesaikan.\n";
        return;
    }

    Tugas* temp = awalTugas;
    Tugas* prev = NULL;

    while (temp != NULL && temp->idTugas != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "\n[Error] Maaf, ID Tugas " << id << " tidak ditemukan pada daftar aktif.\n";
        return;
    }

    if (prev == NULL) {
        awalTugas = temp->next;
    } else {
        prev->next = temp->next;
    }

    pushStack(temp->idTugas, temp->namaTugas);
    cout << "\n>> Selamat! Tugas '" << temp->namaTugas << "' telah selesai dikerjakan dan dipindah ke riwayat.\n";
    delete temp;
}

void lihatSelesai() {
    if (atasStack == NULL) {
        cout << "\n[Info] Belum ada riwayat tugas yang selesai dikerjakan.\n";
        return;
    }
    cout << "\n--- RIWAYAT TUGAS YANG SUDAH SELESAI ---\n";
    NodeHistori* bantu = atasStack;
    while (bantu != NULL) {
        cout << " [SELESAI] ID: " << bantu->idTugas << " - " << bantu->namaTugas << "\n";
        bantu = bantu->next;
    }
}

NodePohon* sisipkanPohon(NodePohon* akar, int id, string nama) {
    if (akar == NULL) {
        NodePohon* nBaru = new NodePohon;
        nBaru->idTugas = id;
        nBaru->namaTugas = nama;
        nBaru->left = NULL;
        nBaru->right = NULL;
        return nBaru;
    }
    
    if (id == akar->idTugas) {
        cout << "\n[Sistem Pohon] ID Tugas " << id << " sudah terdaftar. Data tidak digandakan.\n";
        return akar;
    }

    if (id < akar->idTugas) {
        akar->left = sisipkanPohon(akar->left, id, nama);
    } else {
        akar->right = sisipkanPohon(akar->right, id, nama);
    }
    return akar;
}

NodePohon* telusuriPohon(NodePohon* akar, int id) {
    if (akar == NULL || akar->idTugas == id) {
        return akar;
    }
    if (id < akar->idTugas) {
        return telusuriPohon(akar->left, id);
    }
    return telusuriPohon(akar->right, id);
}

void cetakPohonUrut(NodePohon* akar) {
    if (akar != NULL) {
        cetakPohonUrut(akar->left);
        cout << "   -> ID Tugas: " << akar->idTugas << " | Judul: " << akar->namaTugas << "\n";
        cetakPohonUrut(akar->right);
    }
}

void setGraph() {
    adjMatrix[0][1] = 1;
    adjMatrix[2][0] = 1;
    adjMatrix[1][3] = 1;
    adjMatrix[3][4] = 1;
}

void cetakAlurPrasyarat() {
    cout << "\n=== PETA URUTAN PENGERJAAN TUGAS KELOMPOK (GRAPH) ===\n";
    bool benderaHubungan = false;
    for (int asal = 0; asal < MAX_GRAPH; asal++) {
        for (int tujuan = 0; tujuan < MAX_GRAPH; tujuan++) {
            if (adjMatrix[asal][tujuan] == 1) {
                cout << " * Tugas [" << namaVertex[asal] << "] wajib diselesaikan sebelum -> [" << namaVertex[tujuan] << "]\n";
                benderaHubungan = true;
            }
        }
    }
    if (!benderaHubungan) {
        cout << "Belum ada aturan urutan prasyarat tugas yang diset.\n";
    }
}

int hitungHash(string nim) {
    int hashNilai = 0;
    for (size_t idx = 0; idx < nim.length(); idx++) {
        hashNilai = (hashNilai * 31) + nim[idx];
    }
    return abs(hashNilai) % HASH_SIZE;
}

void tambahMahasiswa(string nim, string nama) {
    int index = hitungHash(nim);
    NodeAnggota* nBaru = new NodeAnggota;
    nBaru->nim = nim;
    nBaru->nama = nama;
    nBaru->next = hashTable[index];
    hashTable[index] = nBaru;
    cout << ">> Sukses menyimpan data mahasiswa " << nama << " pada slot bucket [" << index << "]\n";
}

void cariMahasiswa(string nim) {
    int index = hitungHash(nim);
    NodeAnggota* bantu = hashTable[index];
    while (bantu != NULL) {
        if (bantu->nim == nim) {
            cout << "\n[Data Ditemukan] Terdaftar di Slot Index " << index << "\n";
            cout << "-> NIM  : " << bantu->nim << "\n";
            cout << "-> Nama : " << bantu->nama << "\n";
            return;
        }
        bantu = bantu->next;
    }
    cout << "\n[Pemberitahuan] Mahasiswa dengan NIM " << nim << " tidak terdaftar di sistem.\n";
}

void tampilkanTugasAktif() {
    Tugas* curr = awalTugas;
    if (curr == NULL) {
        cout << "\n[Info] Daftar tugas yang sedang aktif saat ini kosong.\n";
        return;
    }
    cout << "\n--- DAFTAR JALUR TUGAS YANG SEDANG AKTIF ---\n";
    while (curr != NULL) {
        cout << " -> ID: " << curr->idTugas << " | Judul: " << curr->namaTugas << " | Sisa Batas: " << curr->deadline << " Hari\n";
        curr = curr->next;
    }
}

void bersihkanPohon(NodePohon* akar) {
    if (akar != NULL) {
        bersihkanPohon(akar->left);
        bersihkanPohon(akar->right);
        delete akar;
    }
}

void bersihkanSemuaMemori() {
    while (awalTugas != NULL) {
        Tugas* temp = awalTugas;
        awalTugas = awalTugas->next;
        delete temp;
    }
    while (antreanDepan != NULL) {
        Tugas* temp = antreanDepan;
        antreanDepan = antreanDepan->next;
        delete temp;
    }
    while (atasStack != NULL) {
        NodeHistori* temp = atasStack;
        atasStack = atasStack->next;
        delete temp;
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        NodeAnggota* curr = hashTable[i];
        while (curr != NULL) {
            NodeAnggota* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    bersihkanPohon(pohonTugas);
}

int main() {
    setGraph(); 

    tambahMahasiswa("188", "Rafi");
    tambahMahasiswa("200", "Budi");
    tambahMahasiswa("250", "Siti");
    tambahMahasiswa("252", "Andi");
    
    int pilihan, id, dl;
    string nama, nim;

    do {
        cetakBanner();
        cout << "1. Daftarkan Tugas Baru ke Antrean \n";
        cout << "2. Validasi Antrean Masuk ke Daftar Aktif \n";
        cout << "3. Urutkan Tugas Berdasarkan Batas Waktu \n";
        cout << "4. Tandai Tugas Selesai Dikerjakan \n";
        cout << "5. Cari Data Tugas Spesifik \n";
        cout << "6. Tampilkan Alur Prasyarat Tugas Kelompok \n";
        cout << "7. Daftarkan Anggota Kelompok Baru \n";
        cout << "8. Cari Informasi Mahasiswa \n";
        cout << "9. Keluar Aplikasi\n";
        buatGaris();
        cout << "Pilihan menu Anda: "; cin >> pilihan;

        if (pilihan == 1) {
            cout << "\n--- FORM INPUT ANTRIAN TUGAS BARU ---\n";
            cout << "Masukkan ID Tugas (Angka): "; cin >> id;
            cin.ignore();
            cout << "Masukkan Judul Tugas    : "; getline(cin, nama);
            cout << "Sisa Waktu (Dalam Hari) : "; cin >> dl;
            cin.ignore();
            
            masukAntrean(id, nama, dl);
            pohonTugas = sisipkanPohon(pohonTugas, id, nama);
            
        } else if (pilihan == 2) {
            tampilkanAntrean();
            validasiTugas();
            tampilkanTugasAktif();
            
        } else if (pilihan == 3) {
            urutTugas();
            tampilkanTugasAktif();
            
        } else if (pilihan == 4) {
            tampilkanTugasAktif();
            buatGaris();
            cout << "Masukkan ID Tugas yang berhasil dikerjakan: "; cin >> id;
            selesaikanTugas(id);
            lihatSelesai();
            
        } else if (pilihan == 5) {
            cout << "\n--- INDEX TUGAS DALAM POHON BINER (IN-ORDER) ---\n";
            cetakPohonUrut(pohonTugas);
            buatGaris();
            cout << "Masukkan ID Tugas yang ingin dicari posisinya: "; cin >> id;
            
            NodePohon* hasil = telusuriPohon(pohonTugas, id);
            if (hasil != NULL) {
                cout << "\n[Hasil Pencarian] Elemen Tugas Ditemukan!\n";
                cout << "   Judul Tugas: " << hasil->namaTugas << " (ID: " << hasil->idTugas << ")\n";
            } else {
                cout << "\n[Hasil Pencarian] ID Tugas " << id << " tidak berada di dalam pohon biner.\n";
            }
            
        } else if (pilihan == 6) {
            cetakAlurPrasyarat();
            
        } else if (pilihan == 7) {
            cout << "\n--- FORM REGISTRASI MAHASISWA BARU ---\n";
            cout << "Masukkan NIM mahasiswa : "; cin >> nim;
            cin.ignore();
            cout << "Masukkan Nama Lengkap : "; getline(cin, nama);
            tambahMahasiswa(nim, nama);
            
        } else if (pilihan == 8) {
            cout << "\n--- FORM PENCARIAN MAHASISWA ---\n";
            cout << "Masukkan NIM Mahasiswa yang dicari: "; cin >> nim;
            cariMahasiswa(nim);
            
        } else if (pilihan == 9) {
            buatGaris();
            cout << "Membersihkan sisa alokasi memori RAM sistem...\n";
            bersihkanSemuaMemori();
            cout << "Keluar dari program. Terima kasih telah menggunakan Sistem Manajemen Tugas & Kolaborasi Proyek Kelompok \n";
            buatGaris();
        } else {
            cout << "\n[Peringatan] Pilihan menu tidak valid, silakan coba lagi.\n";
        }
        
        cout << "\nTekan Enter untuk melanjutkan...";
        cin.ignore();
        cin.get();
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

    } while (pilihan != 9);

    return 0;
}