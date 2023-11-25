#pragma once

#include <windows.h>
#include <algorithm>
#include <vector>
#include <map>
#include <shlobj.h>
#include <io.h>
#include <iostream>
#include <fcntl.h>
#include <atlbase.h>

#include "Debug.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
