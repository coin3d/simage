include(FindPackageHandleStandardArgs)

find_path(
	GDIPLUS_INCLUDE_DIRS
	NAMES
	GdiPlus.h gdiplus.h
	HINTS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1A;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.0;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.0A;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.1;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.1A;InstallationFolder]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot81]"
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]"
	PATH_SUFFIXES
	Include
	Include/um
	Include/shared
)

mark_as_advanced(GDIPLUS_INCLUDE_DIRS)

set(GDIPLUS_LIBRARIES gdiplus)

find_package_handle_standard_args(
	GdiPlus
	DEFAULT_MSG
	GDIPLUS_INCLUDE_DIRS
	GDIPLUS_LIBRARIES
)
