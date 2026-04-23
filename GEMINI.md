# Karbitcoin Development Guidelines

Dokumen ini berisi instruksi khusus untuk pengembangan proyek Karbitcoin menggunakan Gemini CLI. Instruksi di sini bersifat wajib dan mendahului instruksi umum lainnya.

## Alur Kerja Pengembangan

### 1. Perencanaan (Planning)
Setiap kali ingin mengimplementasikan fitur baru atau memperbaiki bug, buatlah rencana implementasi terlebih dahulu dengan menggunakan plan mode. Rencana ini harus mencakup:
- **Deskripsi Fitur/Bug**: Penjelasan singkat tentang apa yang akan dikerjakan.
- **Library Tambahan**: Daftar library baru yang dibutuhkan (jika ada).
- **Target Akhir**: Definisi sukses atau hasil akhir yang diharapkan.
- **Informasi Relevan**: Detail teknis atau catatan tambahan lainnya.

### 2. Test Driven Development (TDD)
Proyek ini dikembangkan dengan prinsip TDD. Sebelum mengimplementasikan fitur atau perbaikan:
- Buat test case yang merepresentasikan fungsionalitas yang diharapkan.
- Pastikan test tersebut gagal (karena fitur belum ada).
- Baru kemudian implementasikan kode hingga test tersebut berhasil (pass).

### 3. Struktur Fase
Bagi rencana menjadi beberapa fase yang logis. Setiap fase harus dianggap sebagai satu unit kerja yang akan menjadi satu commit (termasuk test dan implementasinya).

### 4. Manajemen Branch
- **DILARANG** melakukan pengembangan langsung di branch `main`.
- Sebelum memulai implementasi rencana, pastikan untuk membuat dan checkout ke branch baru dari `main` (misal: `feat/nama-fitur` atau `fix/deskripsi-bug`).

### 4. Komitmen Perubahan (Committing)
- Lakukan commit setiap kali selesai menjalankan satu fase dalam rencana.
- Gunakan standar **Conventional Commits** untuk pesan commit (contoh: `feat: add block validation logic`, `fix: resolve utxo lookup bug`).

### 5. Post Implementation
Setelah menyelesaikan rencana implementasi, update README.md agar sesuai dengan progress project terbaru. Setelah itu, push perubahannya ke remote.

## Catatan Tambahan
Selalu prioritaskan integritas kode dan pengujian di setiap fase sebelum melangkah ke fase berikutnya.
