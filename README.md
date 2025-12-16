# 🛡️ SysAdmin Simulator: Server Defense

> **"Server compromised. Locate and delete the malware."**


## 📖 About The Project

**SysAdmin Simulator** adalah game berbasis CLI (*Command Line Interface*) yang ditulis menggunakan bahasa C. Game ini mensimulasikan pengalaman seorang System Administrator yang harus menyelamatkan server dari serangan *malware* di lingkungan terminal Linux.

Project ini dibuat untuk mereka yang menghargai estetika *retro computing*, logika sistem operasi, dan ketegangan mencari "jarum di tumpukan jerami" tanpa bantuan GUI (Graphical User Interface).

**Kenapa game ini dibuat?**
Karena GUI itu untuk *user* biasa. Admin sejati pakai Terminal. 😎

## ✨ Key Features

* **Simulated Filesystem**: Navigasi direktori (`/home`, `/etc`, `/root`) layaknya di OS Linux asli.
* **Malware Hunting Mechanics**: Gunakan logika dan *scanning tool* untuk membedakan file sistem yang aman vs virus berbahaya.
* **Risk System**: Hapus file yang salah, dan nyawamu berkurang (karena kamu baru saja menghapus file konfigurasi penting!).
* **Save/Load System**: Progress permainan tersimpan secara otomatis.
* **Cross-Platform Logic**: Kode ditulis agar kompatibel (sebagian besar) dengan *compiler* standar C.
* **Aesthetic UI**: Menggunakan ANSI escape codes untuk pewarnaan teks yang menarik (Red for Danger, Cyan for System).

## 🎮 Controls / Commands

Game ini dikendalikan sepenuhnya lewat ketikan *keyboard*:

| Command | Deskripsi |
| :--- | :--- |
| `ls` | Menampilkan isi folder saat ini (Folder berwarna BIRU). |
| `cd <dir>` | Pindah direktori (Gunakan `..` untuk kembali). |
| `cat <file>` | Membaca isi file text. |
| `scan <file>` | Menganalisa file untuk mendeteksi *signature* virus. |
| `rm <file>` | Menghapus file (Hati-hati! Jangan hapus file sistem). |
| `save` | Menyimpan progress (sisa nyawa). |
| `clear` | Membersihkan layar terminal. |
| `help` | Menampilkan menu bantuan. |
| `quit` | Keluar dari game. |

## 🛠️ How to Build & Run

Pastikan kamu memiliki compiler C (seperti GCC) terinstall di komputer kamu.

### Linux / WSL / macOS
1.  Clone repo ini.
2.  Buka terminal di folder project.
3.  Compile kode:
    ```bash
    gcc sysadmin_game.c -o sysadmin
    ```
4.  Jalankan game:
    ```bash
    ./sysadmin
    ```

### Windows (MinGW/CMD)
1.  Pastikan GCC sudah masuk path variable.
2.  Compile kode:
    ```cmd
    gcc sysadmin_game.c -o sysadmin.exe
    ```
3.  Jalankan game:
    ```cmd
    sysadmin.exe
    ```

## 🧩 The Mission

Kamu login sebagai **root**. Ada virus tersembunyi di suatu tempat di dalam direktori server. 
* **Hint:** Baca file `readme.txt` begitu game dimulai.
* **Warning:** Jangan asal hapus file `.exe` atau `.sys` tanpa di-*scan* dulu!

## 👨‍💻 Author

**Henix**
* Informatics Student @ UNS
* System Enthusiast

---
*Built with ❤️ and C.*
