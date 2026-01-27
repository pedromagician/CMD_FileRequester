# FileRequester
FileRequester for Windows command line.

A lightweight Windows command‑line tool that opens native file/folder dialogs and returns the selected path.
Perfect for batch scripts, automation, and tools that need interactive file selection.

Note: The latest Microsoft Visual C++ Redistributable (x86) is required to run the program. See: [https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170]

![bench](https://raw.githubusercontent.com/pedromagician/CMD_FileRequester/main/pic/screenshot.png)

## 🎯 Overview

`filerequester.exe` launches a standard Windows dialog (Open, Save, or Folder Select) and prints the chosen path to stdout.
This makes it easy to integrate GUI file selection into batch files or console utilities.

## 📦 Basic Usage

### Open File dialog
```bat
filerequester -open
```

### Save File dialog
```bat
filerequester -save
```

### Select Folder dialog
```bat
filerequester -directory
```

## 🧩 Batch Script Example

Store the selected path into a variable:
```bat
@echo|set /p="@set mytmp=">1.bat
@filerequester -open>>1.bat
@call 1.bat
@echo %mytmp%
```

## 🔧 Command‑Line Options

## 🔧 Command‑Line Options

| Switch | Description |
|--------|-------------|
| `-open` | Show Open File dialog |
| `-save` | Show Save File dialog |
| `-directory` | Select a folder |
| `-path <folder>` | Initial directory |
| `-title <text>` | Dialog title |
| `-filename <name>` | Default filename |
| `-ok <label>` | Custom OK button text |
| `-filter <text>` | File filter (e.g. `"Images|*.png;*.jpg|All|*.*"`) |
| `-drawersonly`, `-foldersonly` | Removes the File gadget, effectively turning the Open dialog into a folder‑only selector (Open mode only) |

## 📜 License

Free to use.
Amiga Rulez!
